#include <LiquidCrystal.h>
#include <UIPEthernet.h>
#include <utility/logging.h>
#include <SPI.h>
#include <PubSubClient.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 9, en = 8, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x10 };
const char* mqtt_server = "test.mosquitto.org";
int vagaDisponivel;
int vagaOcupada;
boolean retainedMessage;

const int pinLed = 7;

void callback(char* topic, byte* payload, unsigned int length) {
  String mensagem;
  String topicStr;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    mensagem = mensagem + (char)payload[i];
  }

  Serial.println(mensagem);
  
  Serial.println();

  for (int i = 0; i < length; i++) {
    topicStr = (char)topic[i];
  }

  Serial.println(topicStr);

  //  String mensagem = String(payload[0]);
  Serial.println("Esta Ã© mensagem que vei do mosquito:");
  Serial.println(mensagem);

  String ArrayTopic[40];

  for (int x = 0; x < 40; x++) {
    if (topicStr == ArrayTopic[x] && mensagem == 0) {
      vagaOcupada += 1;
      vagaDisponivel -= 1;

    } else if (topicStr == ArrayTopic[x] && mensagem == 1) {
      vagaOcupada -= 1;
      vagaDisponivel += 1;

    }
    if (topicStr != ArrayTopic[x]) {
      ArrayTopic[x] = topicStr;
      Serial.println(ArrayTopic[x]);
    }
  }

  /*
  if (topicStr == "senai-code-xp/vagas/10") {
    Serial.println("Esta Ã© mensagem que vei do mosquito:");
    Serial.println(mensagem);
  }
  else
  {
    Serial.println("Nao recebi nenhuma mensagem neste topico");
    Serial.println(topicStr);
  }

  if (mensagem == 0) {
    Serial.println("Não disponível");
    vagaOcupada += 1;

  } else if (mensagem == 1) {
    Serial.println("Disponível");
    vagaDisponivel += 1;
  }

  Serial.println("Vaga Ocupada:" + vagaOcupada);
  Serial.println("Vaga Disponível:" + vagaDisponivel);
  */

  delay(5000);
}

EthernetClient ethClient;
PubSubClient client(mqtt_server, 1883, callback, ethClient);

void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);

  //client.willRetain = true;

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  lcd.print("Kevin");

  if (!Ethernet.begin(mac)) {
    Serial.println("Falha no DHCP");
  } else {
    Serial.println(Ethernet.localIP());
  }

  retainedMessage = true;

  delay(1000);

  if (client.connect("Placa0X10")) {
    Serial.println("Cheguei aqui");
    // Serial.flush();
    client.publish("senai-code-xp/vagas/10", "vaicorinthians", retainedMessage);
    client.subscribe("senai-code-xp/vagas/#");
    client.setCallback(callback);

  }
  else {
    Serial.println("Nao conectei ao servidor MQTT");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    // Attempt to connect
    if (client.connect("Placa")) {
      Serial.println("connected");
      delay(10 * 1000);
      // Once connected, publish an announcement...
      client.publish("senai-code-xp/vagas/10", "vaicorinthians", retainedMessage);
      client.subscribe("senai-code-xp/vagas/#");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  digitalWrite(pinLed, HIGH);
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
  client.loop();

}

