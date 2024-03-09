from nonebot import on_keyword
from nonebot.adapters.onebot.v11 import Event
from nonebot import logger
from .MQTT import publish
light = on_keyword(['开灯','关灯','开灯1','开灯2','开灯12','关灯1','关灯2','关灯12'],priority=50)
@light.handle()
async def handle_function(event: Event):
    msg = str(event.get_message()).strip()
    logger.info(f"消息：{msg}")
    if msg == "开灯1":
        await light.send(f"已发送开灯请求")
        result = publish(1)
        if result:
            await light.finish(result)
        else:
            await light.finish(f"控制板已失联")
    elif msg == "关灯1":
        await light.send(f"已发送关灯请求")
        result = publish(2)
        if result:
            await light.finish(result)
        else:
            await light.finish(f"控制板已失联")
    elif msg == "开灯2":
        await light.send(f"已发送开灯请求")
        result = publish(3)
        if result:
            await light.finish(result)
        else:
            await light.finish(f"控制板已失联")
    elif msg == "关灯2":
        await light.send(f"已发送关灯请求")
        result = publish(4)
        if result:
            await light.finish(result)
        else:
            await light.finish(f"控制板已失联")
    elif msg == "开灯12" or msg == "开灯":
        await light.send(f"已发送开灯请求")
        result = publish(5)
        if result:
            await light.finish(result)
        else:
            await light.finish(f"控制板已失联")
    elif msg == "关灯12" or msg == "关灯":
        await light.send(f"已发送关灯请求")
        result = publish(6)
        if result:
            await light.finish(result)
        else:
            await light.finish(f"控制板已失联")
