#include "tgtypes.h"
#include "tgparse.h"

/**
 * @file
 * @brief Contains the error object and library initialization/Telegram methods.
 *
 * See <a href="https://core.telegram.org/bots/api#available-methods">here</a> for
 * more information regarding the methods.
 */

/**
 * @defgroup group1 Library types and methods
 * @brief Functions/types to manage the library and errors.
 * @{
 */

/**
 * @brief All errors you'll encounter using this library.
 * @see tg_res
 */
typedef enum tgcode
{
    //! No errors encountered.
    TG_OKAY,
    //! Telegram return ok: False.
    /*! Check tg_res.description for more information */
    TG_NOTOKAY,
    //! The token is invalid.
    TG_TOKENFAIL,
    //! LibCurl encountered an error.
    /*! Check tg_res.error_code for more information */
    TG_CURLFAIL,
    //! Jansson encounted an error.
    /*! Check tg_res.json_err for more information */
    TG_JSONFAIL,
    //! Failed to allocate memory (OOM).
    TG_ALLOCFAIL
} tgcode;

/**
 * @brief Library error object.
 * @see tgcode
 *
 * Pass around this object when using the libraries various functions
 * for information on errors. If tg_res.ok is #TG_OKAY there is no need to check
 * any other member of the struct.
 */
typedef struct tg_res
{
    //! tgcode value indicating any errors.
    tgcode ok;
    //! Stores the Telegram/Curl status codes on error.
    int error_code;
    //! Stores the Telegram error description on error.
    char description[100];
    //! Stores any Jansson errors.
    /*! If empty this probably indicates the library ran out of memory. */
    json_error_t json_err;
} tg_res;

/**
 * @brief Initialize the library.
 * @see tg_cleanup
 *
 * @param api_token Your bots authorization token. Learn more about obtaining one
 * <a href="https://core.telegram.org/bots#botfather">here</a>.
 * @param res A tg_res object. Check this for more details in case initialization fails.
 *
 * @return Returns 0 on success on 1 on failure.
 */
_Bool tg_init (const char *api_token, tg_res *res);

/**
 * @brief Cleans up the library.
 * @see tg_init
 *
 * This does not clean up curl so you will need to use curl_global_cleanup if your
 * program continues to run.
 */
void tg_cleanup (void);
/**@}
 * @defgroup group2 Telegram Methods
 * @brief Methods to interact with the Telegram bot api.
 * 
 * See <a href="https://core.telegram.org/bots/api#available-methods">here</a> for
 * more information regarding the methods.
 *
 * @{
 */

/**
 * @brief getMe
 * @see User_free
 * 
 * Returns basic information about the bot in form of a User object. See
 * <a href="https://core.telegram.org/bots/api#getme">here</a> for more information.
 *
 * @param res Error object.
 *
 * @returns A User_s object. Will be empty in the case of failure (Check tg_res).
 * Use User_free to cleanup the returned object.
 */
User_s getMe (tg_res *res);

/**
 * @brief getUpdates
 * @see Update_free
 *
 * Poll Telegram servers for new incoming updates. 
 * See <a href="https://core.telegram.org/bots/api#getupdates">here</a> for more information.
 *
 * @param offset Identifier of the first update to be returned. You will be responsible
 * for handling this.
 * @param limit Number of updates you want to retrieve. This number will be modified to reflect
 * the actual amount retrieved.
 * @param timeout Timeout for long polling. Set this to a high number.
 * @param res Error object.
 *
 * @returns An Update_s object. Check the \p limit param for the length of the result.
 * Use Update_free afterwards to cleanup the returned object.
 */
Update_s *getUpdates (const long long offset, size_t *limit, const int timeout, tg_res *res);

/**
 * @brief sendMessage
 * @see Message_free
 *
 * Use this function to send a message to a chat or channel. See 
 * <a href="https://core.telegram.org/bots/api#sendmessage">here</a> for more information.
 *
 * @param chat_id String identifier of the target chat or channel.
 * @param text Text of the message you're sending.
 * @param parse_mode Parsing mode for the text. Pass NULL, "Markdown", or "HTML".
 * @param disable_web_page_preview Disables link previews on a true value.
 * @param disable_notification Sends a silent message on a true value.
 * @param reply_to_message_id Replies to a previously sent message. Set to 0 to not reply.
 * @param reply_markup Json object containing any reply markup you wish to send.
 * @param res Error object.
 *
 * @returns A filled in Message_s object on success. Use Message_free afterwards to cleanup.
 */
Message_s sendMessage (const char *chat_id, const char *text, const char *parse_mode,
        const _Bool disable_web_page_preview, const _Bool disable_notification,
        const long long reply_to_message_id, json_t *reply_markup, tg_res *res);

/**
 * @brief forwardMessage
 * @see Message_free
 *
 * Use this function to forward a message. See 
 * <a href="https://core.telegram.org/bots/api#forwardmessage">here</a> for more information.
 * 
 * @param chat_id String identifier of the target chat or channel.
 * @param from_chat_id Originating chat/channel of the message.
 * @param disable_notification Sends a silent message on a true value.
 * @param message_id Identifier of the message to forward.
 * @param res Error object.
 *
 * @returns A filled in Message_s object on success. Use Message_free afterwards to cleanup.
 */
Message_s forwardMessage (const char *chat_id, const char *from_chat_id,
        const _Bool disable_notification, const long long message_id, tg_res *res);

/**@}*/

