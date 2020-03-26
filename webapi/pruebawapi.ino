#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "RestClient.h"
#include <SPI.h>

// Update these with values suitable for your network.

const char* ssid = "morado";
const char* password = "87654321";
const char* server = "127.0.0.1:5000";
const char* port="5000";
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
	char json[256];
	JsonObject& root = jsonBuffer.createObject();
	root["name"] = "S3E1";
	root["estado"] = snprintf;
	root.printTo(json, sizeof(json));
	int statusCode = client.post("/framework", json, &response);
	Serial.print("Status code from server: ");
	Serial.println(statusCode);
	Serial.print("Response body from server: ");
	Serial.println(response);
	delay(1000);
  
}
/*
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
}*/

