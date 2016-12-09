#include <stdint.h>
#include <jansson.h>

/**
 * @file
 * @brief Headfile containing Telegram types.
 */

/**
 * @defgroup group3 Telegram Types
 * @brief Telegram types.
 *
 * Follows the naming convention available
 * <a href = "https://core.telegram.org/bots/api/#available-types">here</a>
 * with an added _s. All the structures are typedefined.
 * @{
 */

//! Typedef of Update type
typedef struct Update_s Update_s;

//! Typedef of User type
typedef struct User_s User_s;
//! Typedef of Message type
typedef struct Message_s Message_s;
//! Typedef of Chat type
typedef struct Chat_s Chat_s;
//! Typedef of MessageEntity type
typedef struct MessageEntity_s MessageEntity_s;
//! Typedef of PhotoSize type
typedef struct PhotoSize_s PhotoSize_s;
//! Typedef of Audio type
typedef struct Audio_s Audio_s;
//! Typedef of Document type
typedef struct Document_s Document_s;
//! Typedef of Sticker type
typedef struct Sticker_s Sticker_s;
//! Typedef of Video type
typedef struct Video_s Video_s;
//! Typedef of Voice type
typedef struct Voice_s Voice_s;
//! Typedef of Contact type
typedef struct Contact_s Contact_s;
//! Typedef of Location type
typedef struct Location_s Location_s;
//! Typedef of Venue type
typedef struct Venue_s Venue_s;
//! Typedef of UserProfilePhotos type
typedef struct UserProfilePhotos_s UserProfilePhotos_s;
//! Typedef of File type
typedef struct File_s File_s;

//! Typedef of Game type
typedef struct Game_s Game_s;
//! Typedef of Animation type
typedef struct Animation_s Animation_s;

/**
 * @brief Update type
 *
 * See <a href="https://core.telegram.org/bots/api/#update">here</a>
 * for more information.
 */
struct Update_s
{
    //! The update‘s unique identifier
    json_int_t *update_id;
    //! Optional. New incoming message of any kind — text, photo, sticker, etc.
    Message_s *message;
    //! Optional. New version of a message that is known to the bot and was edited
    Message_s *edited_message;
    //! Optional. New incoming channel post of any kind — text, photo, sticker, etc.
    Message_s *channel_post;
    //! Optional. New version of a channel post that is known to the bot and was edited
    Message_s *edited_channel_post;
};

/**
 * @brief User type
 *
 * See <a href="https://core.telegram.org/bots/api/#user">here</a>
 * for more information.
 */
struct User_s
{
    //! Unique identifier for this user or bot
    json_int_t *id;
    //! User‘s or bot’s first name
    char *first_name;
    //! Optional. User‘s or bot’s last name
    char *last_name;
    //! Optional. User‘s or bot’s username
    char *username;
};

/**
 * @brief Chat type
 *
 * See <a href="https://core.telegram.org/bots/api/#chat">here</a>
 * for more information.
 */
struct Chat_s
{
    //! Unique identifier for this chat
    json_int_t *id;
    //! Type of chat, can be either “private”, “group”, “supergroup” or “channel”
    char *type;
    //! Optional. Title, for supergroups, channels and group chats
    char *title;
    //! Optional. Username, for private chats, supergroups and channels if available
    char *username;
    //! Optional. First name of the other party in a private chat
    char *first_name;
    //! Optional. Last name of the other party in a private chat
    char *last_name;
    //! Optional. True if a group has ‘All Members Are Admins’ enabled
    _Bool *all_members_are_administrators;
};

/**
 * @brief Message type
 *
 * See <a href="https://core.telegram.org/bots/api#message">here</a>
 * for more information.
 */
