#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <Ticker.h>

//定义舵机
Servo myservo1;
Servo myservo2;

// WiFi settings
const char *ssid = "WiFi name"; // Enter your WiFi name
const char *password = "password";  // Enter WiFi password

// MQTT Broker settings
//const char *mqtt_broker = "broker-cn.emqx.io"; // EMQX broker endpoint
const char *mqtt_broker = "broker.emqx.io"; // EMQX broker ensdpoint
const char *mqtt_topic = "test/status";     // MQTT topic
const char *mqtt_topic_replay = "test/reply";     // MQTT topic
const char *mqtt_username = "emqx";  // MQTT username for authentication
const char *mqtt_password = "public";  // MQTT password for authentication
const int mqtt_port = 1883;  // MQTT port (TCP)

WiFiClient espClient;
PubSubClient mqtt_client(espClient);
Ticker timer;

void timer_keep();

void connectToWiFi();

void connectToMQTTBroker();

void mqttCallback(char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    myservo1.attach(2);//舵机的IO口，nodemcu的D4口
    myservo1.write(90);//上电时舵机归零垂直
    myservo2.attach(14);
    myservo2.write(90);//上电时舵机归零垂直
    connectToWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
    connectToMQTTBroker();
    timer.attach(5, timer_keep);
}

void timer_keep() {
    mqtt_client.publish(mqtt_topic, "connected");
    Serial.println("topic pub");
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to the WiFi network");
}

void connectToMQTTBroker() {
    while (!mqtt_client.connected()) {
        String client_id = "esp8266-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
        if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            mqtt_client.subscribe(mqtt_topic);
            // Publish message upon successful connection
            mqtt_client.publish(mqtt_topic, "Hi EMQX I'm ESP8266 ^^");
        } else {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    String state = "0";
    state[0] = (char) payload[0];
    Serial.println("-----------------------");
    //打开1号舵机
    if (state == "1") {
        myservo1.write(60);
        delay(500);//延时
        myservo1.write(160);
        mqtt_client.publish(mqtt_topic_replay, "servo1 has opened");
        delay(500);//延时
        myservo1.write(90);
        //关闭1号舵机
    } else if (state == "2") {
        myservo1.write(120);
        delay(500);//延时
        myservo1.write(20);
        mqtt_client.publish(mqtt_topic_replay, "servo1 has closed");
        delay(500);//延时
        myservo1.write(90);
        //打开2号舵机
    } else if (state == "3") {
        myservo2.write(120);
        delay(500);//延时
        myservo2.write(20);
        mqtt_client.publish(mqtt_topic_replay, "servo2 has opened");
        delay(500);//延时
        myservo2.write(90);
        //关闭2号舵机
    } else if (state == "4") {
        myservo2.write(60);
        delay(500);//延时
        myservo2.write(160);
        mqtt_client.publish(mqtt_topic_replay, "servo2 has closed");
        delay(500);//延时
        myservo2.write(90);
        //打开12号舵机
    } else if (state == "5") {
        myservo1.write(60);
        delay(500);//延时
        myservo1.write(160);
        mqtt_client.publish(mqtt_topic_replay, "servo1 and servo2 have opened");
        delay(500);//延时
        myservo1.write(90);
        myservo2.write(120);
        delay(500);//延时
        myservo2.write(20);
        delay(500);//延时
        myservo2.write(90);
        //关闭12号舵机
    } else if (state == "6") {
        myservo1.write(120);
        delay(500);//延时
        myservo1.write(20);
        mqtt_client.publish(mqtt_topic_replay, "servo1 and servo2 have closed");
        delay(500);//延时
        myservo1.write(90);
        myservo2.write(60);
        delay(500);//延时
        myservo2.write(160);
        delay(500);//延时
        myservo2.write(90);
    }
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        connectToWiFi();
    }
    if (!mqtt_client.connected()) {
        connectToMQTTBroker();
    }
    mqtt_client.loop();
}
