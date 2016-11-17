#include <jansson.h>

/* Forward dec */
typedef struct tg_res tg_res;

/* Copy type from json object into a target */
void parse_str (json_t *root, char **target, char *field, tg_res *res);
void parse_int (json_t *root, json_int_t **target, char *field, tg_res *res);
void parse_bool (json_t *root, _Bool **target, char *field, tg_res *res);

/* Type parsers */
void user_parse (json_t *root, User_s *api_s, tg_res *res);
void chat_parse (json_t *root, Chat_s *api_s, tg_res *res);
void messageentity_parse (json_t *root, MessageEntity_s *api_s, tg_res *res);
void photosize_parse (json_t *root, PhotoSize_s *api_s, tg_res *res);
void audio_parse (json_t *root, Audio_s *api_s, tg_res *res);

/* Type Free Wrappers */
void User_free (User_s *api_s);
void Chat_free (Chat_s *api_s);
void MessagEntity_free (MessageEntity_s *api_s);
void PhotoSize_free (PhotoSize_s *api_s);
void Audio_free (Audio_s *api_s);

