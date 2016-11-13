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

_Bool is_okay (json_t *root, void *api_s)
{
    /*
     * Checks if ok:false in the telegram response and
     * fills in relevant fields.
     */

    json_t *ok = json_object_get (root, "ok");
    User_s *work_ptr = (User_s *) api_s;

    if (!ok)
        return 0;

    if (json_boolean_value (ok))
    {
        work_ptr->ok = 1;
        work_ptr->error_code = 0;
        work_ptr->description = NULL;

        json_decref (ok);
        return 1;
    }
    else
    {
        const char *err_description;

        work_ptr->ok = 0;

        json_t *err_code_obj = json_object_get (root, "error_code");
        work_ptr->error_code = json_integer_value (err_code_obj);

        json_t *desc_obj = json_object_get (root, "description");
        err_description = json_string_value (desc_obj);
        if (err_description)
            json_str_cpy (err_description, &work_ptr->description);
        else
            work_ptr->description = NULL;

        json_decref (err_code_obj);
        json_decref (desc_obj);
        json_decref (ok);
        return 0;
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

