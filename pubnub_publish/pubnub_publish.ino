/*********************************************************************************
PUBNUB PUBLISH DATA USING REST API
*********************************************************************************/
#include <ESP8266WiFi.h>
#include "settings.h"
unsigned long g_startMillis;
unsigned long g_switchTimeMillis;
boolean       g_heaterInHighPhase;

/*********************************************************************************
Function Name     : setup
Description       : Initialize the Serial Communication with baud 115200, connect 
                    to the Router with SSID and PASSWORD 
Parameters        : void
Return            : void
*********************************************************************************/

void setup(void){
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_AP);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/*********************************************************************************
Function Name     : loop
Description       : Publish the data to the hello_world channel using the 
                    GET Request 
Parameters        : void
Return            : void
*********************************************************************************/
void loop(void){

  WiFiClient client;
  const int l_httpPort = 80;
  if (!client.connect(g_host, l_httpPort)) {
    Serial.println("Pubnub Connection Failed");
    return;
  }

//DATA FORMAT : http://pubsub.pubnub.com /publish/pub-key/sub-key/signature/channel/callback/message

  String url = "/publish/";
  url += g_pubKey;
  url += "/";
  url += g_subKey;
  url += "/0/";
  url += g_channel;
  url += "/0/";
  url += "\"hello_world\"";
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + g_host + "\r\n" + 
             "Connection: close\r\n\r\n");
  delay(10);
  
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("Pubnub Connection Closed");
}

//End of the Program
