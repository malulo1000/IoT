#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

const char *ssid =  "***"; // Nombre de la red
const char *pass = "***";  // Contraseña de la red
const char *server = "***";  // Dirección IP del servidor
const int port = 8081;                  // Puerto del servidor
const char *endpoint = "/data";   // Ruta del recurso

WiFiClient client;
HTTPClient http;
Adafruit_MPU6050 mpu;
int id = 1;

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  double aX = a.acceleration.x;
  double aY = a.acceleration.y;
  double aZ = a.acceleration.z;

  double gX = g.gyro.x;
  double gY = g.gyro.y;
  double gZ = g.gyro.z;

  String postData = "{\"id\": " + String(id) + ", ";
  postData += "\"ax\": " + String(aX, 15) + ", ";
  postData += "\"ay\": " + String(aY, 15) + ", ";
  postData += "\"az\": " + String(aZ, 15) + ", ";
  postData += "\"gx\": " + String(gX, 15) + ", ";
  postData += "\"gy\": " + String(gY, 15) + ", ";
  postData += "\"gz\": " + String(gZ, 15) + "}";

  if (http.begin(client, server, port, endpoint)) {
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      Serial.print("POST request enviado con éxito, código de respuesta: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error en el POST request, código de respuesta: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Error al iniciar la conexión HTTP");
  }

  Serial.print("ID:");
  Serial.print(id);
  Serial.println("");

  Serial.print("Acceleration X: ");
  Serial.print(aX, 2);
  Serial.print(", Y: ");
  Serial.print(aY, 2);
  Serial.print(", Z: ");
  Serial.print(aZ, 2);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(gX, 2);
  Serial.print(", Y: ");
  Serial.print(gY, 2);
  Serial.print(", Z: ");
  Serial.print(gZ, 2);
  Serial.println(" rad/s");
  Serial.println("");

  Serial.print("aX:");
  Serial.print(aX);
  Serial.print("\taY:");
  Serial.print(aY);
  Serial.print("\taZ:");
  Serial.print(aZ);
  Serial.print("\tgX:");
  Serial.print(gX);
  Serial.print("\tgY:");
  Serial.print(gY);
  Serial.print("\tgZ:");
  Serial.println(gZ);

  id++;
}
