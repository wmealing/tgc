#include <jansson.h>

/**
 * @file
 * @brief Functions to parse and free Telegram types.
 */

/**
 * @defgroup group3 Type parsers
 * @brief Internally used functions to parse Telegram results.
 * @{
 */

//! Forward dec of error type
typedef struct tg_res tg_res;

/**
 * @brief Copies a string from a json object to a target.
 * @see parse_int parse_bool parse_double
 *
 * @param root Json object used to retrieve the string.
 * @param target Where the string wil be copies to.
 * @param field The name of the json field where the string is.
 * @param res Error object.
 */
void parse_str (json_t *root, char **target, char *field, tg_res *res);

/**
 * @brief Copies an long long from a json object to a target.
 * @see parse_str parse_bool parse_double
 *
 * @param root Json object used to retrieve the integer.
 * @param target Where the long long will be copied to.
 * @param field The name of the json field where the long long is.
 * @param res Error object.
 */
void parse_int (json_t *root, json_int_t **target, char *field, tg_res *res);

/**
 * @brief Copies a boolean from a json object to a target.
 * @see parse_str parse_int parse_double
 *
 * @param root Json object used to retrieve the boolean.
 * @param target Where the boolean value will be copied to.
 * @param field The name of the json field where the boolean is.
 * @param res Error Object.
 */
void parse_bool (json_t *root, _Bool **target, char *field, tg_res *res);

/**
 * @brief Copies a double value from a json object to a target.
 * @see parse_str parse_int parse_bool
 *
 * @param root Json object used to retrieve the double.
 * @param target Where the double value will be copied to.
 * @param field The name of the json field where the double is.
 * @param res Error Object.
 */
void parse_double (json_t *root, double **target, char *field, tg_res *res);

/**
 * @brief Allocated memory for an object.
 * 
 * @param obj_size Size of allocation. `sizeof (obj)`
 * @param target Allocation target.
 * @param res Error object.
 */
_Bool alloc_obj (size_t obj_size, void *target, tg_res *res);

/**
 * @brief Parses an array of Updates.
 * @see Update_s
 *
 * @param root Json object containing an array of updates.
 * @param api_s Target for the array of updates.
 * @param res Error object.
 */
size_t update_parse (json_t *root, Update_s **api_s, tg_res *res);

/**
 * @brief Parses a User type.
 * @see User_s
 *
 * @param root Json object containing an array of updates.
 * @param api_s Target for the User_s.
 * @param res Error Object.
 */
void user_parse (json_t *root, User_s *api_s, tg_res *res);

/**
 * @brief Parses a Chat type.
 * @see Chat_s
 *
 * @param root Json object containing a User type.
 * @param api_s Target for the parsed Chat_s.
 * @param res Error Object.
 */
void chat_parse (json_t *root, Chat_s *api_s, tg_res *res);

/**
 * @brief Parses a Message type.
 * @see Message_s
 *
 * @param root Json object containing a Message type.
 * @param api_s Target for the parsed Chat_s.
 * @param res Error Object.
 */
void message_parse (json_t *root, Message_s *api_s, tg_res *res);

/**
 * @brief Parses a MessageEntity type
 * @see MessageEntity_s
 *
 * @param root Json object containing a MessageEntity type.
 * @param api_s Target for the parsed MessageEntity_s.
 * @param res Error Object.
 */
void messageentity_parse (json_t *root, MessageEntity_s *api_s, tg_res *res);

/**
 * @brief Parses a PhotoSize type.
 * @see PhotoSize_s
 *
 * @param root Json object containing a PhotoSize type.
 * @param api_s Target for the parsed PhotoSize_s.
 * @param res Error Object.
 */
void photosize_parse (json_t *root, PhotoSize_s *api_s, tg_res *res);

/**
 * @brief Parses an Audio type.
 * @see Audio_s
 *
 * @param root Json object containing an Audio type.
 * @param api_s Target for the parsed Audio_s.
 * @param res Error Object.
 */
