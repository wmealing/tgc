#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "tgapi.h"

/*
 * Object free helper
 */

#define OBJ_FREE(obj, obj_freer) if (obj) { obj_freer (obj); free (obj); }

#define OBJ_ARR_FREE(obj, obj_len, obj_freer)\
    for (int i = 0; i < obj_len; i++)\
        obj_freer (&obj[i]);\
    free (obj)

/*
 * Object parse helper
 */
#define OBJ_PARSE(root_json_obj, json_obj, field, obj, obj_type, obj_parser)\
    json_obj = json_object_get (root_json_obj, field);\
    if (json_obj)\
    {\
        if (!alloc_obj (sizeof (obj_type), &obj, res))\
            obj_parser (json_obj, obj, res);\
    } else\
        obj = NULL

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

void update_parse (json_t *root, Update_s *api_s, tg_res *res)
{
    json_t *field;

    OBJ_PARSE (root, field, "message", api_s->message, Message_s, message_parse);
    OBJ_PARSE (root, field, "edited_message", api_s->edited_message, Message_s, message_parse);
    OBJ_PARSE (root, field, "channel_post", api_s->channel_post, Message_s, message_parse);
    OBJ_PARSE (root, field, "edited_channel_post", api_s->edited_channel_post, Message_s, message_parse);
}

void Update_free (Update_s *api_s, size_t arr_length)
{
    for (int i = 0; i < arr_length; i++)
    {
        free (api_s[i].message);
        free (api_s[i].edited_message);
        free (api_s[i].channel_post);
        free (api_s[i].edited_channel_post);
    }
}

void user_parse (json_t *root, User_s *api_s, tg_res *res)
{
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
}

void chat_parse (json_t *root, Chat_s *api_s, tg_res *res)
{
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
}

void message_parse (json_t *root, Message_s *api_s, tg_res *res)
{
    json_t *field;

    parse_int (root, &api_s->message_id, "message_id", res);
    parse_int (root, &api_s->date, "date", res);
    parse_int (root, &api_s->forward_from_message_id, "forward_from_message_id", res);
    parse_int (root, &api_s->forward_date, "forward_date", res);
    parse_int (root, &api_s->edit_date, "edit_date", res);
    parse_str (root, &api_s->text, "text", res);
    parse_str (root, &api_s->caption, "caption", res);
    parse_str (root, &api_s->new_chat_title, "new_chat_title", res);
    parse_bool (root, &api_s->delete_chat_photo, "delete_chat_photo", res);
    parse_bool (root, &api_s->group_chat_created, "group_chat_created", res);
    parse_bool (root, &api_s->supergroup_chat_created, "supergroup_chat_created", res);
    parse_bool (root, &api_s->channel_chat_created, "channel_chat_created", res);
    parse_int (root, &api_s->migrate_to_chat_id, "migrate_to_chat_id", res);
    parse_int (root, &api_s->migrate_from_chat_id, "migrate_from_chat_id", res);

    field = json_object_get (root, "entities");
    messageentityarr_parse (field, &api_s->entities, &api_s->entities_len, res);
    json_decref (field);

    field = json_object_get (root, "photo");
    photosizearr_parse (field, &api_s->photo, &api_s->photo_len, res);
    json_decref (field);

    field = json_object_get (root, "new_chat_photo");
    photosizearr_parse (field, &api_s->new_chat_photo, &api_s->new_chat_photo_len, res);
    json_decref (field);

    OBJ_PARSE (root, field, "from", api_s->from, User_s, user_parse);
    OBJ_PARSE (root, field, "chat", api_s->chat, Chat_s, chat_parse);
    OBJ_PARSE (root, field, "forward_from", api_s->forward_from, User_s, user_parse);
    OBJ_PARSE (root, field, "forward_from_chat", api_s->forward_from_chat, Chat_s, chat_parse);
    OBJ_PARSE (root, field, "reply_to_message", api_s->reply_to_message, Message_s, message_parse);
    OBJ_PARSE (root, field, "audio", api_s->audio, Audio_s, audio_parse);
    OBJ_PARSE (root, field, "document", api_s->document, Document_s, document_parse);
    OBJ_PARSE (root, field, "game", api_s->game, Game_s, game_parse);
    OBJ_PARSE (root, field, "sticker", api_s->sticker, Sticker_s, sticker_parse);
    OBJ_PARSE (root, field, "video", api_s->video, Video_s, video_parse);
    OBJ_PARSE (root, field, "voice", api_s->voice, Voice_s, voice_parse);
    OBJ_PARSE (root, field, "contact", api_s->contact, Contact_s, contact_parse);
    OBJ_PARSE (root, field, "location", api_s->location, Location_s, location_parse);
    OBJ_PARSE (root, field, "venue", api_s->venue, Venue_s, venue_parse);
    OBJ_PARSE (root, field, "new_chat_member", api_s->new_chat_member, User_s, user_parse);
    OBJ_PARSE (root, field, "left_chat_member", api_s->left_chat_member, User_s, user_parse);
    OBJ_PARSE (root, field, "pinned_message", api_s->pinned_message, Message_s, message_parse);
}

