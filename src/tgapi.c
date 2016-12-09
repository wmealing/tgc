#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include "tgapi.h"

/**
 * @file
 * @brief Heart of the library
 */

/**
 * @brief Performs a curl action and checks the response.
 * @see tg_request
 *
 * In the case of an error prints to stderr.
 */
#define CURLE_CHECK(res, func) do {\
    res = (func);\
    if (res != CURLE_OK)\
    {\
        fprintf(stderr, "Runtime error: %s returned %d at %s:%d", #func,  res, __FILE__, __LINE__);\
        goto curl_error;\
    }\
} while (0)

//! Library curl share handle
CURLSH *tg_handle;
//! Library headers
struct curl_slist *headers;
//! Api token
char tg_token[50];

/**
 * @brief HTTP response object (CURLOPT_WRITEDATA)
 * @see write_response
 */
typedef struct
{
    //! The response
    char *data;
    //! The size of response
    size_t size;
} http_response;

_Bool tg_init (const char *api_token, tg_res *res)
{
    *res = (tg_res){ 0 };
    tg_handle = NULL;
    headers = NULL;

    if (strlen (api_token) < 50)
        strncpy (tg_token, api_token, 50);
    else
    {
        res->ok = TG_TOKENFAIL;
        return 1;
    }

    headers = curl_slist_append (headers, "Content-Type: application/json");
    if (!headers)
    {
        res->ok = TG_CURLFAIL;
        return 1;
    }

    tg_handle = curl_share_init();
    if (!tg_handle)
    {
        res->ok = TG_CURLFAIL;
        return 1;
    }

    CURLE_CHECK (res->error_code, curl_share_setopt (tg_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS));
    CURLE_CHECK (res->error_code, curl_share_setopt (tg_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION));

    return 0;

curl_error:
    curl_share_cleanup (tg_handle);
    curl_slist_free_all (headers);
    return 1;
}

void tg_cleanup (void)
{
    curl_share_cleanup (tg_handle);
    curl_slist_free_all (headers);
}

/**
 * @brief Writes response to http_response (CURLOPT_WRITEFUNCTION)
 * @see http_response
 *
 * https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 */
size_t write_response (void *response, size_t size, size_t nmemb, void *write_struct)
{
    size_t real_size = size * nmemb;
    char *old_data = NULL;
    http_response *mem = (http_response *) write_struct;

    if (mem->data)
    {
        old_data = mem->data;
        mem->data = realloc (old_data, mem->size + real_size + 1);
    } else
        mem->data = malloc (real_size + 1);

    if (!mem->data)
    {
        free (old_data);
        return 0;
    }

    memcpy (&(mem->data[mem->size]), response, real_size);
    mem->size += real_size;
    mem->data[mem->size] = '\0';

    return real_size;
}

/**
 * @brief Wrapper for Telegram http requests
 * 
 * @param response Stores the response here
 * @param method Method appended to the base Telegram url
 * @param post_json Optional post json object
 * @param res Error Object
 *
 * @returns 0 on success and 1 on error.
 */
_Bool tg_request (http_response *response, char *method, json_t *post_json, tg_res *res)
{
    CURL *curl_handle;
    char url[200] = { 0 };
    char *post_data = NULL;

    response->data = NULL;
    response->size = 0;

    snprintf (url, 199, "%s%s%s", "https://api.telegram.org/bot", tg_token, method);

    curl_handle = curl_easy_init();
    if (!curl_handle)
    {
        res->ok = TG_CURLFAIL;
        res->error_code = CURLE_FAILED_INIT;
        json_decref (post_json);
        return 1;
    }

    if (post_json) {
        post_data = json_dumps (post_json, 0);

        if (!post_data)
        {
            res->ok = TG_JSONFAIL;
            json_decref (post_json);
            return 1;
        }

        CURLE_CHECK(res->error_code, curl_easy_setopt (curl_handle, CURLOPT_POSTFIELDS, post_data));
        CURLE_CHECK(res->error_code, curl_easy_setopt (curl_handle, CURLOPT_HTTPHEADER, headers));
    }

    CURLE_CHECK(res->error_code, curl_easy_setopt (curl_handle, CURLOPT_SHARE, tg_handle));
    CURLE_CHECK(res->error_code, curl_easy_setopt (curl_handle, CURLOPT_WRITEFUNCTION, write_response));
    CURLE_CHECK(res->error_code, curl_easy_setopt (curl_handle, CURLOPT_WRITEDATA, (void *) response));
    CURLE_CHECK(res->error_code, curl_easy_setopt (curl_handle, CURLOPT_URL, url));

    CURLE_CHECK(res->error_code, curl_easy_perform (curl_handle));
    
    curl_easy_cleanup (curl_handle);
    json_decref (post_json);
    free (post_data);
    return 0;

curl_error:
    free (response->data);
    json_decref (post_json);
    free (post_data);
    curl_easy_cleanup (curl_handle);
    res->ok = TG_CURLFAIL;
    return 1;
}


