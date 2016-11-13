#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "tgapi.h"

_Bool json_str_cpy (const char *data, char **target)
{
    /*
     * Wrapper for strncpy for use by parse_str.
     */

    int str_size = strlen (data);
    *target = malloc (str_size + 1);

    if (target == NULL)
        return 0;
    else
    {
        strncpy (*target, data, str_size);
        return 1;
    }
}

_Bool parse_str (json_t *root, char **target, char *field)
{
    /*
     * Copies a string from a JSON object to a target.
     */

    json_t *field_obj = json_object_get (root, field);

    if (!field_obj)
        return 0;

    const char *tmp_str = json_string_value (field_obj);

    if (*tmp_str)
    {
        json_str_cpy (tmp_str, target);
        json_decref (field_obj);
        return 1;
    }
    else
    {
        *target = NULL;
        json_decref (field_obj);
        return 0;
    }
}

_Bool parse_int (json_t *root, void *target, char *field)
{
    /*
     * Copies an int from a JSON object to a target.
     */

    json_t *field_obj = json_object_get (root, field);

    if (!field_obj)
        return 0;

    /*
     * json_integer_value works like this:
     * Returns the associated value of integer, or 0 if json is not a JSON integer.
     * So no way of properly differentiating errors
     */

    *(json_int_t *)target = json_integer_value (field_obj);
    json_decref (field_obj);

    return 1;
}

_Bool parse_bool (json_t *root, _Bool *target, char *field)
{
    /*
     * Copies a bool from a JSON object to a target.
     */

    json_t *field_obj = json_object_get (root, field);

    if (!field_obj)
        return 0;

    *target = json_boolean_value (field_obj);
    json_decref (field_obj);

    return 1;
}

_Bool user_parse (json_t *root, User_s *api_s)
{
    /*
     * Parses a User object
     */

    if (!root)
        return 0;

    if (!parse_int (root, &api_s->id, "id")) return 0;
    if (!parse_str (root, &api_s->first_name, "first_name")) return 0;
    parse_str (root, &api_s->last_name, "last_name");
    if (!parse_str (root, &api_s->username, "username")) return 0;

    return 1;
}

void User_free (User_s *api_s)
{
    free (api_s->description);
    free (api_s->first_name);
    free (api_s->last_name);
    free (api_s->username);
    free (api_s);
}

_Bool chat_parse (json_t *root, Chat_s *api_s)
{
    /*
     * Parses a Chat object
     */
    
    if (!root)
        return 0;
    
    if (!parse_int (root, &api_s->id, "id")) return 0;
    if (!parse_str (root, &api_s->type, "type")) return 0;
    parse_str (root, &api_s->title, "title");
    parse_str (root, &api_s->username, "username");
    parse_str (root, &api_s->first_name, "first_name");
    parse_str (root, &api_s->last_name, "last_name");
    parse_bool (root,
            &api_s->all_members_are_administrators,
            "all_members_are_administrators");

    return 1;
}

void Chat_free (Chat_s *api_s)
{
    free (api_s->type);
    free (api_s->title);
    free (api_s->username);
    free (api_s->first_name);
    free (api_s->last_name);
    free (api_s);
}

_Bool messageentity_parse (json_t *root, MessageEntity_s *api_s)
{
    /*
     * Parses a MessageEntity object
     */

    json_t *user;
    
    if (!root)
        return 0;

    if (!parse_str (root, &api_s->type, "type")) return 0;
    if (!parse_int (root, &api_s->offset, "offset")) return 0;
    if (!parse_int (root, &api_s->length, "length")) return 0;
    parse_str (root, &api_s->url, "url");
    
    user = json_object_get (root, "user");
    user_parse (user, api_s->user);
    json_decref (user);

    return 1;
}

void MessagEntity_free (MessageEntity_s *api_s)
{
    free (api_s->type);
    free (api_s->url);
    free (api_s);
    if (api_s->user)
        User_free (api_s->user);
}

_Bool photosize (json_t *root, PhotoSize_s *api_s)
{
    /*
     * Parses a PhotoSize object
     */
    if (!root)
        return 0;

    if (!parse_str (root, &api_s->file_id, "file_id")) return 0;
    if (!parse_int (root, &api_s->width, "width")) return 0;
    if (!parse_int (root, &api_s->height, "height")) return 0;
    parse_int (root, &api_s->file_size, "file_size");

    return 1;
}

void PhotoSize_free (PhotoSize_s *api_s)
{
    free (api_s->file_id);
}

