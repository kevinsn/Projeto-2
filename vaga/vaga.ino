#include <UIPEthernet.h>
#include <utility/logging.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <Ultrasonic.h>
//tigger 05   echo 06
Ultrasonic ultrasonic(05, 06); // usando porta 05 pra trigger e 06 pra echo
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x10 };

//const char* mqtt_server = "192.168.3.186";
const IPAddress mqtt_server(192, 168, 3, 186);
unsigned long previousMillis = 0; // will store last time LED was updated
// constants won't change :
const long interval = 1000; // interval at which to blink (milliseconds)

const int pinLed = 7;
const int pinLedMqtt = 8;

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

  pinMode(pinLed, OUTPUT);
  pinMode(pinLedMqtt, OUTPUT);
  while (!Serial) {}
  Serial.println("iniciando...");

  if (Ethernet.begin(mac)) {
    Serial.println(Ethernet.localIP());
  } else {
    Serial.println("Falha no DHCP");
  }
  delay(1500);

  if (client.connect("Placa0X10",
                     "vagas/10",
                     1,
                     true,
                     "0"
                    )) {
    Serial.println("conectado");
    // client.publish("vaga/1","hello world");
    client.subscribe("vagas/10");
    client.connect("couceiroLeo",  "vagas/10",   0,   true,  ""); // envia mensagem vazia para o topico caso o arduino fique fora de operação
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
        digitalWrite(pinLedMqtt, HIGH);
      }
    }
  } else if (client.connected()) {
    digitalWrite(pinLedMqtt, HIGH);
    long now2 = millis();
    if (now2  - intervaloSensor > 1000) {
      intervaloSensor = now2;
      Serial.print("Distance in CM: ");
      Serial.println(ultrasonic.distanceRead());
      int distancia = ultrasonic.distanceRead();
      if (distancia > 9) {
        client.publish("vagas/10", "1", true);
        digitalWrite(pinLed, HIGH);
        //delay(1000);
      } else {
        client.publish("vagas/10", "0", true);
        digitalWrite(pinLed, LOW);
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
    digitalWrite(pinLedMqtt, LOW);
    delay(250);
    digitalWrite(pinLedMqtt, HIGH);
    delay(250);
    digitalWrite(pinLedMqtt, LOW);
    delay(250);
    digitalWrite(pinLedMqtt, HIGH);
    delay(250);
    digitalWrite(pinLedMqtt, LOW);
    delay(250);
    digitalWrite(pinLedMqtt, HIGH);
    delay(250);
    digitalWrite(pinLedMqtt, LOW);
    delay(250);
    digitalWrite(pinLedMqtt, HIGH);
    delay(250);
    digitalWrite(pinLedMqtt, LOW);
    Serial.println("conectado");
    // client.publish("vaga/1","hello world");
    client.subscribe("vagas/10");
    client.connect("meu-clientid",  "topico-testamento",   0,   true,  "mensagem-testamento");
  }
  return client.connected();
}


