from twitchio.ext import commands
import os

class Bot(commands.Bot):
    def __init__(self):
        super().__init__(
            irc_token=os.environ['TMI_TOKEN'], 
            client_id=os.environ['CLIENT_ID'], 
            nick=os.environ['BOT_NICK'], 
            prefix=['BOT_PREFIX'], 
            initial_channels=[os.environ['CHANNEL']
            )

    @commands.command(name='ping')
    async def my_command(self, ctx):
        await ctx.send(f'pong {ctx.author.name}!')

bot = Bot()
bot.run()