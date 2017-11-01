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

void setup()
{
  Serial.begin(9600);
  while (!Serial) {}

  //
  if (Ethernet.begin(mac)) {
    Serial.println(Ethernet.localIP());
  } else {
    Serial.println("Falha no DHCP");
  }
  reconnect();
  //delay(1000);
}



void loop()
{
  boolean publicacaoVaga = true;
  reconnect();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    Serial.print("Distance in CM: ");
    Serial.println(ultrasonic.distanceRead());
    int distancia = ultrasonic.distanceRead();
    if (distancia > 20) {
      client.publish("senai-code-xp/vagas/11", "1");
    } else {
      client.publish("senai-code-xp/vagas/11", "0");
    }
    previousMillis = currentMillis;
  }
    client.publish("senai-code-xp/vagas/11", "Vai Tricolor!!!!!",publicacaoVaga);
    if(publicacaoVaga){
      Serial.println("Deu CERTOOOOO!! Publicou!!");
      }
      else{
        Serial.println("Nao deu certo!!! Nao Publicou...");
      }
      
  
  client.loop();
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Tentando conectar ao Servidor MQTT...");
    // Create a random client ID
    // Attempt to connect
    if (client.connect("eaeaedfdfa87")) {
      Serial.println("Cheguei aqui");
      client.publish("senai-code-xp/vagas/11", "vaicorinthians");
      client.subscribe("senai-code-xp/vagas/11");
      client.setCallback(callback);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}