void Message_free (Message_s *api_s)
{
    free (api_s->message_id);
    free (api_s->date);
    free (api_s->chat);
    free (api_s->forward_from_message_id);
    free (api_s->forward_date);
    free (api_s->edit_date);
    free (api_s->text);
    free (api_s->caption);
    free (api_s->new_chat_title);
    free (api_s->delete_chat_photo);
    free (api_s->group_chat_created);
    free (api_s->supergroup_chat_created);
    free (api_s->channel_chat_created);
    free (api_s->migrate_to_chat_id);
    free (api_s->migrate_from_chat_id);

    OBJ_FREE (api_s->from, User_free);
    OBJ_FREE (api_s->chat, Chat_free);
    OBJ_FREE (api_s->forward_from, User_free);
    OBJ_FREE (api_s->forward_from_chat, Chat_free);
    OBJ_FREE (api_s->reply_to_message, Message_free);
    OBJ_FREE (api_s->entities, MessageEntity_free);
    OBJ_FREE (api_s->audio, Audio_free);
    OBJ_FREE (api_s->document, Document_free);
    OBJ_FREE (api_s->game, Game_free);
    OBJ_FREE (api_s->photo, PhotoSize_free);
    OBJ_FREE (api_s->sticker, Sticker_free);
    OBJ_FREE (api_s->video, Video_free);
    OBJ_FREE (api_s->voice, Voice_free);
    OBJ_FREE (api_s->contact, Contact_free);
    OBJ_FREE (api_s->location, Location_free);
    OBJ_FREE (api_s->venue, Venue_free);
    OBJ_FREE (api_s->new_chat_member, User_free);
    OBJ_FREE (api_s->left_chat_member, User_free);
    OBJ_FREE (api_s->new_chat_photo, PhotoSize_free);
    OBJ_FREE (api_s->pinned_message, Message_free);
}

void messageentity_parse (json_t *root, MessageEntity_s *api_s, tg_res *res)
{
    /*
     * Parses a MessageEntity object
     * https://core.telegram.org/bots/api/#messageentity
     */

    json_t *user;

    parse_str (root, &api_s->type, "type", res);
    parse_int (root, &api_s->offset, "offset", res);
    parse_int (root, &api_s->length, "length", res);
    parse_str (root, &api_s->url, "url", res);
    
    OBJ_PARSE (root, user, "user", api_s->user, User_s, user_parse);
}

void messageentityarr_parse (json_t *root, MessageEntity_s **api_s, size_t *array_size, tg_res *res)
{
    json_t *current_entity;
    size_t mem_size;

    if (!root)
    {
        *api_s = NULL;
        *array_size = 0;
        return;
    }

    *array_size = json_array_size (root);

    if (*array_size)
    {
        mem_size = sizeof (MessageEntity_s) * *array_size;
        if (!alloc_obj (mem_size, api_s, res))
        {
            for (int i = 0; i < *array_size; i++)
            {
                current_entity = json_array_get (root, i);
                messageentity_parse (current_entity, &((*api_s)[i]), res);
                json_decref (current_entity);
            }
        }
    } else
        *api_s = NULL;
}

