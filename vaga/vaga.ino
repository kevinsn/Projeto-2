#include <UIPEthernet.h>
#include <utility/logging.h>
#include <SPI.h>
#include <PubSubClient.h>

const int LedLuz = 9;
// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x11 };
//IPAddress server(172, 16, 0, 2);
const char* mqtt_server = "test.mosquitto.org";

void callback(char* topic, byte* payload, unsigned int length) {
String mensagem;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
mensagem = mensagem + (char)payload[i];
  }
 // Serial.println();

//  String mensagem = String(payload[0]);
  Serial.println("Esta Ã© mensagem que vei do mosquito:");
  Serial.println(mensagem);

String topicStr = String(topic);

  if (topicStr == "senai-code-xp/vagas/11") {
Serial.println("Esta Ã© mensagem que vei do mosquito:");
      Serial.println(mensagem);
  }
  else
  {
    Serial.println("Nao recebi nenhuma mensagem neste topico");
  }
    /*
    // Switch on the LED if an 1 was received as first character

    if (payload[0] == '0') {
      digitalWrite(LedLuz, LOW);   // Turn the LED on (Note that LOW is the voltage level
      Serial.println("Desligado");
      String mensagem = String(payload[0]);
      // but actually the LED is on; this is because
      // it is acive low on the ESP-01)
    } else {
      // digitalWrite(LedLuz, HIGH);  // Turn the LED off by making the voltage HIGH
      Serial.println("Ligado");
      String mensagem = String(payload[0]);
      Serial.println("Esta Ã© mensagem que vei do mosquito:");
      Serial.println(mensagem);
    }

  }
  else {
    Serial.println("NÃ£o funcionou");
    // client.loop();

  }*/
  delay(5000);
}

EthernetClient ethClient;
PubSubClient client(mqtt_server, 1883, callback, ethClient);

void setup()
{
  Serial.begin(9600);
  while (!Serial) {}

  pinMode(LedLuz, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  if (Ethernet.begin(mac)) {
    Serial.println(Ethernet.localIP());
  } else {
    Serial.println("Falha no DHCP");
  }
delay(1000);
   //if (client.connect("mqttdash-785fc3ce")){
  //if (client.connect( )) {
  if(client.connect("eaeaedfdfa8788787878e")){
  Serial.println("Cheguei aqui");
 // Serial.flush();
  client.publish("senai-code-xp/vagas/11", "vaicorinthians");
  client.subscribe("senai-code-xp/vagas/11");
  client.setCallback(callback);
  
   }
   else {
   Serial.println("Nao conectei ao servidor MQTT");
   }
  // Serial.println("NÃ£o Cheguei aqui");
}

void loop()
{
/*
 if(client.connect("eaeaedfdfa8788787878e")){
  Serial.println("Cheguei aqui");
 // Serial.flush();
  client.publish("senai-code-xp/vagas/11", "vaicorinthians");
  client.subscribe("senai-code-xp/vagas/11");
  client.setCallback(callback);
  
   }
   else {
   Serial.println("Nao conectei ao servidor MQTT");
   }
   delay(10000);
   */
  client.loop();
}

