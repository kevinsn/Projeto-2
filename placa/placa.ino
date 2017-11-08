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
int vagaDisponivel;
int vagaOcupada;

const int pinLed = 7;
int vagas[40];
int newTopic;

void callback(char* topic, byte* payload, unsigned int length) {

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
  /*
    for (int x = 0; x < 40; x++) {
      topic[vaga[x]];
      vagas[x];
    }
    vaga[1] = "senai-code-xp/vagas/01";
    vaga[2] = "senai-code-xp/vagas/02";
    vaga[3] = "senai-code-xp/vagas/03";
    vaga[4] = "senai-code-xp/vagas/04";
    vaga[1] = "senai-code-xp/vagas/05";
    vaga[1] = "senai-code-xp/vagas/06";
    vaga[1] = "senai-code-xp/vagas/07";
    char* mensagem = (char)payload[0] - 48;

    if (int(mensagem) == '0') {
      Serial.println("Vaga nao disponivel");
    }  else if {
    Serial.println("Vaga disponivel");
    }*/


  vagas[vaga.toInt()] = mensagem;

  if (vagas[vaga.toInt()] == 0) {
    vagaOcupada += 1;
    vagaDisponivel -= 1;
  } else if (vagas[vaga.toInt()] == 1) {
    vagaOcupada -= 1;
    vagaDisponivel += 1;
  }


  /*
    if (vaga == vagas[x] && mensagem == 0) {
    vagaOcupada += 1;
    vagaDisponivel -= 1;

    } else if (vaga == vagas[x] && mensagem == 1) {
    vagaOcupada -= 1;
    vagaDisponivel += 1;

    } else if ((vaga != vagas[x]) && (vagas[x] == -1)) {
    vagas[x] = vaga.toInt();
    if (mensagem == 0) {
      vagaOcupada += 1;

    } else if (mensagem == 1) {
      vagaOcupada -= 1;

    }
    }* /

    }

    Serial.print("Vagas disponiveis: ");
    Serial.print(vagaDisponivel);
    Serial.print("e ocupadas: ");
    Serial.println(vagaOcupada); * /

    /*int teste1 = topic[20] - 48;
    int teste2 = topic[21] - 48; //- '1';
    //Serial.print(topic[19]);
    //Serial.println(topic[20, 21]);

    //Serial.print(teste1);
    //Serial.print(teste2);

    int testeFinal = teste1 * 10 + teste2;

    //Serial.println(testeFinal);
    //Serial.print(teste1 + teste2);


    char* newTopic = topic[20] + topic[21];

    Serial.println(newTopic);

    //vagas = topic[20] + topic[21];

    Serial.println();*/

  /*for (int i = 40; i < length; i++) {
    if (vagas[i] == 0) {


    }
    }*/
}

EthernetClient ethClient;
PubSubClient client(mqtt_server, 1883, callback, ethClient);

void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  //for que insere os valores -1 as vagas
  for (int i = 0; i < 40; i++) {
    vagas[i] = -1;
  }

  Serial.print("oi");

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  lcd.print("Kevin");

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
    //client.publish("senai-code-xp/vagas/10", "vaicorinthians", true);
    client.subscribe("senai-code-xp/vagas/#");

  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println("try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(1000);
  }

  //delay(1000);

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
      client.publish("senai-code-xp/vagas/10", "vaicorinthians", true);
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

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
  client.loop();

}