void MessageEntity_free (MessageEntity_s *api_s)
{
    free (api_s->type);
    free (api_s->offset);
    free (api_s->length);
    free (api_s->url);
    
    OBJ_FREE (api_s->user, User_free);
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

void photosizearr_parse (json_t *root, PhotoSize_s **api_s, size_t *array_size, tg_res *res)
{
    json_t *current_photo;
    size_t mem_size;

    if (!root)
    {
        *api_s = NULL;
        *array_size = 0;
        return;
    }

    *array_size = json_array_size (root);

    if (*array_size)
    {
        mem_size = sizeof (PhotoSize_s) * *array_size;

        if (!alloc_obj (mem_size, api_s, res))
        {
            for (int i = 0; i < *array_size; i++)
            {
                current_photo = json_array_get (root, i);
                photosize_parse (current_photo, &((*api_s)[i]), res);
                json_decref (current_photo);
            }
        }
    } else
        *api_s = NULL;
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
    
    json_t *thumb;

    parse_str (root, &api_s->file_id, "file_id", res);
    parse_str (root, &api_s->file_name, "file_name", res);
    parse_str (root, &api_s->mime_type, "mime_type", res);
    parse_int (root, &api_s->file_size, "file_size", res);
    
    OBJ_PARSE (root, thumb, "thumb", api_s->thumb, PhotoSize_s, photosize_parse);
}

void Document_free (Document_s *api_s)
{
    free (api_s->file_id);
    free (api_s->file_name);
    free (api_s->mime_type);
    free (api_s->file_size);

    OBJ_FREE (api_s->thumb, PhotoSize_free);
}

void sticker_parse (json_t *root, Sticker_s *api_s, tg_res *res)
{
    json_t *thumb;

    parse_str (root, &api_s->file_id, "file_id", res);
    parse_int (root, &api_s->width, "width", res);
    parse_int (root, &api_s->height, "height", res);
    parse_str (root, &api_s->emoji, "emoji", res);
    parse_int (root, &api_s->file_size, "file_size", res);

    OBJ_PARSE (root, thumb, "thumb", api_s->thumb, PhotoSize_s, photosize_parse);
}

void Sticker_free (Sticker_s *api_s)
{
    free (api_s->file_id);
    free (api_s->width);
    free (api_s->height);
    free (api_s->emoji);
    free (api_s->file_size);
    
    OBJ_FREE (api_s->thumb, PhotoSize_free);
}

void video_parse (json_t *root, Video_s *api_s, tg_res *res)
{
    json_t *thumb;

    parse_str (root, &api_s->file_id, "file_id", res);
    parse_int (root, &api_s->width, "width", res);
    parse_int (root, &api_s->height, "height", res);
    parse_int (root, &api_s->duration, "duration", res);
    parse_str (root, &api_s->mime_type, "mime_type", res);
    parse_int (root, &api_s->file_size, "file_size", res);

    OBJ_PARSE (root, thumb, "thumb", api_s->thumb, PhotoSize_s, photosize_parse);
}

void Video_free (Video_s *api_s)
{
    free (api_s->file_id);
    free (api_s->width);
    free (api_s->height);
    free (api_s->duration);
    free (api_s->mime_type);
    free (api_s->file_size);
    
    OBJ_FREE (api_s->thumb, PhotoSize_free);
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

void venue_parse (json_t *root, Venue_s *api_s, tg_res *res)
{
    json_t *location;

    parse_str (root, &api_s->title, "title", res);
    parse_str (root, &api_s->address, "address", res);
    parse_str (root, &api_s->foursquare_id, "foursquare_id", res);

    OBJ_PARSE (root, location, "location", api_s->location, Location_s, location_parse);
}

void Venue_free (Venue_s *api_s)
{
    free (api_s->title);
    free (api_s->address);
    free (api_s->foursquare_id);

    OBJ_FREE (api_s->location, Location_free);
}

void userprofilephotos_parse (json_t *root, UserProfilePhotos_s *api_s, tg_res *res)
{
    json_t *photos;

    parse_int (root, &api_s->total_count, "total_count", res);

    photos = json_object_get (root, "photos");
    photosizearr_parse (photos, &api_s->photos, &api_s->photos_len, res);
    json_decref (photos);
}

void UserProfilePhotos_free (UserProfilePhotos_s *api_s)
{
    free (api_s->total_count);
    
    OBJ_ARR_FREE (api_s->photos, api_s->photos_len, PhotoSize_free);
}

void file_parse (json_t *root, File_s *api_s, tg_res *res)
{
    parse_str (root, &api_s->file_id, "file_id", res);
    parse_int (root, &api_s->file_size, "file_size", res);
    parse_str (root, &api_s->file_path, "file_path", res);
}

void File_free (File_s *api_s)
{
    free (api_s->file_id);
    free (api_s->file_size);
    free (api_s->file_path);
}

void game_parse (json_t *root, Game_s *api_s, tg_res *res)
{
    json_t *photo, *text_entities, *animation;
    
    parse_str (root, &api_s->title, "title", res);
    parse_str (root, &api_s->description, "description", res);
    parse_str (root, &api_s->text, "text", res);

    photo = json_object_get (root, "photo");
    photosizearr_parse (photo, &api_s->photo, &api_s->photo_len, res);
    json_decref (photo);

    text_entities = json_object_get (root, "text_entities");
    messageentityarr_parse (text_entities, &api_s->text_entities, &api_s->text_entities_len, res);
    json_decref (text_entities);

    OBJ_PARSE (root, animation, "animation", api_s->animation, Animation_s, animation_parse);
}

void Game_free (Game_s *api_s)
{
    free (api_s->title);
    free (api_s->description);
    free (api_s->text);
    
    OBJ_ARR_FREE (api_s->photo, api_s->photo_len, PhotoSize_free);
    OBJ_ARR_FREE (api_s->text_entities, api_s->text_entities_len, MessageEntity_free);

    OBJ_FREE (api_s->animation, Animation_free);
}

void animation_parse (json_t *root, Animation_s *api_s, tg_res *res)
{
    json_t *thumb;

    parse_str (root, &api_s->file_id, "file_id", res);
    parse_str (root, &api_s->file_name, "file_name", res);
    parse_str (root, &api_s->mime_type, "mime_type", res);
    parse_int (root, &api_s->file_size, "file_size", res);

    OBJ_PARSE (root, thumb, "thumb", api_s->thumb, PhotoSize_s, photosize_parse);
}

void Animation_free (Animation_s *api_s)
{
    free (api_s->file_id);
    free (api_s->file_name);
    free (api_s->mime_type);
    free (api_s->file_size);

    OBJ_FREE (api_s->thumb, PhotoSize_free);
}

