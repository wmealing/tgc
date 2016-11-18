#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "tgapi.h"

void parse_str (json_t *root, char **target, char *field, tg_res *res)
{
    /*
     * Copies a string from a JSON object to a target.
     */

    json_t *field_obj = json_object_get (root, field);
    int str_size;

    if (!field_obj)
    {
        res->ok = TG_JSONFAIL;
        *target = NULL;
        return;
    }

    const char *tmp_str = json_string_value (field_obj);

    if (tmp_str)
    {
        str_size = strlen (tmp_str) + 1;
        *target = malloc (str_size);
 
        if (*target)
        {
            strncpy (*target, tmp_str, str_size);
            json_decref (field_obj);
            return;
        }
        else
        {
            json_decref (field_obj);
            res->ok = TG_ALLOCFAIL;
            return;
        }
    }
    else
    {
        *target = NULL;
        json_decref (field_obj);
        return;
    }
}

void parse_int (json_t *root, json_int_t **target, char *field, tg_res *res)
{
    /*
     * Copies an int from a JSON object to a target.
     */

    json_t *field_obj = json_object_get (root, field);

    if (!field_obj)
    {
        res->ok = TG_JSONFAIL;
        *target = NULL;
        return;
    }

    /*
     * json_integer_value works like this:
     * Returns the associated value of integer, or 0 if json is not a JSON integer.
     * So no way of properly differentiating errors
     */

    *target = malloc (sizeof (json_int_t));

    if (*target)
    {
        **target = json_integer_value (field_obj);
        json_decref (field_obj);
        return;
    }
    else
    {
        json_decref (field_obj);
        res->ok = TG_ALLOCFAIL;
        return;
    }
}

void parse_double (json_t *root, double **target, char *field, tg_res *res)
{
    /*
     * Copies a double from a JSON object to a target.
     */

    json_t *field_obj = json_object_get (root, field);

    if (!field_obj)
    {
        res->ok = TG_JSONFAIL;
        *target = NULL;
        return;
    }

    *target = malloc (sizeof (double));

    if (*target)
    {
        **target = json_real_value (field_obj);
        json_decref (field_obj);
        
        return;
    }
    else
    {
        json_decref (field_obj);
        res->ok = TG_ALLOCFAIL;
        return;
    }
}

void parse_bool (json_t *root, _Bool **target, char *field, tg_res *res)
{
    /*
     * Copies a bool from a JSON object to a target.
     */

    json_t *field_obj = json_object_get (root, field);

    if (!field_obj)
    {
        res->ok = TG_JSONFAIL;
        return;
    }

    *target = malloc (sizeof (_Bool));

    if (*target)
    {
        **target = json_boolean_value (field_obj);
        json_decref (field_obj);

        return;
    }
    else
    {
        json_decref (field_obj);
        res->ok = TG_ALLOCFAIL;
        return;
    }
}

_Bool alloc_obj (size_t obj_size, void *target, tg_res *res)
{
    /*
     * Allocates memory for objects and sets res on failure
     */

    *(int **)target = malloc (obj_size);

    if (*(int **)target)
        return 0;
    else
    {
        res->ok = TG_ALLOCFAIL;
        return 1;
    }
}

void user_parse (json_t *root, User_s *api_s, tg_res *res)
{
    /*
     * Parses a User object
     * https://core.telegram.org/bots/api/#user
     */

    parse_int (root, &api_s->id, "id", res);
    parse_str (root, &api_s->first_name, "first_name", res);
    parse_str (root, &api_s->last_name, "last_name", res);
    parse_str (root, &api_s->username, "username", res);
}

void User_free (User_s *api_s)
{
    free (api_s->ok);
    free (api_s->id);
    free (api_s->first_name);
    free (api_s->last_name);
    free (api_s->username);
    free (api_s);
}

void chat_parse (json_t *root, Chat_s *api_s, tg_res *res)
{
    /*
     * Parses a Chat object
     * https://core.telegram.org/bots/api/#chat
     */
    
    parse_int (root, &api_s->id, "id", res);
    parse_str (root, &api_s->type, "type", res);
    parse_str (root, &api_s->title, "title", res);
    parse_str (root, &api_s->username, "username", res);
    parse_str (root, &api_s->first_name, "first_name", res);
    parse_str (root, &api_s->last_name, "last_name", res);
    parse_bool (root, &api_s->all_members_are_administrators, 
            "all_members_are_administrators", res);
}

void Chat_free (Chat_s *api_s)
{
    free (api_s->id);
    free (api_s->type);
    free (api_s->title);
    free (api_s->username);
    free (api_s->first_name);
    free (api_s->last_name);
    free (api_s->all_members_are_administrators);
    free (api_s);
}

/*
 * message_parse
 * Message_free
 */

void messageentity_parse (json_t *root, MessageEntity_s *api_s, tg_res *res)
{
    /*
     * Parses a MessageEntity object
     * https://core.telegram.org/bots/api/#messageentity
     */

    json_t *user = json_object_get (root, "user");

    parse_str (root, &api_s->type, "type", res);
    parse_int (root, &api_s->offset, "offset", res);
    parse_int (root, &api_s->length, "length", res);
    parse_str (root, &api_s->url, "url", res);
    
    if (user)
    {
        if (!alloc_obj (sizeof (User_s), &api_s->user, res))
            user_parse (user, api_s->user, res);
        json_decref (user);
    } else
        api_s->user = NULL;
}

void MessagEntity_free (MessageEntity_s *api_s)
{
    free (api_s->type);
    free (api_s->offset);
    free (api_s->length);
    free (api_s->url);
    free (api_s);
    if (api_s->user)
        User_free (api_s->user);
}