void audio_parse (json_t *root, Audio_s *api_s, tg_res *res);

/**
 * @brief Parses a Document type.
 * @see Document_s
 *
 * @param root Json object containing Document type.
 * @param api_s Target for the parsed Document_s.
 * @param res Error Object.
 */
void document_parse (json_t *root, Document_s *api_s, tg_res *res);

/**
 * @brief Parses a Sticker type.
 * @see Sticker_s
 *
 * @param root Json object containing a Sticker type.
 * @param api_s Target for the parsed Sticker_s.
 * @param res Error Object.
 */
void sticker_parse (json_t *root, Sticker_s *api_s, tg_res *res);

/**
 * @brief Parses a Video type.
 * @see Video_s
 *
 * @param root Json object containing a Video type.
 * @param api_s Target for the parsed Video_s.
 * @param res Error Object.
 */
void video_parse (json_t *root, Video_s *api_s, tg_res *res);

/**
 * @brief Parses a Voice type.
 * @see Voice_s
 *
 * @param root Json object containing a Voice type.
 * @param api_s Target for the parsed Voice_s.
 * @param res Error Object.
 */
void voice_parse (json_t *root, Voice_s *api_s, tg_res *res);

/**
 * @brief Parses a Contact type.
 * @see Contact_s
 *
 * @param root Json object containing a Contact type.
 * @param api_s Target for the parsed Contact_s.
 * @param res Error Object.
 */
void contact_parse (json_t *root, Contact_s *api_s, tg_res *res);

/**
 * @brief Parses a Location type.
 * @see Location_s
 *
 * @param root Json object containing a Location type.
 * @param api_s Target for the parsed Location_s.
 * @param res Error Object.
 */
void location_parse (json_t *root, Location_s *api_s, tg_res *res);

/**
 * @brief Parses a Venue_s type.
 * @see Venue_s
 *
 * @param root Json object containing a Venue type.
 * @param api_s Target for the parsed Venue_s.
 * @param res Error Object.
 */
void venue_parse (json_t *root, Venue_s *api_s, tg_res *res);

/**
 * @brief Parses a UserProfilePhotos type.
 * @see UserProfilePhotos_s
 *
 * @param root Json object containing a UserProfilePhotos type.
 * @param api_s Target for the parsed UserProfilePhotos_s.
 * @param res Error Object.
 */
void userprofilephotos_parse (json_t *root, UserProfilePhotos_s *api_s, tg_res *res);

/**
 * @brief Parses a File type.
 * @see File_s
 *
 * @param root Json object containing a File type.
 * @param api_s Target for the parsed File_s.
 * @param res Error Object.
 */
void file_parse (json_t *root, File_s *api_s, tg_res *res);

/**
 * @brief Parses an CallbackQuery type.
 * @see CallbackQuery_s
 *
 * @param root Json object containing a CallbackQuery type.
 * @param api_s Target for the parsed CallbackQuery_s.
 * @param res Error Object.
 */
void callbackquery_parse (json_t *root, CallbackQuery_s *api_s, tg_res *res);

/**
 * @brief Parses an InlineQuery type.
 * @see InlineQuery_s
 *
 * @param root Json object containing a InlineQuery type.
 * @param api_s Target for the parsed File_s.
 * @param res Error Object.
 */
void inlinequery_parse (json_t *root, InlineQuery_s *api_s, tg_res *res);

 /**
  * @brief Parses a ChosenInlineResult type.
  * @see ChosenInlineResult_s
  *
 * @param root Json object containing a ChosenInlineResult type.
 * @param api_s Target for the parsed ChosenInlineResult_s.
 * @param res Error Object.
  */
void choseninlineresult_parse (json_t *root, ChosenInlineResult_s *api_s, tg_res *res);

/**
 * @brief Parses a Game type.
 * @see Game_s
 *
 * @param root Json object containing a Game type.
 * @param api_s Target for the parsed Game_s.
 * @param res Error Object.
 */
void game_parse (json_t *root, Game_s *api_s, tg_res *res);

