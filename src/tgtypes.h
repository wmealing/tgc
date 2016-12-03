#include <stdint.h>
#include <jansson.h>

/* Declarations  */
typedef struct Update_s Update_s;

typedef struct User_s User_s;
typedef struct Message_s Message_s;
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
 * Update
 * This object represents an incoming update.
 * At most one of the optional parameters can be present in any given update.
 * https://core.telegram.org/bots/api/#update
 *
 * TODO: InlineQuery and CallbackQuery support.
 */

struct Update_s
{
    json_int_t *update_id;
    Message_s *message;
    Message_s *edited_message;
    Message_s *channel_post;
    Message_s *edited_channel_post;
};

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

/*
 * Chat
 * This object represents a chat.
 * https://core.telegram.org/bots/api/#chat
 */

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

/*
 * Message
 * This object represents a message.
 * https://core.telegram.org/bots/api/#message
 */

struct Message_s
{
    json_int_t *message_id;
    User_s *from;
    json_int_t *date;
    Chat_s *chat;
    User_s *forward_from;
    Chat_s *forward_from_chat;
    json_int_t *forward_from_message_id;
    json_int_t *forward_date;
    Message_s *reply_to_message;
    json_int_t *edit_date;
    char *text;
    MessageEntity_s *entities;
    size_t entities_len;
    Audio_s *audio;
    Document_s *document;
    Game_s *game;
    PhotoSize_s *photo;
    size_t photo_len;
    Sticker_s *sticker;
    Video_s *video;
    Voice_s *voice;
    char *caption;
    Contact_s *contact;
    Location_s *location;
    Venue_s *venue;
    User_s *new_chat_member;
    User_s *left_chat_member;
    char *new_chat_title;
    PhotoSize_s *new_chat_photo;
    size_t new_chat_photo_len;
    _Bool *delete_chat_photo;
    _Bool *group_chat_created;
    _Bool *supergroup_chat_created;
    _Bool *channel_chat_created;
    json_int_t *migrate_to_chat_id;
    json_int_t *migrate_from_chat_id;
    Message_s *pinned_message;
};

/* 
 * MessageEntity
 * This object represents one special entity in a text message. For example, hashtags, usernames, URLs, etc.
 * https://core.telegram.org/bots/api/#messageentity
 */

struct MessageEntity_s
{
    char *type;
    json_int_t *offset;
    json_int_t *length;
    char *url;
    User_s *user;
};

/*
 * PhotoSize
 * This object represents one size of a photo or a file / sticker thumbnail.
 * https://core.telegram.org/bots/api/#photosize
 */

struct PhotoSize_s
{
    char *file_id;
    json_int_t *width;
    json_int_t *height;
    json_int_t *file_size;
};

/*
 * Audio
 * This object represents an audio file to be treated as music by the Telegram clients.
 * https://core.telegram.org/bots/api/#audio
 */

struct Audio_s
{
    char *file_id;
    json_int_t *duration;
    char *performer;
    char *title;
    char *mime_type;
    json_int_t *file_size;
};

/*
 * Document
 * This object represents a general file (as opposed to photos, voice messages and audio files).
 * https://core.telegram.org/bots/api/#document
 */

struct Document_s
{
    char *file_id;
    PhotoSize_s *thumb;
    char *file_name;
    char *mime_type;
    json_int_t *file_size;
};

/*
 * Sticker
 * This object represents a sticker.
 * https://core.telegram.org/bots/api/#sticker
 */

struct Sticker_s
{
    char *file_id;
    json_int_t *width;
    json_int_t *height;
    PhotoSize_s *thumb;
    char *emoji;
    json_int_t *file_size;
};

/*
 * Video
 * This object represents a video file.
 * https://core.telegram.org/bots/api/#video
 */

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

/*
 * Voice
 * This object represents a voice note.
 * https://core.telegram.org/bots/api/#voice
 */

struct Voice_s
{
    char *file_id;
    json_int_t *duration;
    char *mime_type;
    json_int_t *file_size;
};

/*
 * Contact
 * This object represents a phone contact.
 * https://core.telegram.org/bots/api/#contact
 */

struct Contact_s
{
    char *phone_number;
    char *first_name;
    char *last_name;
    json_int_t *user_id;
};

/*
 * Location
 * This object represents a point on the map.
 * https://core.telegram.org/bots/api/#location
 */

struct Location_s
{
    double *longitude;
    double *latitude;
};

/*
 * Venue
 * This object represents a venue.
 * https://core.telegram.org/bots/api/#venue
 */

struct Venue_s
{
    Location_s *location;
    char *title;
    char *address;
    char *foursquare_id;
};

/*
 * UserProfilePhotos
 * This object represent a user's profile pictures.
 * https://core.telegram.org/bots/api/#userprofilephotos
 */

struct UserProfilePhotos_s
{
    json_int_t *total_count;
    PhotoSize_s *photos;
    size_t photos_len;
};

/*
 * File
 * This object represents a file ready to be downloaded.
 * The file can be downloaded via the link https://api.telegram.org/file/bot<token>/<file_path>.
 * It is guaranteed that the link will be valid for at least 1 hour.
 * When the link expires, a new one can be requested by calling getFile.
 * https://core.telegram.org/bots/api/#file
 */

struct File_s
{
    char *file_id;
    json_int_t *file_size;
    char *file_path;
};

/*
 * Game
 * This object represents a game.
 * Use BotFather to create and edit games, their short names will act as unique identifiers.
 * https://core.telegram.org/bots/api/#game
 */

struct Game_s
{
    char *title;
    char *description;
    PhotoSize_s *photo;
    size_t photo_len;
    char *text;
    MessageEntity_s *text_entities;
    size_t text_entities_len;
    Animation_s *animation;
};

/*
 * Animation
 * This object represents an animation file to be displayed in the message containing a game.
 * https://core.telegram.org/bots/api/#animation
 */

struct Animation_s
{
    char *file_id;
    PhotoSize_s *thumb;
    char *file_name;
    char *mime_type;
    json_int_t *file_size;
};