/**
 * @brief Checks if Telegram responds with ok:true
 *
 * If the ok object resolves to false the error code and
 * error description are copied to \p res.
 *
 * @param root Json object of the Telegram response
 * @param res Error object
 *
 * @returns 0 on success and 1 on error.
 */
_Bool is_okay (json_t *root, tg_res *res)
{
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

/**
 * @brief Checks and returns the result from a Telegram object.
 *
 * @param data Telegram response. Freed before returning.
 * @param resp_obj Stores the response object here to allow the caller to free
 * @param res Error Object
 *
 * Calls is_okay and attempts to return the result
 *
 * @returns 0 on success and 1 on error.
 */
json_t *tg_load (char **data, json_t **resp_obj, tg_res *res)
{
    json_t *result;

    *resp_obj = json_loads (*data, 0, &res->json_err);
    free (*data);

    if (!*resp_obj)
    {
        res->ok = TG_JSONFAIL;
        return NULL;
    }

    if (is_okay (*resp_obj, res))
    {
        json_decref (*resp_obj);
        return NULL;
    }

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
    http_response response;
    json_t *response_obj, *result;
    User_s api_s = { NULL };
    *res = (tg_res){ 0 };
    
    if (tg_request (&response, "/getMe", NULL, res))
        return api_s;

    result = tg_load (&response.data, &response_obj, res);
    if (!result)
        return api_s;
    
    user_parse (result, &api_s, res);

    json_decref (response_obj);
    return api_s;
}

Update_s *getUpdates (const long long offset, size_t *limit, const int timeout, tg_res *res)
{
    http_response response;
    json_t *post, *response_obj, *result;
    Update_s *api_s = NULL;
    *res = (tg_res){ 0 };
    
    post = json_object();
    
    if (!post)
    {
        res->ok = TG_JSONFAIL;
        return NULL;
    }
    json_object_set_new (post, "offset", json_integer (offset));
    json_object_set_new (post, "limit", json_integer (*limit));
    *limit = 0;
    json_object_set_new (post, "timeout", json_integer (timeout));
    
    if (tg_request (&response, "/getUpdates", post, res))
        return NULL;
    
    result = tg_load (&response.data, &response_obj, res);
    if (!result)
        return NULL;
    
    *limit = update_parse (result, &api_s, res);

    json_decref (response_obj);
    return api_s;
}

Message_s sendMessage (const char *chat_id, const char *text, const char *parse_mode, 
        const _Bool disable_web_page_preview, const _Bool disable_notification, 
        const long long reply_to_message_id, json_t *reply_markup, tg_res *res)
{
    http_response response;
    json_t *post, *response_obj, *result;
    Message_s api_s = { 0 };
    *res = (tg_res){ 0 };

    post = json_object();

    if (!post)
    {
        res->ok = TG_JSONFAIL;
        return api_s;
    }

    json_object_set_new (post, "chat_id", json_string (chat_id));
    json_object_set_new (post, "text", json_string (text));
    json_object_set_new (post, "parse_mode", json_string (parse_mode));
    json_object_set_new (post, "disable_web_page_preview", json_boolean (disable_web_page_preview));
    json_object_set_new (post, "disable_notification", json_boolean (disable_notification));
    json_object_set_new (post, "reply_to_message_id", json_integer (reply_to_message_id));
    json_object_set (post, "reply_markup", reply_markup);

    if (tg_request (&response, "/sendMessage", post, res))
        return api_s;

    result = tg_load (&response.data, &response_obj, res);
    if (!result)
        return api_s;
    
    message_parse (result, &api_s, res);

    json_decref (response_obj);
    return api_s;
}

Message_s forwardMessage (const char *chat_id, const char *from_chat_id,
        const _Bool disable_notification, const long long message_id, tg_res *res)
{
    http_response response;
    json_t *post, *response_obj, *result;
    Message_s api_s = { 0 };
    *res = (tg_res){ 0 };
    
    post = json_object();
    
    if (!post)
    {
        res->ok = TG_JSONFAIL;
        return api_s;
    }
    
    json_object_set_new (post, "chat_id", json_string (chat_id));
    json_object_set_new (post, "from_chat_id", json_string (from_chat_id));
    json_object_set_new (post, "disable_notification", json_boolean (disable_notification));
    json_object_set_new (post, "message_id", json_integer (message_id));
    
    if (tg_request (&response, "/forwardMessage", post, res))
        return api_s;
    
    result = tg_load (&response.data, &response_obj, res);
    if (!result)
        return api_s;
    
    message_parse (result, &api_s, res);
    
    json_decref (response_obj);
    return api_s;
}
