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

CURL *tg_handle; /* curl handle */
char *tg_token; /* api token */

void tg_init (char *api_token)
{
    /*
    ** Initializes the library with a token and curl handle
    ** Will be expanded to support threading later
    */
    tg_handle = curl_easy_init();
    tg_token = api_token;
}

void tg_cleanup (void)
{
    /*
     * Cleans up CURL handle.
     */
    curl_easy_cleanup (tg_handle);
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

    extern CURL *tg_handle;
    CURLcode tg_res;

    response->data = NULL;

    tg_res = curl_easy_setopt(tg_handle, CURLOPT_WRITEFUNCTION, write_response);
    if(tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_setopt(tg_handle, CURLOPT_WRITEDATA, (void *) response);
    if(tg_res != CURLE_OK) goto curl_error;

    tg_res = curl_easy_setopt(tg_handle, CURLOPT_URL, url);
    if(tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_setopt(tg_handle, CURLOPT_POST, 1);
    if(tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_setopt(tg_handle, CURLOPT_POSTFIELDS, post);
    if(tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_perform(tg_handle);
    if(tg_res != CURLE_OK) goto curl_error;

    return 1;

    curl_error:
        if(response->data) free(response->data);
        fprintf(stderr, "CURL: %s\n", curl_easy_strerror(tg_res));
        return 0;
}

_Bool tg_get (http_response *response, char *url)
{
    /*
     * HTTP get request wrapper
     */

    extern CURL *tg_handle;
    CURLcode tg_res;

    response->data = NULL;

    tg_res = curl_easy_setopt(tg_handle, CURLOPT_WRITEFUNCTION, write_response);
    if(tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_setopt(tg_handle, CURLOPT_WRITEDATA, (void *) response);
    if(tg_res != CURLE_OK) goto curl_error;

    tg_res = curl_easy_setopt(tg_handle, CURLOPT_URL, url);
    if(tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_setopt(tg_handle, CURLOPT_HTTPGET, 1);
    if(tg_res != CURLE_OK) goto curl_error;
    tg_res = curl_easy_perform(tg_handle);
    if(tg_res != CURLE_OK) goto curl_error;

    return 1;

    curl_error:
        if(response->data) free(response->data);
        fprintf(stderr, "CURL: %s\n", curl_easy_strerror(tg_res));
        return 0;
}

tgcode getMe (User_s *api_s)
{
    /*
     * Makes a getMe request
     */

    char url[100];
    http_response response;
    json_t *resp_obj;
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
    if (!resp_obj) return TG_JSONFAIL;

    /* Parse the response */
    success = user_parse (resp_obj, api_s);
    json_decref (resp_obj);
    if (success)
        return TG_OKAY;
    else
        return TG_PARSERFAIL;
}