struct Message_s
{
    //! Unique message identifier inside this chat
    json_int_t *message_id;
    //! Optional. Sender, can be empty for messages sent to channels
    User_s *from;
    //! Date the message was sent in Unix time
    json_int_t *date;
    //! Conversation the message belongs to
    Chat_s *chat;
    //! Optional. For forwarded messages, sender of the original message
    User_s *forward_from;
    //! Optional. For messages forwarded from a channel, information about the original channel
    Chat_s *forward_from_chat;
    //! Optional. For forwarded channel posts, identifier of the original message in the channel
    json_int_t *forward_from_message_id;
    //! Optional. For forwarded messages, date the original message was sent in Unix time
    json_int_t *forward_date;
    //! Optional. For replies, the original message. 
    Message_s *reply_to_message;
    //! Optional. Date the message was last edited in Unix time
    json_int_t *edit_date;
    //! Optional. For text messages, the actual UTF-8 text of the message.
    char *text;
    //! Optional. For text messages, special entities that appear in the text 
    MessageEntity_s *entities;
    //! Length of the MessageEntity_s array
    size_t entities_len;
    //! Optional. Message is an audio file, information about the file
    Audio_s *audio;
    //! Optional. Message is a general file, information about the file
    Document_s *document;
    //! Optional. Message is a game, information about the game.
    Game_s *game;
    //! Optional. Message is a photo, available sizes of the photo
    PhotoSize_s *photo;
    //! Length of the PhotoSize_s array
    size_t photo_len;
    //! Optional. Message is a sticker, information about the sticker
    Sticker_s *sticker;
    //! Optional. Message is a video, information about the video
    Video_s *video;
    //! Optional. Message is a voice message, information about the file
    Voice_s *voice;
    //! Optional. Caption for the document, photo or video, 0-200 characters
    char *caption;
    //! Optional. Message is a shared contact, information about the contact
    Contact_s *contact;
    //! Optional. Message is a shared location, information about the location
    Location_s *location;
    //! Optional. Message is a venue, information about the venue
    Venue_s *venue;
    //! Optional. A new member was added to the group, information about them
    User_s *new_chat_member;
    //! Optional. A member was removed from the group, information about them
    User_s *left_chat_member;
    //! Optional. A chat title was changed to this value
    char *new_chat_title;
    //! Optional. A chat photo was change to this value
    PhotoSize_s *new_chat_photo;
    //! Length of the PhotoSize_s array
    size_t new_chat_photo_len;
    //! Optional. Service message: the chat photo was deleted
    _Bool *delete_chat_photo;
    //! Optional. Service message: the group has been created
    _Bool *group_chat_created;
    //! Optional. Service message: the supergroup has been created
    _Bool *supergroup_chat_created;
    //! Optional. Service message: the channel has been created
    _Bool *channel_chat_created;
    //! Optional. The group has been migrated to a supergroup with the specified identifier
    json_int_t *migrate_to_chat_id;
    //! Optional. The supergroup has been migrated from a group with the specified identifier
    json_int_t *migrate_from_chat_id;
    //! Optional. Specified message was pinned
    Message_s *pinned_message;
};

/**
 * @brief MessageEntity type
 *
 * See <a href="https://core.telegram.org/bots/api#messageentity">here</a>
 * for more information.
 */
struct MessageEntity_s
{
    //! Type of the entity
    char *type;
    //! Offset in UTF-16 code units to the start of the entity
    json_int_t *offset;
    //! Length of the entity in UTF-16 code units
    json_int_t *length;
    //! Optional. For “text_link” only, url that will be opened after user taps on the text
    char *url;
    //! Optional. For “text_mention” only, the mentioned user
    User_s *user;
};

/**
 * @brief PhotoSize type
 *
 * See <a href="https://core.telegram.org/bots/api#photosize">here</a>
 * for more information.
 */
struct PhotoSize_s
{
    //! Unique identifier for this file
    char *file_id;
    //! Photo width
    json_int_t *width;
    //! Photo height
    json_int_t *height;
    //! Optional. File size
    json_int_t *file_size;
};

/**
 * @brief Audio type
 *
 * See <a href="https://core.telegram.org/bots/api#audio">here</a>
 * for more information.
 */
struct Audio_s
{
    //! Unique identifier for this file
    char *file_id;
    //! Duration of the audio in seconds as defined by sender
    json_int_t *duration;
    //! Optional. Performer of the audio as defined by sender or by audio tags
    char *performer;
    //! Optional. Title of the audio as defined by sender or by audio tags
    char *title;
    //! Optional. MIME type of the file as defined by sender
    char *mime_type;
    //! Optional. File size
    json_int_t *file_size;
};

/**
 * @brief Document type
 *
 * See <a href="https://core.telegram.org/bots/api#document">here</a>
 * for more information.
 */
struct Document_s
{
    //! Unique file identifier
    char *file_id;
    //! Optional. Document thumbnail as defined by sender
    PhotoSize_s *thumb;
    //! Optional. Original filename as defined by sender
    char *file_name;
    //! Optional. MIME type of the file as defined by sender
    char *mime_type;
    //! Optional. File size
    json_int_t *file_size;
};

/**
 * @brief Sticker Type
 *
 * See <a href="https://core.telegram.org/bots/api#sticker">here</a>
 * for more information.
 */
