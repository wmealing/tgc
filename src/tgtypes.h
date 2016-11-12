#include <stdint.h>

/* Declarations  */
typedef struct User_s User_s;

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

