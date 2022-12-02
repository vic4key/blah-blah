# https://core.telegram.org/
# https://www.shellhacks.com/telegram-api-send-message-personal-notification-bot/

# notes:
# dump json object by forward a chat message to `@JsonDumpBot`
# the channel id is '-100{<id>-from-url}' # `https://web.telegram.org/k/<id>`
# https://gist.github.com/mraaroncruz/e76d19f7d61d59419002db54030ebe35

''' to create a telegram bot and get its token
1. find `BotFather`
2. start to talk with `/newbot`
3. follow the bots instruction to get it done
'''

''' to get the chat id
1. send a message to the bot
2. tun the command `curl https://api.telegram.org/bot<bot_token>/getUpdates`
3. the chat id can be found in the response
'''

channel_name = "@vic4key_test_channel"
channel_id = "-1001642803959" # https://web.telegram.org/k/-1642803959

bot_name = "@da_Poet_BOT"
bot_token = ""
target_id = "1809685164" # the id of user that bot is chatting with (eg. 'vic4key')

# pip install python-telegram-bot
# 
# from telegram import Bot
# bot = Bot(bot_token)
# bot.send_message(target_id, "halo")

def send_to_telegram(message, chat_id=target_id, access_token=bot_token):
    import requests
    requests.packages.urllib3.disable_warnings()
    try:
        response = requests.post(
            f'https://api.telegram.org/bot{access_token}/sendMessage',
            json={'chat_id': chat_id, 'text': message, 'parse_mode': 'html'},
            verify=False,
        )
        print(response.text)
    except Exception as e: print(e)

send_to_telegram("<b>halo</b> <i>halo</i> <u>halo</u>")