#include <stdint.h>

/* Declarations  */
typedef struct User_s User_s;
typedef struct Chat_s Chat_s;

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
    _Bool ok;
    int error_code;
    char *description;
    uint64_t id;
    char *type;
    char *title;
    char *username;
    char *first_name;
    char *last_name;
    _Bool all_members_are_administrators;
};


