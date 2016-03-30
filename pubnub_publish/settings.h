#ifndef settings_h
#define settings_h

//Function Prototypes
//void pubnubPublish(char *p_data);
//void prepare_json_data(const char* p_rfidNum);

//Setting up the Linkit to Work with the Local WiFi Router 
#define WIFI_AP       "radio"     // provide your WIFI_AP name
#define WIFI_PASSWORD "radio@123" //provide your WIFI password

//Setting up the PubNub Publish and Subscribe Channels with Channel Name
const char* g_host      = "pubsub.pubnub.com";
const char* g_pubKey    = "demo";
const char* g_subKey    = "demo";
const char* g_channel   = "hello_world";

#endif
