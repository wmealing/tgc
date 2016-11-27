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
    /* Error allocating mem */
    TG_ALLOCFAIL
} tgcode;

typedef struct tg_res {
    tgcode ok;
    int error_code; 
    char description[100];
    json_error_t json_err;
} tg_res;

/*
 * Initialize the library with a token
 * Must be done before utilizing the library
 */

_Bool tg_init (char *api_token);

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

User_s getMe (tg_res *res);

/*
 * Makes a getUpdates request.
 * Use this method to receive incoming updates using long polling.
 * An Array of Update objects is returned.
 * https://core.telegram.org/bots/api#getupdates
 */

Update_s *getUpdates (long long offset, size_t *limit, int timeout, tg_res *res);
