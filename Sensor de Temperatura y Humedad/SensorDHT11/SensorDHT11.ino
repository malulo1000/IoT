#include <DHT.h>
#include <ESP8266WiFi.h>

String apiKey = "****"; // Clave de escritura de ThingSpeak
const char *ssid = "***"; // Nombre de la red WiFi
const char *pass = "***"; // Contraseña de la red WiFi
const char *server = "api.thingspeak.com";

#define DHTPIN 2
DHT dht(DHTPIN, DHT11);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();

  Serial.println("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado");
}

void loop() {
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println("¡Error al leer el sensor DHT!");
    return;
  }

  if (client.connect(server, 80)) {
    String postStr = "api_key=" + apiKey + "&field1=" + String(temperatura) + "&field2=" + String(humedad) + "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print(" grados Celsius, Humedad: ");
    Serial.print(humedad);
    Serial.println("%. Enviado a ThingSpeak.");
  } else {
    Serial.println("Error al conectar con ThingSpeak.");
  }
  
  client.stop();
  
  Serial.println("Esperando...");
  delay(500);
}
