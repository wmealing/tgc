#include "tgtypes.h"
#include "tgparse.h"

// Library error codes.
typedef enum tgcode
{
    // Success
    TG_OKAY,
    // Telegram returned ok:false. Check the description and error_code.
    TG_NOTOKAY,
    // Invalid token given to tg_init().
    TG_TOKENFAIL,
    // Curl failed. Check the error_code.
    TG_CURLFAIL,
    // Json error. Check json_err.
    TG_JSONFAIL,
    // Error allocating memory.
    TG_ALLOCFAIL
} tgcode;

// Library error object.
typedef struct tg_res {
    tgcode ok;
    int error_code; 
    char description[100];
    json_error_t json_err;
} tg_res;

// Always initialize the library with a token before using.
_Bool tg_init (const char *api_token, tg_res *res);

// Clean up the library when you are done.
// curl_global_cleanup must still be performed.
void tg_cleanup (void);

/*
 * getMe
 * Returns basic information about the bot in form of a User object.
 * Free this with User_free();
 * https://core.telegram.org/bots/api/#getme
 */

User_s getMe (tg_res *res);

/*
 * getUpdates
 * Use this method to receive incoming updates using long polling.
 * An Array of Update objects is returned.
 * Free this with Update_free().
 * https://core.telegram.org/bots/api#getupdates
 */

Update_s *getUpdates (const long long offset, size_t *limit, const int timeout, tg_res *res);

/*
 * sendMessage
 * Use this method to send text messages. 
 * On success, the sent Message is returned.
 * Free this with Message_free().
 * https://core.telegram.org/bots/api#sendmessage
*/

Message_s sendMessage (const char *chat_id, const char *text, const char *parse_mode,
        const _Bool disable_web_page_preview, const _Bool disable_notification,
        const long long reply_to_message_id, tg_res *res);

