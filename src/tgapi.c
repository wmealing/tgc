#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include "tgapi.h"

#define TG_URL "https://api.telegram.org/bot"

typedef struct
{
    char *data;
    size_t size;
} http_response;

/* curl share handle */
CURLSH *tg_handle;
/* api token */
char tg_token[50] = { 0 };

_Bool tg_init (char *api_token)
{
    /*
    ** Initializes the library with a token and curl share handle
    */
    CURLSHcode res;

    strncpy (tg_token, api_token, 49);

    tg_handle = curl_share_init();
    if (!tg_handle) return 1;
    res = curl_share_setopt (tg_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    if (res != CURLSHE_OK) return 1;
    res = curl_share_setopt (tg_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
    if (res != CURLSHE_OK) return 1;

    return 0;
}

void tg_cleanup (void)
{
    /*
     * Cleans up CURL share handle.
     */
    curl_share_cleanup (tg_handle);
}

size_t write_response (void *response, size_t size, size_t nmemb, void *write_struct)
{
    /*
     * CURLOPT_WRITEFUNCTION
     */

    size_t actual_size = size * nmemb;
    http_response *mem_pointer = (http_response *) write_struct;

    mem_pointer->size = actual_size;
    mem_pointer->data = malloc (mem_pointer->size);

    if (mem_pointer->data == NULL)
        return 0;

    memcpy (mem_pointer->data, response, actual_size);
    mem_pointer->data[mem_pointer->size] = '\0';

    return actual_size;
}

_Bool tg_post (http_response *response, char *method, json_t *post_json, tg_res *res)
{
    /*
     * HTTP post request wrapper
     * Telegram is fine with no application/json headers
     */

    extern CURLSH *tg_handle;
    extern char tg_token[50];
    CURLcode curl_res;
    CURL *curl_handle = curl_easy_init ();
    char url[200] = { 0 };
    char *post_data;

    if (!curl_handle)
    {
        res->ok = TG_CURLFAIL;
        res->error_code = CURLE_FAILED_INIT;
        return 1;
    }
    
    strncat (url, "https://api.telegram.org/bot", 199);
    strncat (url, tg_token, 199);
    strncat (url, method, 199);

    post_data = json_dumps (post_json, 0);
    if (!post_data)
    {
        res->ok = TG_JSONFAIL;
        return 1;
    }

    response->data = NULL;

    curl_res = curl_easy_setopt (curl_handle, CURLOPT_SHARE, tg_handle);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEFUNCTION, write_response);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEDATA, (void *) response);
    if (curl_res != CURLE_OK) goto curl_error;

    curl_res = curl_easy_setopt (curl_handle, CURLOPT_URL, url);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_POSTFIELDS, post_data);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_perform (curl_handle);
    if (curl_res != CURLE_OK) goto curl_error;

    curl_easy_cleanup (curl_handle);
    free (post_data);
    return 0;

curl_error:
    if (response->data)
        free (response->data);
    free (post_data);
	curl_easy_cleanup (curl_handle);
    res->ok = TG_CURLFAIL;
    res->error_code = curl_res;
    return 1;
}

_Bool tg_get (http_response *response, char *method, tg_res *res)
{
    /*
     * HTTP get request wrapper
     */

    extern CURLSH *tg_handle;
    extern char tg_token[50];
    CURLcode curl_res;
    CURL *curl_handle = curl_easy_init();
    char url[200] = {0};

    if (!curl_handle)
    {
        res->ok = TG_CURLFAIL;
        res->error_code = CURLE_FAILED_INIT;
        return 1;
    }
    
    strncat (url, "https://api.telegram.org/bot", 199);
    strncat (url, tg_token, 199);
    strncat (url, method, 199);

    response->data = NULL;

    curl_res = curl_easy_setopt (curl_handle, CURLOPT_SHARE, tg_handle);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEFUNCTION, write_response);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEDATA, (void *) response);
    if (curl_res != CURLE_OK) goto curl_error;

    curl_res = curl_easy_setopt (curl_handle, CURLOPT_URL, url);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_perform (curl_handle);
    if (curl_res != CURLE_OK) goto curl_error;

    curl_easy_cleanup (curl_handle);
    return 0;

curl_error:
    if (response->data)
        free (response->data);
    curl_easy_cleanup (curl_handle);
    res->ok = TG_CURLFAIL;
    res->error_code = curl_res;
    return 1;
}

_Bool is_okay (json_t *root, tg_res *res)
{
    /*
     * Checks if ok:false in the telegram response and
     * fills in relevant fields.
     */
    
    json_t *ok, *error_code, *description;
    const char *err_description;

    ok = json_object_get (root, "ok");

    if (!ok)
    {
        res->ok = TG_JSONFAIL;
        return 1;
    }

    if (json_boolean_value (ok))
        return 0;
    else
    {
        res->ok = TG_NOTOKAY;

        error_code = json_object_get (root, "error_code");
        res->error_code = json_integer_value (error_code);

        description = json_object_get (root, "description");
        err_description = json_string_value (description);
        if (err_description)
            strncpy (res->description, err_description, 99);

        return 1;
    }
}

json_t *tg_load (char **data, json_t **resp_obj, tg_res *res)
{
    json_t *result;

    /* Loads the response. */
    *resp_obj = json_loads (*data, 0, &res->json_err);
    free (*data);

    if (!*resp_obj)
    {
        res->ok = TG_JSONFAIL;
        return NULL;
    }

    /* Checks if ok:true */
    if (is_okay (*resp_obj, res))
    {
        json_decref (*resp_obj);
        return NULL;
    }

    /* Loads the result */
    result = json_object_get (*resp_obj, "result");
    if (!result)
    {
        json_decref (*resp_obj);
        res->ok = TG_JSONFAIL;
    } 
    
    return result;
}

User_s getMe (tg_res *res)
{
    /*
     * Makes a getMe request
     * Returns basic information about the bot in form of a User object.
     * https://core.telegram.org/bots/api/#getme
     */

    http_response response;
    json_t *response_obj, *result;
    User_s api_s = { NULL };
    *res = (tg_res){ 0 };
    
    /* Make request */
    if (tg_get (&response, "/getMe", res))
        return api_s;

    /* Checks and gets the result */
    result = tg_load (&response.data, &response_obj, res);
    if (!result)
        return api_s;
    
    /* Parse the result */
    user_parse (result, &api_s, res);

    /* Clean up and return */
    json_decref (response_obj);
    return api_s;
}

