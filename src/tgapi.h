#include "tgtypes.h"
#include "tgparse.h"

/*
 * Library error responses
 */

typedef enum tgcode
{
    /* All is well */
    TG_OKAY,
    /* Telegram returned ok:false */
    TG_NOTOKAY,
    /* Curl Error */
    TG_CURLFAIL,
    /* Json Error */
    TG_JSONFAIL,
    /* Error parsing */
    TG_PARSERFAIL
} tgcode;

/*
 * Initialize the library with a token
 * Must be done before utilizing the library
 */

void tg_init (char *api_token);

/*
 * Cleans up the library.
 * Note: curl_global_cleanup must still be performed
 */

void tg_cleanup (void);

/*
 * getMe
 * Returns basic information about the bot in form of a User object.
 * https://core.telegram.org/bots/api/#getme
 * Pass User_s
 */

tgcode getMe (User_s *api_s);
