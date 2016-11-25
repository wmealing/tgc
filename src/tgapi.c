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

CURLSH *tg_handle; /* curl share handle */
char *tg_token; /* api token */

_Bool tg_init (char *api_token)
{
    /*
    ** Initializes the library with a token and curl share handle
    */
    CURLSHcode res;

    tg_token = api_token;

    tg_handle = curl_share_init ();
    if (!tg_handle) return 0;
    res = curl_share_setopt (tg_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    if (res != CURLSHE_OK) return 0;
    res = curl_share_setopt (tg_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
    if (res != CURLSHE_OK) return 0;

    return 1;
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

    if (mem_pointer->data == NULL) {
        fprintf (stderr, "write_response - malloc() failed to allocate memory.\n");
        return 0;
    }

    memcpy (mem_pointer->data, response, actual_size);
    mem_pointer->data[mem_pointer->size] = '\0';

    return actual_size;
}

_Bool tg_post (http_response *response, char *url, char *post)
{
    /*
     * HTTP post request wrapper
     * Telegram is fine with no application/json headers
     */

    extern CURLSH *tg_handle;
    CURLcode curl_res;
    CURL *curl_handle = curl_easy_init ();

    if (!curl_handle)
        return CURLE_FAILED_INIT;

    response->data = NULL;

    curl_res = curl_easy_setopt (curl_handle, CURLOPT_SHARE, tg_handle);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEFUNCTION, write_response);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEDATA, (void *) response);
    if (curl_res != CURLE_OK) goto curl_error;

    curl_res = curl_easy_setopt (curl_handle, CURLOPT_URL, url);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_POST, 1);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_POSTFIELDS, post);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_perform (curl_handle);
    if (curl_res != CURLE_OK) goto curl_error;

    curl_easy_cleanup (curl_handle);
    return CURLE_OK;

curl_error:
    if (response->data)
        free (response->data);
	curl_easy_cleanup (curl_handle);
        fprintf (stderr, "CURL: %s\n", curl_easy_strerror (curl_res));
        return 0;
}

CURLcode tg_get (http_response *response, char *url)
{
    /*
     * HTTP get request wrapper
     */

    extern CURLSH *tg_handle;
    CURLcode curl_res;
    CURL *curl_handle = curl_easy_init ();

    if (!curl_handle)
            return CURLE_FAILED_INIT;

    response->data = NULL;

    curl_res = curl_easy_setopt (curl_handle, CURLOPT_SHARE, tg_handle);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEFUNCTION, write_response);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEDATA, (void *) response);
    if (curl_res != CURLE_OK) goto curl_error;

    curl_res = curl_easy_setopt (curl_handle, CURLOPT_URL, url);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_HTTPGET, 1);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_perform (curl_handle);
    if (curl_res != CURLE_OK) goto curl_error;

    curl_easy_cleanup (curl_handle);
    return CURLE_OK;

    curl_error:
        if (response->data)
            free (response->data);
        curl_easy_cleanup (curl_handle);
        fprintf (stderr, "CURL: %s\n", curl_easy_strerror (curl_res));
        return curl_res;
}

_Bool is_okay (json_t *root, tg_res *res)
{
    /*
     * Checks if ok:false in the telegram response and
     * fills in relevant fields.
     */

    json_t *ok = json_object_get (root, "ok");

    if (!ok)
        return 0;

    if (json_boolean_value (ok))
    {
        res->ok = TG_OKAY;
        res->error_code = 0;
        *res->description = '\0';
 
        return 1;
    }
    else
    {
        const char *err_description;

        res->ok = TG_NOTOKAY;

        json_t *err_code_obj = json_object_get (root, "error_code");
        res->error_code = json_integer_value (err_code_obj);

        json_t *desc_obj = json_object_get (root, "description");
        err_description = json_string_value (desc_obj);
        if (err_description)
            strcpy (res->description, err_description);
        else
            *res->description = '\0';

        return 0;
    }
}

char *tg_load (char **data, tg_res *res)
{
    json_t *resp_obj, *result;
    char *result_txt = NULL;

    /* Loads the response. */
    resp_obj = json_loads (*data, 0, &res->json_err);
    free (*data);

    if (!resp_obj)
    {
        res->ok = TG_JSONFAIL;
        return result_txt;
    }

    /* Checks if ok:true */
    if (!is_okay (resp_obj, res))
    {
        json_decref (resp_obj);
        return result_txt;
    }

    /* Loads the result */
    result = json_object_get (resp_obj, "result");
    if (!result)
    {
        json_decref (resp_obj);
        res->ok = TG_JSONFAIL;
        return result_txt;
    }

    /* Dumps to result and cleans up */
    result_txt = json_dumps (result, 0);
    json_decref (resp_obj);
    if (result_txt)
        res->ok = TG_OKAY;
    else
        res->ok = TG_JSONFAIL;
    return result_txt;
}

tg_res getMe (User_s *api_s)
{
    /*
     * Makes a getMe request
     */

    char url[100];
    http_response response;
    char *result_txt;
    json_t *result;
    tg_res res;

    /* Format URL */
    sprintf (url, TG_URL "%s/getMe", tg_token);

    /* Make the get request */
    res.error_code = tg_get (&response, url);
    if (res.error_code != CURLE_OK)
    {
        res.ok = TG_CURLFAIL;
        return res;
    }

    /* Checks and gets the result */
    result_txt = tg_load (&response.data, &res);
    if (!result_txt)
        return res;

    /* Loads the result */
    result = json_loads (result_txt, 0, &res.json_err);
    free (result_txt);
    if (!result)
    {
        res.ok = TG_JSONFAIL;
        return res;
    }

    /* Parse the result */
    user_parse (result, api_s, &res);

    /* Clean up and return */
    json_decref (result);
    return res;
}

