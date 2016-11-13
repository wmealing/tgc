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
    ** Initializes the library with a token and curl handle
    ** Will be expanded to support threading later
    */
    CURLSHcode res;

    tg_handle = curl_share_init ();
    if (!tg_handle) return 0;
    res = curl_share_setopt (tg_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    if (res != CURLSHE_OK) return 0;
    res = curl_share_setopt (tg_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
    if (res != CURLSHE_OK) return 0;
    tg_token = api_token;

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
    mem_pointer->data = malloc(mem_pointer->size);

    if(mem_pointer->data == NULL) {
        fprintf(stderr, "write_response - malloc() failed to allocate memory.\n");
        return 0;
    }

    memcpy(mem_pointer->data, response, actual_size);
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
    CURLcode tg_res;
    CURL *curl_handle = curl_easy_init ();

    if (!curl_handle)
        return 0;

    response->data = NULL;

    tg_res = curl_easy_setopt (curl_handle, CURLOPT_SHARE, tg_handle);
    if (tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEFUNCTION, write_response);
    if (tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) response);
    if (tg_res != CURLE_OK) goto curl_error;

    tg_res = curl_easy_setopt (curl_handle, CURLOPT_URL, url);
    if (tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_setopt (curl_handle, CURLOPT_POST, 1);
    if (tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_setopt (curl_handle, CURLOPT_POSTFIELDS, post);
    if (tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_perform (curl_handle);
    if (tg_res != CURLE_OK) goto curl_error;

    curl_easy_cleanup (curl_handle);
    return 1;

    curl_error:
        if (response->data)
            free (response->data);
	curl_easy_cleanup (curl_handle);
        fprintf (stderr, "CURL: %s\n", curl_easy_strerror (tg_res));
        return 0;
}

_Bool tg_get (http_response *response, char *url)
{
    /*
     * HTTP get request wrapper
     */

    extern CURLSH *tg_handle;
    CURLcode tg_res;
    CURL *curl_handle = curl_easy_init ();

    if (!curl_handle)
            return 0;

    response->data = NULL;

    tg_res = curl_easy_setopt (curl_handle, CURLOPT_SHARE, tg_handle);
    if (tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEFUNCTION, write_response);
    if (tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEDATA, (void *) response);
    if (tg_res != CURLE_OK) goto curl_error;

    tg_res = curl_easy_setopt (curl_handle, CURLOPT_URL, url);
    if (tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_setopt (curl_handle, CURLOPT_HTTPGET, 1);
    if (tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_perform (curl_handle);
    if(tg_res != CURLE_OK) goto curl_error;

    curl_easy_cleanup (curl_handle);
    return 1;

    curl_error:
        if (response->data)
            free (response->data);
        fprintf (stderr, "CURL: %s\n", curl_easy_strerror (tg_res));
        return 0;
}

tgcode getMe (User_s *api_s)
{
    /*
     * Makes a getMe request
     */

    char url[100];
    http_response response;
    json_t *resp_obj, *result;
    json_error_t json_err;
    _Bool success;

    /* Format URL */
    sprintf (url, TG_URL "%s/getMe", tg_token);

    /* Make the get request */
    success = tg_get (&response, url);
    if (!success)
        return TG_CURLFAIL;

    /* Load the response */
    resp_obj = json_loads (response.data, 0, &json_err);
    free (response.data);
    if (!resp_obj)
        return TG_JSONFAIL;

    /* Check the response and get the result */
    if (!is_okay (resp_obj, api_s))
        return TG_NOTOKAY;
    result = json_object_get (resp_obj, "result");

    /* Parse the result */
    success = user_parse (result, api_s);

    /* Clean up and return */
    json_decref (result);
    json_decref (resp_obj);
    
    if (success)
        return TG_OKAY;
    else
        return TG_PARSERFAIL;
}