/**
 * @brief Parses an Animation type.
 * @see Animation_s
 *
 * @param root Json object containing an Animation type.
 * @param api_s Target for the parsed Animation_ss.
 * @param res Error Object.
 */
void animation_parse (json_t *root, Animation_s *api_s, tg_res *res);

/**@}*/

/**
 * @defgroup group4 Type Freers
 * @brief Methods to clean up Telegram types.
 * @{
 */

/**
 * @brief Frees an Update type.
 * @see Update_s
 *
 * @param api_s The Update_s array to free
 * @param arr_length Length of the Update_s array
 */
void Update_free (Update_s *api_s, size_t arr_length); 

/**
 * @brief Frees a User type.
 * @see User_s
 *
 * @param api_s Object to free
 */
void User_free (User_s api_s);

/**
 * @brief Frees a Chat type.
 * @see Chat_s
 *
 * @param api_s Object to free
 */
void Chat_free (Chat_s api_s);

/**
 * @brief Frees a Message type.
 * @see Message_s
 *
 * @param api_s Object to free
 */
void Message_free (Message_s api_s);

/**
 * @brief Frees a MessageEntity type
 * @see MessageEntity_s
 *
 * @param api_s Object to free
 */
void MessageEntity_free (MessageEntity_s api_s);

/**
 * @brief Frees a PhotoSize type
 * @see PhotoSize_s
 *
 * @param api_s Object to free
 */
void PhotoSize_free (PhotoSize_s api_s);

/**
 * @brief Frees an Audio type
 * @see Audio_s
 *
 * @param api_s Object to free
 */
void Audio_free (Audio_s api_s);

/**
 * @brief Frees a Document type
 * @see Document_s
 *
 * @param api_s Object to free
 */
void Document_free (Document_s api_s);

/**
 * @brief Frees a Sticker type
 * @see Sticker_s
 *
 * @param api_s Object to free
 */
void Sticker_free (Sticker_s api_s);

/**
 * @brief Frees a Video type
 * @see Video_s
 *
 * @param api_s Object to free
 */
void Video_free (Video_s api_s);

/**
 * @brief Frees a Voice type
 * @see Voice_s
 *
 * @param api_s Object to free
 */
void Voice_free (Voice_s api_s);

/**
 * @brief Frees a Contact type
 * @see Contact_s
 *
 * @param api_s Object to free
 */
void Contact_free (Contact_s api_s);

/**
 * @brief Frees a Location type
 * @see Location_s
 *
 * @param api_s Object to free
 */
void Location_free (Location_s api_s);

/**
 * @brief Frees a Venue type
 * @see Venue_s
 *
 * @param api_s Object to free
 */
void Venue_free (Venue_s api_s);

/**
 * @brief Frees a UserProfilePhotos type
 * @see UserProfilePhotos_s
 *
 * @param api_s Object to free
 */
void UserProfilePhotos_free (UserProfilePhotos_s api_s);

/**
 * @brief Frees a CallbackQuery type
 * @see CallbackQuery_s
 *
 * @param api_s Object to free
 */
void CallbackQuery_free (CallbackQuery_s api_s);

/**
 * @brief Frees a InlineQuery type
 * @see InlineQuery_s
 *
 * @param api_s Object to free
 */
void InlineQuery_free (InlineQuery_s api_s);

/**
 * @brief Frees a ChosenInlineResult type
 * @see ChosenInlineResult_s
 *
 * @param api_s Object to free
 */
void ChosenInlineResult_free (ChosenInlineResult_s api_s);

/**
 * @brief Frees a File type
 * @see File_s
 *
 * @param api_s Object to free
 */
void File_free (File_s api_s);

/**
 * @brief Frees a Game type
 * @see Game_s
 *
 * @param api_s Object to free
 */
void Game_free (Game_s api_s);

/**
 * @brief Frees an Animation type
 * @see Animation_s
 *
 * @param api_s Object to free
 */
void Animation_free (Animation_s api_s);

/**@}*/