struct Sticker_s
{
    //! Unique identifier for this file
    char *file_id;
    //! Sticker width
    json_int_t *width;
    //! Sticker height
    json_int_t *height;
    //! Optional. Sticker thumbnail in .webp or .jpg format
    PhotoSize_s *thumb;
    //! Optional. Emoji associated with the sticker
    char *emoji;
    //! Optional. File size
    json_int_t *file_size;
};

/**
 * @brief Video type
 *
 * See <a href="https://core.telegram.org/bots/api#video">here</a>
 * for more information.
 */
struct Video_s
{
    //! Unique identifier for this file
    char *file_id;
    //! Video width as defined by sender
    json_int_t *width;
    //! Video height as defined by sender
    json_int_t *height;
    //! Duration of the video in seconds as defined by sender
    json_int_t *duration;
    //! Optional. Video thumbnail
    PhotoSize_s *thumb;
    //! Optional. Mime type of a file as defined by sender
    char *mime_type;
    //! Optional. File size
    json_int_t *file_size;
};

/**
 * @brief Voice type
 *
 * See <a href="https://core.telegram.org/bots/api#voice">here</a>
 * for more information
 */
struct Voice_s
{
    //! Unique identifier for this file
    char *file_id;
    //! Duration of the audio in seconds as defined by sender
    json_int_t *duration;
    //! Optional. MIME type of the file as defined by sender
    char *mime_type;
    //! Optional. File size
    json_int_t *file_size;
};

/**
 * @brief Contact type
 *
 * See <a href="https://core.telegram.org/bots/api#contact">here</a>
 * for more information.
 */
struct Contact_s
{
    //! Contact's phone number
    char *phone_number;
    //! Contact's first name
    char *first_name;
    //! Optional. Contact's last name
    char *last_name;
    //! Optional. Contact's user identifier in Telegram
    json_int_t *user_id;
};

/**
 * @brief Location type
 *
 * See <a href="https://core.telegram.org/bots/api#location">here</a>
 * for more information.
 */
struct Location_s
{
    //! Longitude as defined by sender
    double *longitude;
    //! Latitude as defined by sender
    double *latitude;
};

/**
 * @brief Venue type
 *
 * See <a href="https://core.telegram.org/bots/api#venue">here</a>
 * for more information.
 */
struct Venue_s
{
    //! Venue location
    Location_s *location;
    //! Name of the venue
    char *title;
    //! Address of the venue
    char *address;
    //! Optional. Foursquare identifier of the venue
    char *foursquare_id;
};

/**
 * @brief UserProfilePhoto type
 *
 * See <a href="https://core.telegram.org/bots/api#userprofilephotos">here</a>
 * for more information.
 */
struct UserProfilePhotos_s
{
    //! Total number of profile pictures the target user has
    json_int_t *total_count;
    //! Requested profile pictures (in up to 4 sizes each)
    PhotoSize_s *photos;
    //! Length of the PhotoSize_s array
    size_t photos_len;
};

/**
 * @brief File type
 *
 * See <a href="https://core.telegram.org/bots/api#file">here</a>
 * for more information.
 */
struct File_s
{
    //! Unique identifier for this file
    char *file_id;
    //! Optional. File size, if known
    json_int_t *file_size;
    //! Optional. File path.
    char *file_path;
};

/**
 * @brief ReplyKeyboardMarkup type
 *
 * See <a href="https://core.telegram.org/bots/api#game">here</a>
 * for more information.
 */
struct Game_s
{
    //! Title of the game
    char *title;
    //! Description of the game
    char *description;
    //! Photo that will be displayed in the game message in chats.
    PhotoSize_s *photo;
    //! Length of the PhotoSize_s array
    size_t photo_len;
    //! Optional. Brief description of the game or high scores included in the game message
    char *text;
    //! Optional. Special entities that appear in text
    MessageEntity_s *text_entities;
    //! Length of the MessageEntity_s array
    size_t text_entities_len;
    //! Optional. Animation that will be displayed in the game message in chats.
    Animation_s *animation;
};

/**
 * @brief Animation type
 *
 * See <a href="https://core.telegram.org/bots/api#animation">here</a>
 * for more information.
 */
struct Animation_s
{
    //! Unique file identifier
    char *file_id;
    //! Optional. Animation thumbnail as defined by sender
    PhotoSize_s *thumb;
    //! Optional. Original animation filename as defined by sender
    char *file_name;
    //! Optional. MIME type of the file as defined by sender
    char *mime_type;
    //! Optional. File size
    json_int_t *file_size;
};

/**@}*/

