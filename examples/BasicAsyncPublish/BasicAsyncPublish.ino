/*  NETPIE ESP8266 basic asynchronous publish sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <Ticker.h>

#define SSID        <WIFI_SSID>
#define PASSWORD    <WIFI_PASSWORD>
#define APPID       <APPID>
#define KEY         <APPKEY>
#define SECRET      <APPSECRET>
#define ALIAS       "esp8266"


WiFiClient client;
MicroGear microgear(client);
Ticker    ticker;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
  /* You may want to use other method that is more complicated, but provide better user experience */
  if (WiFi.begin(SSID, PASSWORD)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connecting to NETPIE.io");

  /* Call onMsghandler() when new message arraives */
  microgear.on(MESSAGE, [](char *topic, uint8_t* data, unsigned int len) {
    Serial.print("Incoming message --> ");
    char msg[len+1]; snprintf(msg, sizeof(msg), "%*s", len, data);
    /* print message */
    Serial.printf("Incoming message --> %s  at [%lu] \n", msg, millis());
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

  /* Initial with KEY, SECRET and also set the ALIAS here */
  microgear.init(KEY, SECRET, ALIAS);

  /* connect to NETPIE to a specific APPID */
  microgear.connect(APPID);

  ticker.attach_ms(2000,[](){
    if(microgear.connected()) {
      Serial.printf("Publish at [%lu]\n", millis());
      microgear.chat(ALIAS, "HELLO");
    }else{
      Serial.println("connection lost, reconnect...");
      microgear.connect(APPID);    
    }
  });

}

void loop() {
  microgear.loop();
}

