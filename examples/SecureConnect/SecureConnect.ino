/*  NETPIE ESP8266 secure microgear connection sample                  */
/*  It differs from the normal connection by 2 stpes                   */
/*     1. Declare a client as WiFiClientSecure instead of WiFiClient.  */
/*     2. Call microgear.useTLS(true) before initial                   */
/*  More information visit : https://netpie.io                         */

#include <ESP8266WiFi.h>
#include <MicroGear.h>

#define SSID        <WIFI_SSID>
#define PASSWORD    <WIFI_PASSWORD>
#define APPID       <APPID>
#define KEY         <APPKEY>
#define SECRET      <APPSECRET>
#define ALIAS       "esp8266"

 /*  1. Declare a client as WiFiClientSecure instead of WiFiClient. */
WiFiClientSecure client;
MicroGear microgear(client);
int timer = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  if (WiFi.begin(SSID, PASSWORD)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* Call onMsghandler() when new message arraives */
  microgear.on(MESSAGE, [](char *topic, uint8_t* data, unsigned int len) {
    Serial.print("Incoming message --> ");
    char msg[len+1]; snprintf(msg, sizeof(msg), "%*s", len, data);
    Serial.println(msg);
  });

  /* Call onFoundgear() when new gear appear */
  microgear.on(PRESENT, [](char *attribute, uint8_t* data, unsigned int len) {
    Serial.print("Found new member --> ");
    char msg[len+1]; snprintf(msg, sizeof(msg), "%*s", len, data);
    Serial.println(msg);
  });

  /* Call onLostgear() when some gear goes offline */
  microgear.on(ABSENT, [](char *attribute, uint8_t* data, unsigned int len) {
    Serial.print("Lost member --> ");
    char msg[len+1]; snprintf(msg, sizeof(msg), "%*s", len, data);
    Serial.println(msg);
  });

  /* Call onConnected() when NETPIE connection is established */
  microgear.on(CONNECTED, [](char *attribute, uint8_t* data, unsigned int len) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setAlias(ALIAS);
  });

  /* 2. Call microgear.useTLS(true) before initial */
  microgear.useTLS(true);
  microgear.init(KEY, SECRET, ALIAS);
  microgear.connect(APPID);
}

void loop() {
  if (microgear.connected()) {
    Serial.println("connected");
    microgear.loop();
    if (timer >= 1000) {
      Serial.println("Publish...");
      microgear.chat(ALIAS, "Hello");
      timer = 0;
    }
    else timer += 100;
  }
  else {
    Serial.println("connection lost, reconnect...");
    if (timer >= 5000) {
      microgear.connect(APPID);
      timer = 0;
    }
    else timer += 100;
  }
  delay(100);
}