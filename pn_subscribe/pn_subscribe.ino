/*********************************************************************************
  Pubnub Subscribe Sample
*********************************************************************************/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

const char* g_ssid       = "radio";
const char* g_password   = "radio@123";
const char* g_host       = "pubsub.pubnub.com";
const char* g_pubKey     = "pub-c-913ab39c-d613-44b3-8622-2e56b8f5ea6d";
const char* g_subKey     = "sub-c-8ad89b4e-a95e-11e5-a65d-02ee2ddab7fe";
const char* g_channel    = "locker_device";
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

  while (client.available())
  {
    String line = client.readStringUntil('\r');
    if (line.endsWith("]"))
    {
      json_handler(string_parser(line));
    }
  }
  Serial.println("closing connection");
  delay(5000);
}

uint8_t json_handler(String p_json) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json_parsed = jsonBuffer.parseObject(p_json);
  if (!json_parsed.success())
  {
    Serial.println("parseObject() failed");
    return;
  }
  String key_parsed = json_parsed["text"];
  Serial.println(key_parsed);
}

String string_parser(String data) {
  char bufferr[200];
  memset(bufferr, '\0', 200);

  int flower_count = 0;
  String json;

  typedef enum {
    STATE_INIT = 0,
    STATE_START_JSON,
    STATE_END_JSON,
    STATE_SKIP_JSON,
    STATE_INIT_TIME_TOKEN,
    STATE_START_TIME_TOKEN,
    STATE_END_TIME_TOKEN
  } STATE;
  STATE state = STATE_INIT;

  data.toCharArray(bufferr, 200);

  for (int i = 1; i <= strlen(bufferr); i++) {
    if (bufferr[i] == '[' && bufferr[i + 2] == '{') {
      state = STATE_START_JSON;
      json = "\0";
      continue;
    }
    else if (bufferr[i] == '[' && bufferr[i + 4] == '"') {
      state = STATE_INIT_TIME_TOKEN;
      json = "{}";
      timeToken = "\0";
      continue;
    }
    switch (state) {
      case STATE_START_JSON:
        if (bufferr[i] == '[') {
          break;
        }
        else if (bufferr[i] == '{') {
          flower_count++;
          json += bufferr[i];
        }
        else if (bufferr[i] == '}') {
          flower_count--;
          json += bufferr[i];
          if (flower_count <= 0) {
            state = STATE_END_JSON;
          }
        }
        else {
          json += bufferr[i];
          if (bufferr[i] == '}') {
            state = STATE_END_JSON;
          }
        }
        break;
      case STATE_END_JSON:
        if (bufferr[i] == ']' && bufferr[i + 2] == '"') {
          state = STATE_INIT_TIME_TOKEN;
          timeToken = "\0";
        }
        break;
      case STATE_INIT_TIME_TOKEN:
        if (bufferr[i] == '"') {
          state = STATE_START_TIME_TOKEN;
          timeToken = "\0";
        }
        break;
      case STATE_START_TIME_TOKEN:
        if (bufferr[i] == '"' && bufferr[i + 1] == ']') {
          state = STATE_END_TIME_TOKEN;
          break;
        }
        timeToken += bufferr[i];
        break;
      case STATE_END_TIME_TOKEN:
        break;
      default:
        break;
    }
  }
  return json;
}

//End of the Code
