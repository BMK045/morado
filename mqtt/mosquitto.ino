/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "morado";
const char* password = "87654321";
const char* mqtt_server = "192.168.137.39";
const int buttonPin = 13;     // Pin del pulsador
const int buttonPin2 = 14;     // Pin del pulsador
int d;
int i;
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("casa", "Enviando mensaje");
      // ... and resubscribe
       client.subscribe("casa");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);     
  pinMode(buttonPin2, INPUT_PULLUP);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
 long now = millis();

    ++value;
delay(2000);
    // Si el valor del pin del pulsador es HIGH es que esta pulsado
  if (digitalRead(buttonPin) == HIGH) {    
           snprintf (msg,50, "POSICION DERECHA #%ld", value);
    
                          }
  else if (digitalRead(buttonPin2) == HIGH){
           snprintf (msg, 50, "POSICION IZQUIERDA #%ld", value);
                                  }
 else{
        snprintf (msg, 50, "POSICION CENTRAL #%ld", value);
  }
Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("casa", msg);
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

 

  
}

void loop_post{
  if(WiFi.status()== WL_CONNECTED){
    
    HTTPClient http;
    String datos_a_enviar = "&disp=" + dispositivo;
    
    http.begin("");
    
    
    int codigo_respuesta = htttp.POST(datos_a_enviar);
    
    if(codigo_respuesta>0){
      Serial.println("Codigo HTTP > " + String(codigo_respuesta));
      
      if(cdigo_respuesta == 200){
        String cuerpo_respuest = http.getString();
        Serial.println("El servidor respondio");
        Serial.println(cuerpo_respuesta);
  }
    }else{
       Serial.print("Error enviando POST, codigo: ");
       Serial.println(codigo_respuesta);
    }
    
    http.end();
    
  }else{
    
    Serial.println("Error en la conexion WIFI");
  }
 delay(2000); 
}
  
}
