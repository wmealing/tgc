#include <stdint.h>

/* Declarations  */
typedef struct User_s User_s;
typedef struct Chat_s Chat_s;
typedef struct MessageEntity_s MessageEntity_s;
typedef struct PhotoSize_s PhotoSize_s;

/*
 * User
 * This object represents a Telegram user or bot.
 * https://core.telegram.org/bots/api/#user
 */

struct User_s
{
    _Bool ok;
    int error_code;
    char *description;
    uint64_t id;
    char *first_name;
    char *last_name;
    char *username;
};

struct Chat_s
{
    uint64_t id;
    char *type;
    char *title;
    char *username;
    char *first_name;
    char *last_name;
    _Bool all_members_are_administrators;
};

struct MessageEntity_s
{
    char *type;
    unsigned int offset;
    unsigned int length;
    char *url;
    User_s *user;
};

struct PhotoSize_s
{
    char *file_id;
    unsigned int width;
    unsigned int height;
    unsigned int file_size;
};

