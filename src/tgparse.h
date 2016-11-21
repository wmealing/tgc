#include <jansson.h>

/* Forward dec */
typedef struct tg_res tg_res;

/* Copy type from json object into a target */
void parse_str (json_t *root, char **target, char *field, tg_res *res);
void parse_int (json_t *root, json_int_t **target, char *field, tg_res *res);
void parse_bool (json_t *root, _Bool **target, char *field, tg_res *res);
void parse_double (json_t *root, double **target, char *field, tg_res *res);

/* Allocs space for new type */
_Bool alloc_obj (size_t obj_size, void *target, tg_res *res);

/* Type parsers */
void user_parse (json_t *root, User_s *api_s, tg_res *res);
void chat_parse (json_t *root, Chat_s *api_s, tg_res *res);
void messageentity_parse (json_t *root, MessageEntity_s *api_s, tg_res *res);
void photosize_parse (json_t *root, PhotoSize_s *api_s, tg_res *res);
void audio_parse (json_t *root, Audio_s *api_s, tg_res *res);
void document_parse (json_t *root, Document_s *api_s, tg_res *res);
void sticker_parse (json_t *root, Sticker_s *api_s, tg_res *res);
void video_parse (json_t *root, Video_s *api_s, tg_res *res);
void voice_parse (json_t *root, Voice_s *api_s, tg_res *res);
void contact_parse (json_t *root, Contact_s *api_s, tg_res *res);
void location_parse (json_t *root, Location_s *api_s, tg_res *res);
void venue_parse (json_t *root, Venue_s *api_s, tg_res *res);
void userprofilephotos_parse (json_t *root, UserProfilePhotos_s *api_s, tg_res *res);
void file_parse (json_t *root, File_s *api_s, tg_res *res);

/* Type Free Wrappers */
void User_free (User_s *api_s);
void Chat_free (Chat_s *api_s);
void MessagEntity_free (MessageEntity_s *api_s);
void PhotoSize_free (PhotoSize_s *api_s);
void Audio_free (Audio_s *api_s);
void Document_free (Document_s *api_s);
void Sticker_free (Sticker_s *api_s);
void Video_free (Video_s *api_s);
void Voice_free (Voice_s *api_s);
void Contact_free (Contact_s *api_s);
void Location_free (Location_s *api_s);
void Venue_free (Venue_s *api_s);
void UserProfilePhotos_free (UserProfilePhotos_s *api_s);
void File_free (File_s *api_s);

