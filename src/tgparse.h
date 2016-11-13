#include <jansson.h>

/* strncpy wrapper which allocs memory */
_Bool json_str_cpy (const char *data, char **target);

/* Copy type from json object into a target */
_Bool parse_str (json_t *root, char **target, char *field);
_Bool parse_int (json_t *root, void *target, char *field);
_Bool parse_bool (json_t *root, _Bool *target, char *field);

/* Parses a user object */
_Bool user_parse (json_t *root, User_s *api_s);
_Bool chat_parse (json_t *root, Chat_s *api_s);
_Bool messageentity_parse (json_t *root, MessageEntity_s *api_s);
_Bool photosize (json_t *root, PhotoSize_s *api_s);

/* Mem Free Wrappers */
void User_free (User_s *api_s);
void Chat_free (Chat_s *api_s);
void MessagEntity_free (MessageEntity_s *api_s);
void PhotoSize (PhotoSize_s *api_s);

