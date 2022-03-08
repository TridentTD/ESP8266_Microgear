/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>

#define SSID        "esp8266"
#define PASSWORD    "esp8266"
#define APPID       "esp8266"
#define KEY         "esp8266"
#define SECRET      "esp8266"
#define ALIAS       "esp8266"

WiFiClient client;
MicroGear microgear(client);
int timer = 0;

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

  /* Initial with KEY, SECRET and also set the ALIAS here */
  microgear.init(KEY, SECRET, ALIAS);

  /* connect to NETPIE to a specific APPID */
  microgear.connect(APPID);
}

void loop() {
  /* To check if the microgear is still connected */
  if (microgear.connected()) {
    Serial.println("connected");

    /* Call this method regularly otherwise the connection may be lost */
    microgear.loop();

    if (timer >= 1000) {
      Serial.println("Publish...");

      /* Chat with the microgear named ALIAS which is myself */
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
