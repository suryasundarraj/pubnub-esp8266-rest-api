/*********************************************************************************
Pubnub Subscribe Sample
*********************************************************************************/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

const char* g_ssid       = "ssid";
const char* g_password   = "password";
const char* g_host       = "pubsub.pubnub.com";
const char* g_pubKey     = "demo";
const char* g_subKey     = "demo";
const char* g_channel    = "hello_world";
String      timeToken    = "0";

/*********************************************************************************
Function Name     : setup
Description       : Initialize the Serial Communication with baud 115200, Begin
                    the ESP8266 and connect to the Router and print the ip
Parameters        : void
Return            : void
*********************************************************************************/

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(g_ssid);
  
  WiFi.begin(g_ssid, g_password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
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
Description       : Connect to the PUBNUB REST API with TCP/IP Connection 
                    and Subscribe the sample data from the pubnub
Parameters        : void
Return            : void
*********************************************************************************/

void loop() 
{
  WiFiClient client;
  const int l_httpPort = 80;
  if (!client.connect(g_host, l_httpPort)) 
  {
    Serial.println("connection failed");
    return;
  }
  //DATA FORMAT : http://pubsub.pubnub.com /publish/pub-key/sub-key/signature/channel/callback/message
  //SUB FORMAT :  http://pubsub.pubnub.com /subscribe/sub-key/channel/callback/timetoken

  String url = "/subscribe/";
  url += g_subKey;
  url += "/";
  url += g_channel;
  url += "/0/";
  url += timeToken;
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + g_host + "\r\n" + 
             "Connection: close\r\n\r\n");
  delay(10);

  while(client.available())
  {
    String json;
    String line = client.readStringUntil('\r');
    Serial.print(line);
    if(line.endsWith("]"))
    {
      if(line.length() == 25)
      {
        char pack[26];
        line.toCharArray(pack,26);
        timeToken = pack[6];
        for(int i = 7;i<=22;i++)
        {
          timeToken += pack[i];
        }
      }
      else
      {
        char bufferr[50];
        String data = line;
        data.toCharArray(bufferr,50);
        for(int i = 3;i<=16;i++)
        {
          json += bufferr[i];
        }
        timeToken = bufferr[20];
        for(int i = 21;i<=36;i++)
        {
          timeToken += bufferr[i];
        }
        Serial.println(json);
      }
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& json_parsed = jsonBuffer.parseObject(json);
      if (!json_parsed.success())
      {
        Serial.println("parseObject() failed");
        return;
      }
      String surya = json_parsed["text"];
      Serial.println(surya);
    }
  }
  Serial.println("closing connection");
  delay(5000);
}

//End of the Code
