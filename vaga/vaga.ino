#include <UIPEthernet.h>
#include <utility/logging.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <Ultrasonic.h>
//tigger 05   echo 06
Ultrasonic ultrasonic(05, 06);
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x11 };

//const char* mqtt_server = "192.168.3.186";
const IPAddress mqtt_server(192, 168, 3, 186);
unsigned long previousMillis = 0; // will store last time LED was updated
// constants won't change :
const long interval = 1000; // interval at which to blink (milliseconds)

void callback(char* topic, byte* payload, unsigned int length) {
  // delay(1000);
}

EthernetClient ethClient;
PubSubClient client(mqtt_server, 1883, callback, ethClient);
long lastReconnectAttempt = 0;
long intervaloSensor = 0;

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

  if (client.connect("couceiroLeo")) {
    Serial.println("conectado");
    // client.publish("vaga/1","hello world");
    client.subscribe("vagas/11");
    client.connect("couceiroLeo",  "vagas/11",   0,   true,  ""); // envia mensagem vazia para o topico caso o arduino fique fora de operação
  }
  lastReconnectAttempt = 0;
}


void loop()
{
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 3000) {
      Serial.println("reconectando...");
      lastReconnectAttempt = now;

      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else if (client.connected()) {
    long now2 = millis();
    if (now2  - intervaloSensor > 1000) {
      intervaloSensor = now2;
      Serial.print("Distance in CM: ");
      Serial.println(ultrasonic.distanceRead());
      int distancia = ultrasonic.distanceRead();
      if (distancia > 20) {
        client.publish("vagas/10", "1", true);
        //delay(1000);
      } else {
        client.publish("vagas/10", "0", true);
        // delay(1000);
      }
      intervaloSensor = now2;
      client.loop();
    }
  }
}

boolean reconnect() {
  Serial.println("reconectando...");
  if (client.connect("couceiroLeo")) {
    Serial.println("conectado");
    // client.publish("vaga/1","hello world");
    client.subscribe("vagas/11");
    client.connect("meu-clientid",  "topico-testamento",   0,   true,  "mensagem-testamento");
  }
  return client.connected();
}


