#include <UIPEthernet.h>
#include <utility/logging.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <Ultrasonic.h>

Ultrasonic ultrasonic(05, 06);
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x11 };
const char* mqtt_server = "test.mosquitto.org";
unsigned long previousMillis = 0; // will store last time LED was updated
// constants won't change :
const long interval = 1000; // interval at which to blink (milliseconds)

void callback(char* topic, byte* payload, unsigned int length) {
  // delay(1000);
}

EthernetClient ethClient;
PubSubClient client(mqtt_server, 1883, callback, ethClient);
long lastReconnectAttempt = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("iniciando...");

  if (Ethernet.begin(mac)) {
    Serial.println(Ethernet.localIP());
  } else {
    Serial.println("Falha no DHCP");
  }
  delay(1500);
  lastReconnectAttempt = 0;
}


void loop()
{
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      Serial.println("reconectando...");
      lastReconnectAttempt = now;

      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    Serial.print("Distance in CM: ");
    Serial.println(ultrasonic.distanceRead());
    int distancia = ultrasonic.distanceRead();
    if (distancia > 20) {
      client.publish("senai-code-xp/vagas/11", "1", true);
    } else {
      client.publish("senai-code-xp/vagas/11", "0", true);
    }
    client.loop();
  }
}


boolean reconnect() {
  Serial.println("reconectando...");
  if (client.connect("couceiroLeo")) {
    Serial.println("conectado");
    // client.publish("vaga/1","hello world");
    client.subscribe("senai-code-xp/vagas/11");
  }
  return client.connected();
}


