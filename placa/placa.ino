#include <LiquidCrystal.h>
#include <UIPEthernet.h>
#include <utility/logging.h>
#include <SPI.h>
//#ifdef ONLINE
#include <PubSubClient.h>
//#else
//#include <SerialPubSubClient.h>
//#endif

const int rs = 9, en = 8, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x10 };
const char* mqtt_server = "test.mosquitto.org";
//const IPAddress mqtt_server(192, 168, 3, 186);
int vagaDisponivel;
int vagaOcupada;

const int pinLed = 7;
const int pinLcd = A0;
int vagas[41];
int newTopic;

unsigned long timeAtualiza = 0; // will store last time LED was updated
// constants won't change :
const long interval = 1000; // interval at which to blink (milliseconds)

void callback(char* topic, byte* payload, unsigned int length) {
  timeAtualiza = millis();
  lcd.display();
  digitalWrite(pinLcd, HIGH);
  // ,,,,, acender visor...
  Serial.println("Estou no callback");

  char* mensagem = (char)payload[0] - 48;
  Serial.print("Mensagem: ");
  Serial.print(int(mensagem));
  Serial.println("");
  //Serial.println((char)payload[0]);

  String vaga = String(topic[20]);
  vaga += topic[21];

  Serial.print("vaga: ");
  Serial.print(vaga.toInt());
  Serial.println("");

  if (vagas[vaga.toInt()] == -1) {
    if (mensagem == 0) {
      vagaOcupada += 1;
      vagas[vaga.toInt()] = mensagem;

    } else if (mensagem == 1) {
      vagaDisponivel += 1;
      vagas[vaga.toInt()] = mensagem;

    }
  } else if (vagas[vaga.toInt()] == 0) {
    if (mensagem == 1) {
      vagaOcupada -= 1;
      vagaDisponivel += 1;
      vagas[vaga.toInt()] = mensagem;

    }
  } else if (vagas[vaga.toInt()] == 1) {
    if (mensagem == 0) {
      vagaOcupada += 1;
      vagaDisponivel -= 1;
      vagas[vaga.toInt()] = mensagem;

    }
  }

  Serial.print("Vagas disponiveis: ");
  Serial.print(vagaDisponivel);
  Serial.print(" e ocupadas: ");
  Serial.println(vagaOcupada);

}

EthernetClient ethClient;
PubSubClient client(mqtt_server, 1883, callback, ethClient);

void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  //for que insere os valores -1 as vagas
  for (int i = 0; i < 41; i++) {
    vagas[i] = -1;
  }

  pinMode(pinLcd, OUTPUT);

  Serial.print("oi");

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  lcd.print("Quantidade de");

  Serial.println();

  if (!Ethernet.begin(mac)) {
    Serial.println("Falha no DHCP");
  } else {
    Serial.println(Ethernet.localIP());
  }

  if (client.connect("Placa0X10")) {
    /*if (client.connect("Placa0X10",
                       "luz",
                       1,
                       true,
                       "Morreu"
                      )) {*/
    Serial.println("connected");
    delay(10 * 1000);
    // Once connected, publish an announcement...
    //client.publish("vagas/10", "vaicorinthians", true);
    client.subscribe("senai-code-xp/vagas/#");
    //client.subscribe("vagas/#");

  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println("try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(1000);

  }

  //delay(1000);
  timeAtualiza = millis();
}

void reconnect() {
  // Loop until we're reconnected
  Serial.println("Estou no reconnect");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    // Attempt to connect
    if (client.connect("Placa0X10")) {
      /*if (client.connect("Placa0X10",
                         "luz",
                         1,
                         true,
                         "Morreu"
                        )) {*/
      Serial.println("connected");
      //delay(10 * 1000);
      // Once connected, publish an announcement...
      //client.publish("vagas/10", "vaicorinthians", true);
      
      client.subscribe("senai-code-xp/vagas/#");
      //client.subscribe("vagas/#");

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
  int now = millis();
  if (now - timeAtualiza > interval) {
   // Serial.println("Apagando LCD"); // colocar aqui o codigo para apagar o LED.
    lcd.noDisplay();
    digitalWrite(pinLcd, LOW);
  }

  if (!client.connected()) {
    reconnect();
  }


  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print("Vagas livres: ");
  lcd.print(vagaDisponivel);

  if (vagaDisponivel == 0) {
    digitalWrite(pinLed, HIGH);
  }

  client.loop();

}