void photosize_parse (json_t *root, PhotoSize_s *api_s, tg_res *res)
{
    /*
     * Parses a PhotoSize object
     * https://core.telegram.org/bots/api/#photosize
     */

    parse_str (root, &api_s->file_id, "file_id", res);
    parse_int (root, &api_s->width, "width", res);
    parse_int (root, &api_s->height, "height", res);
    parse_int (root, &api_s->file_size, "file_size", res);
}

void PhotoSize_free (PhotoSize_s *api_s)
{
    free (api_s->file_id);
    free (api_s->width);
    free (api_s->height);
    free (api_s->file_size);
}

void audio_parse (json_t *root, Audio_s *api_s, tg_res *res)
{
    /*
     * Parses an Audio object
     * https://core.telegram.org/bots/api/#audio
     */
    
    parse_str (root, &api_s->file_id, "file_id", res);
    parse_int (root, &api_s->duration, "duration", res);
    parse_str (root, &api_s->performer, "performer", res);
    parse_str (root, &api_s->title, "title", res);
    parse_str (root, &api_s->mime_type, "mime_type", res);
    parse_int (root, &api_s->file_size, "file_size", res);
}

void Audio_free (Audio_s *api_s)
{
    free (api_s->file_id);
    free (api_s->duration);
    free (api_s->performer);
    free (api_s->title);
    free (api_s->mime_type);
    free (api_s->file_size);
}

void document_parse (json_t *root, Document_s *api_s, tg_res *res)
{
    /*
     * Parses a Document object
     * https://core.telegram.org/bots/api/#document
     */
    
    json_t *thumb = json_object_get (root, "thumb");

    parse_str (root, &api_s->file_id, "file_id", res);
    parse_str (root, &api_s->file_name, "file_name", res);
    parse_str (root, &api_s->mime_type, "mime_type", res);
    parse_int (root, &api_s->file_size, "file_size", res);
    
    if (thumb)
    {
        if (!alloc_obj (sizeof (PhotoSize_s), &api_s->thumb, res))
            photosize_parse (thumb, api_s->thumb, res);
        json_decref (thumb);
    } else
        api_s->thumb = NULL;
}

void Document_free (Document_s *api_s)
{
    free (api_s->file_id);
    free (api_s->file_name);
    free (api_s->mime_type);
    free (api_s->file_size);

    if (api_s->thumb)
        PhotoSize_free (api_s->thumb);
}

void sticker_parse (json_t *root, Sticker_s *api_s, tg_res *res)
{
    json_t *thumb = json_object_get (root, "thumb");

    parse_str (root, &api_s->file_id, "file_id", res);
    parse_int (root, &api_s->width, "width", res);
    parse_int (root, &api_s->height, "height", res);
    parse_str (root, &api_s->emoji, "emoji", res);
    parse_int (root, &api_s->file_size, "file_size", res);

    if (thumb)
    {
        if (!alloc_obj (sizeof (PhotoSize_s), &api_s->thumb, res))
            photosize_parse (thumb, api_s->thumb, res);
        json_decref (thumb);
    } else
        api_s->thumb = NULL;
}

void Sticker_free (Sticker_s *api_s)
{
    free (api_s->file_id);
    free (api_s->width);
    free (api_s->height);
    free (api_s->emoji);
    free (api_s->file_size);

    if (api_s->thumb)
        PhotoSize_free (api_s->thumb);
}

void video_parse (json_t *root, Video_s *api_s, tg_res *res)
{
    json_t *thumb = json_object_get (root, "thumb");

    parse_str (root, &api_s->file_id, "file_id", res);
    parse_int (root, &api_s->width, "width", res);
    parse_int (root, &api_s->height, "height", res);
    parse_int (root, &api_s->duration, "duration", res);
    parse_str (root, &api_s->mime_type, "mime_type", res);
    parse_int (root, &api_s->file_size, "file_size", res);

    if (thumb)
    {
        if (!alloc_obj (sizeof (PhotoSize_s), &api_s->thumb, res))
            photosize_parse (thumb, api_s->thumb, res);
        json_decref (thumb);
    } else
        api_s->thumb = NULL;
}

void Video_free (Video_s *api_s)
{
    free (api_s->file_id);
    free (api_s->width);
    free (api_s->height);
    free (api_s->duration);
    free (api_s->mime_type);
    free (api_s->file_size);
    
    if (api_s->thumb)
        PhotoSize_free (api_s->thumb);
}

void voice_parse (json_t *root, Voice_s *api_s, tg_res *res)
{
    parse_str (root, &api_s->file_id, "file_id", res);
    parse_int (root, &api_s->duration, "duration", res);
    parse_str (root, &api_s->mime_type, "mime_type", res);
    parse_int (root, &api_s->file_size, "file_size", res);
}

void Voice_free (Voice_s *api_s)
{
    free (api_s->file_id);
    free (api_s->duration);
    free (api_s->mime_type);
    free (api_s->file_size);
}

void contact_parse (json_t *root, Contact_s *api_s, tg_res *res)
{
    parse_str (root, &api_s->phone_number, "phone_number", res);
    parse_str (root, &api_s->first_name, "first_name", res);
    parse_str (root, &api_s->last_name, "last_name", res);
    parse_int (root, &api_s->user_id, "user_id", res);
}

void Contact_free (Contact_s *api_s)
{
    free (api_s->phone_number);
    free (api_s->first_name);
    free (api_s->last_name);
    free (api_s->user_id);
}

void location_parse (json_t *root, Location_s *api_s, tg_res *res)
{
    parse_double (root, &api_s->longitude, "longitude", res);
    parse_double (root, &api_s->latitude, "latitude", res);
}

void Location_free (Location_s *api_s)
{
    free (api_s->longitude);
    free (api_s->latitude);
}

