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
typedef struct Voice_s Voice_s;
typedef struct Contact_s Contact_s;
typedef struct Location_s Location_s;
typedef struct Venue_s Venue_s;
typedef struct UserProfilePhotos_s UserProfilePhotos_s;
typedef struct File_s File_s;

typedef struct Game_s Game_s;
typedef struct Animation_s Animation_s;

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

struct Voice_s
{
    char *file_id;
    json_int_t *duration;
    char *mime_type;
    json_int_t *file_size;
};

struct Contact_s
{
    char *phone_number;
    char *first_name;
    char *last_name;
    json_int_t *user_id;
};

struct Location_s
{
    double *longitude;
    double *latitude;
};

struct Venue_s
{
    Location_s *location;
    char *title;
    char *address;
    char *foursquare_id;
};

struct UserProfilePhotos_s
{
    json_int_t *total_count;
    PhotoSize_s **photos;
    size_t photos_len;
};

struct File_s
{
    char *file_id;
    json_int_t *file_size;
    char *file_path;
};

struct Game_s
{
    char *title;
    char *description;
    PhotoSize_s **photo;
    size_t photo_len;
    char *text;
    MessageEntity_s **text_entities;
    size_t text_entities_len;
    Animation_s *animation;
};

struct Animation_s
{
    char *file_id;
    PhotoSize_s *thumb;
    char *file_name;
    char *mime_type;
    json_int_t *file_size;
};

