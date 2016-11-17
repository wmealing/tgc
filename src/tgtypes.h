#include <stdint.h>
#include <jansson.h>

/* Declarations  */
typedef struct User_s User_s;
typedef struct Chat_s Chat_s;
typedef struct MessageEntity_s MessageEntity_s;
typedef struct PhotoSize_s PhotoSize_s;
typedef struct Audio_s Audio_s;
typedef struct Document_s Document_s;
typedef struct Sticker_s Sticker_s;
typedef struct Video_s Video_s;

/*
 * User
 * This object represents a Telegram user or bot.
 * https://core.telegram.org/bots/api/#user
 */

struct User_s
{
    _Bool *ok;
    json_int_t *id;
    char *first_name;
    char *last_name;
    char *username;
};

struct Chat_s
{
    json_int_t *id;
    char *type;
    char *title;
    char *username;
    char *first_name;
    char *last_name;
    _Bool *all_members_are_administrators;
};

struct MessageEntity_s
{
    char *type;
    json_int_t *offset;
    json_int_t *length;
    char *url;
    User_s *user;
};

struct PhotoSize_s
{
    char *file_id;
    json_int_t *width;
    json_int_t *height;
    json_int_t *file_size;
};

struct Audio_s
{
    char *file_id;
    json_int_t *duration;
    char *performer;
    char *title;
    char *mime_type;
    json_int_t *file_size;
};

struct Document_s
{
    char *file_id;
    PhotoSize_s *thumb;
    char *file_name;
    char *mime_type;
    json_int_t *file_size;
};

struct Sticker_s
{
    char *file_id;
    json_int_t *width;
    json_int_t *height;
    PhotoSize_s *thumb;
    char *emoji;
    json_int_t *file_size;
};

struct Video_s
{
    char *file_id;
    json_int_t *width;
    json_int_t *height;
    json_int_t *duration;
    PhotoSize_s *thumb;
    char *mime_type;
    json_int_t *file_size;
};

