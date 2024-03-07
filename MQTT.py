# python 3.8

import random
import time

from paho.mqtt import client as mqtt_client


broker = 'broker.emqx.io'
port = 1883
topic = "test"
# generate client ID with pub prefix randomly
client_id = f'python-mqtt-{random.randint(0, 1000)}'
username = 'emqx'
password = "public"


def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(client_id)
    # client.tls_set(ca_certs='./server-ca.crt')
    client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    return client


client = connect_mqtt()
client.loop_start()
def publish(msg):
    if msg == "open" :
        msg = 1
    elif msg == "close":
        msg = 2
    else:
        msg = 0
    result = client.publish(topic, msg)
    time.sleep(1)
    status = result[0]
    if status == 0:
        return True
    else:
        return False

if __name__ == '__main__':
    publish("open")