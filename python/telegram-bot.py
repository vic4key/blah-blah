# https://core.telegram.org/
# https://www.shellhacks.com/telegram-api-send-message-personal-notification-bot/
# https://help.nethunt.com/en/articles/6253243-how-to-make-an-api-call-to-the-telegram-channel

# notes:
# dump json object by forward a chat message to `@JsonDumpBot`
# the channel id is '-100{<id>-from-url}' # `https://web.telegram.org/k/<id>` (eg. https://web.telegram.org/k/-1642803959)
# go to channel's settings and add bot as a subscriber/administrator
# https://gist.github.com/mraaroncruz/e76d19f7d61d59419002db54030ebe35

''' to create a telegram bot and get its token
1. find `BotFather`
2. start to talk with `/newbot`
3. follow the bots instruction to get it done
'''

''' to get the user/channel id
1. for user, bot send a message to the user / for channel, after administrator added bot to a channel)
2. run the command `curl https://api.telegram.org/bot<bot_token>/getUpdates`
3. the chat id can be found in the response
'''

import os
import dotenv
dotenv.load_dotenv()

# bot information

bot_name = "@da_Poet_BOT"
bot_token = os.environ.get("TELEGRAM_BOT_TOKEN")

# send message via requests

def send_message_to_telegram(message, chat_id, access_token=bot_token):
    import requests
    requests.packages.urllib3.disable_warnings()
    try:
        response = requests.post(
            f'https://api.telegram.org/bot{access_token}/sendMessage',
            json={'chat_id': chat_id, 'text': message, 'parse_mode': 'html'},
            verify=False,
            timeout=5,
        )
        print(response.status_code, "\n", response.text, "\n")
    except Exception as e: print(e)

# send message via python-telegram-bot
# 
# pip install requests python-telegram-bot

async def main():
    from telegram import Bot
    bot = Bot(bot_token)
    await bot.send_message(chat_id="-1001642803959", text="hello world")

if __name__ == "__main__":
    import asyncio
    asyncio.run(main())

    # user_id = "1809685164" # the user id that bot is chatting with (eg. 'da Poet's BOT')
    # send_message_to_telegram("<b>halo</b> <i>halo</i> <u>halo</u>", chat_id=user_id)

    # channel_id = "-1001642803959" # the channel id that bot is chatting with (eg. 'TEST CHANNEL')
    # channel_id = "-1003718449530" # the channel id that bot is chatting with (eg. 'HayVL AI' <hayvl_app>)
    # send_message_to_telegram("<b>halo</b> <i>halo</i> <u>halo</u>", chat_id=channel_id)
