from nonebot import on_keyword
from nonebot.adapters.onebot.v11 import Event
from nonebot import logger
from .MQTT import publish
light = on_keyword(['开灯','关灯',"open","close"],priority=50)
@light.handle()
async def handle_function(event: Event):
    msg = str(event.get_message()).strip()
    logger.info(f"消息：{msg}")
    if msg == "开灯" or msg == "open" :
        if publish("open"):
            await light.finish(f"已开灯")
        else:
            await light.finish(f"发生错误")
    elif msg == "关灯" or msg == "close" :
        if publish("close"):
            await light.finish(f"已关灯")
        else:
            await light.finish(f"Cuow")