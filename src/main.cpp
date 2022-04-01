#include <Arduino.h>
#include <SPI.h>
#include <UIPEthernet.h>
#include <PubSubClient.h>

byte mac[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05}; //Ponemos la dirección MAC de la Ethernet Shield
IPAddress ip(192, 168, 1, 45);
IPAddress server(192, 168, 1, 30);

void callback(char *topic, byte *payload, unsigned int length)
{
  // handle message arrived
}

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);
//PubSubClient mqttClient(server, 1883, callback, ethClient);

void setup() {
  // setup ethernet communication using DHCP
  Ethernet.begin(mac, ip);
  mqttClient.setServer(server, 1883);
  String clientIdEsp = "stm32Client-";
  clientIdEsp += String(random(0xffff), HEX);

  if (mqttClient.connect(clientIdEsp.c_str()))
  {
    mqttClient.publish("outTopic", "hello world");
  }
}
 
void reconnect()
{
  // Loop until we're reconnected
  while (!mqttClient.connected())
  {
    // Create a random client ID
    String clientIdEsp = "ESP32Client-";
    clientIdEsp += String(random(0xffff), HEX);
    // Attempt to connect  con contraseña y usuario
    if (mqttClient.connect(clientIdEsp.c_str()))
    {
      mqttClient.subscribe("iot/in");
    }
  }
}

void loop() {
  if (!mqttClient.connected())
    {
      reconnect();
    }
  mqttClient.publish("outTopic", "hello world");
  delay(500);
  mqttClient.loop();
}

