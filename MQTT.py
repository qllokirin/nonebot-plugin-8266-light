# python 3.8

import random
import time
from paho.mqtt import client as mqtt_client


broker = 'broker.emqx.io'
port = 1883
topic = "test/status"
topic_reply = "test/reply"
client_id = f'python-mqtt-{random.randint(0, 1000)}'
username = 'emqx'
password = "public"
reply_msg = ""

def connect_mqtt():
    global reply_msg
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
            client.subscribe(topic_reply)
        else:
            print("Failed to connect, return code %d\n", rc)

    def on_message(client, userdata, msg):
        global reply_msg
        print(f"Received message '{msg.payload.decode()}' on topic '{msg.topic}'")
        reply_msg = msg.payload.decode()

    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(broker, port)
    return client


client = connect_mqtt()
client.loop_start()
def publish(msg):
    global reply_msg
    reply_msg = ""
    client.publish(topic, msg)
    time.sleep(2)
    if reply_msg != "":
        return reply_msg
    else:
        return False

if __name__ == '__main__':
    try:
        while True:
            pass
    except KeyboardInterrupt:
        # 关闭MQTT客户端
        client.loop_stop()
        client.disconnect()