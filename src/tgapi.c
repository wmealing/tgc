#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include "tgapi.h"

#define TG_URL "https://api.telegram.org/bot"

// Internal use.

CURLSH *tg_handle;
char tg_token[50];

typedef struct
{
    char *data;
    size_t size;
} http_response;

_Bool tg_init (const char *api_token, tg_res *res)
{
    /*
    * Initializes the library with a token and curl share handle.
    * Call tg_cleanup() when finished with the lib.
    */
    
    *res = (tg_res){ 0 };

    tg_handle = curl_share_init();
    if (!tg_handle)
    {
        res->ok = TG_CURLFAIL;
        return 1;
    }
    res->error_code = curl_share_setopt (tg_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    if (!tg_handle)
    {
        res->ok = TG_CURLFAIL;
        return 1;
    }
    res->error_code = curl_share_setopt (tg_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
    if (!tg_handle)
    {
        res->ok = TG_CURLFAIL;
        return 1;
    }

    if (strlen (api_token) < 50)
        strncpy (tg_token, api_token, 50);
    else
    {
        res->ok = TG_TOKENFAIL;
        return 1;
    }

    return 0;
}

void tg_cleanup (void)
{
    // Cleans up the share handle.
    curl_share_cleanup (tg_handle);
}

size_t write_response (void *response, size_t size, size_t nmemb, void *write_struct)
{
    // CURLOPT_WRITEFUNCTION

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

_Bool tg_post (http_response *response, char *method, json_t *post_json, tg_res *res)
{
    // Post wrapper. Takes JSON object as argument.

    extern CURLSH *tg_handle;
    extern char tg_token[50];
    CURLcode curl_res;
    struct curl_slist *headers = NULL;
    CURL *curl_handle = curl_easy_init ();
    char url[200] = { 0 };
    char *post_data;

    if (!curl_handle)
    {
        res->ok = TG_CURLFAIL;
        res->error_code = CURLE_FAILED_INIT;
        return 1;
    }
   
    snprintf (url, 199, "%s%s%s", TG_URL, tg_token, method);
    
    post_data = json_dumps (post_json, 0);
    if (!post_data)
    {
        res->ok = TG_JSONFAIL;
        return 1;
    }
    
    response->data = NULL;
    response->size = 0;

    curl_res = curl_easy_setopt (curl_handle, CURLOPT_SHARE, tg_handle);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEFUNCTION, write_response);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_WRITEDATA, (void *) response);
    if (curl_res != CURLE_OK) goto curl_error;
    
    headers = curl_slist_append (headers, "Content-Type: application/json");
    if (!headers) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_HTTPHEADER, headers);
    if (curl_res != CURLE_OK) goto curl_error;

    curl_res = curl_easy_setopt (curl_handle, CURLOPT_URL, url);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_setopt (curl_handle, CURLOPT_POSTFIELDS, post_data);
    if (curl_res != CURLE_OK) goto curl_error;
    curl_res = curl_easy_perform (curl_handle);
    if (curl_res != CURLE_OK) goto curl_error;

    curl_easy_cleanup (curl_handle);
    curl_slist_free_all (headers);
    free (post_data);
    return 0;

curl_error:
    if (response->data)
        free (response->data);
    free (post_data);
	curl_easy_cleanup (curl_handle);
    curl_slist_free_all (headers);
    res->ok = TG_CURLFAIL;
    res->error_code = curl_res;
    return 1;
}

_Bool tg_get (http_response *response, char *method, tg_res *res)
{
    // Get wrapper.

    extern CURLSH *tg_handle;
    extern char tg_token[50];
    CURLcode curl_res;
    CURL *curl_handle = curl_easy_init();
    char url[200] = { 0 };

    if (!curl_handle)
    {
        res->ok = TG_CURLFAIL;
        res->error_code = CURLE_FAILED_INIT;
        return 1;
    }

    snprintf (url, 199, "%s%s%s", TG_URL, tg_token, method);

    response->data = NULL;
    response->size = 0;

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
     * Checks if ok:false in the Telegram response.
     * Returns 1 on failure with filled in res.
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
    
    // Make the request.
    if (tg_get (&response, "/getMe", res))
        return api_s;

    // Load and parse the result.
    result = tg_load (&response.data, &response_obj, res);
    if (!result)
        return api_s;
    
    user_parse (result, &api_s, res);

    // Clean up and return.
    json_decref (response_obj);
    return api_s;
}

Update_s *getUpdates (const long long offset, size_t *limit, const int timeout, tg_res *res)
{
    /*
     * Makes a getUpdates request.
     * Use this method to receive incoming updates using long polling.
     * An Array of Update objects is returned.
     * https://core.telegram.org/bots/api#getupdates
     */
    
    http_response response;
    json_t *post, *response_obj, *result;
    Update_s *api_s = NULL;
    *res = (tg_res){ 0 };
    
    // Prepare JSON post request object.
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
    
    // Make the request.
    if (tg_post (&response, "/getUpdates", post, res))
    {
        json_decref (post);
        return NULL;
    }
    json_decref (post);
    
    // Load and parse the result.
    result = tg_load (&response.data, &response_obj, res);
    if (!result)
        return NULL;
    
    *limit = update_parse (result, &api_s, res);

    // Clean up and return the update array.
    json_decref (response_obj);
    return api_s;
}

Message_s sendMessage (const char *chat_id, const char *text, const char *parse_mode, 
        const _Bool disable_web_page_preview, const _Bool disable_notification, 
        const long long reply_to_message_id, json_t *reply_markup, tg_res *res)
{
    /*
     * sendMessage
     * Use this method to send text messages. 
     * On success, the sent Message is returned.
     * https://core.telegram.org/bots/api#sendmessage
     */

    http_response response;
    json_t *post, *response_obj, *result;
    Message_s api_s = { 0 };
    *res = (tg_res){ 0 };

    // Prepare JSON post request object.
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

    // Make the request.
    if (tg_post (&response, "/sendMessage", post, res))
    {
        json_decref (post);
        return api_s;
    }
    json_decref (post);

    // Load and parse the result.
    result = tg_load (&response.data, &response_obj, res);
    if (!result)
        return api_s;
    
    message_parse (result, &api_s, res);

    // Clean up and return User type.
    json_decref (response_obj);
    return api_s;
}

Message_s forwardMessage (const char *chat_id, const char *from_chat_id,
        const _Bool disable_notification, const long long message_id, tg_res *res)
{
    /*
     * forwardMessage
     * Use this method to forward messages of any kind.
     * On success, the sent Message is returned.
     * https://core.telegram.org/bots/api#forwardmessage
     */
    
    http_response response;
    json_t *post, *response_obj, *result;
    Message_s api_s = { 0 };
    *res = (tg_res){ 0 };
    
    // Prepare the JSON post request object.
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
    
    // Make the request.
    if (tg_post (&response, "/forwardMessage", post, res))
    {
        json_decref (post);
        return api_s;
    }
    json_decref (post);
    
    // Load and parse the result.
    result = tg_load (&response.data, &response_obj, res);
    if (!result)
        return api_s;
    
    message_parse (result, &api_s, res);
    
    // Clean up and return User type.
    json_decref (response_obj);
    return api_s;
}
