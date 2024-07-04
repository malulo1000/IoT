#define SENSOR_HUMEDAD_PIN A1
#define LED_PIN 13

const int MIN_HUMEDAD_SUELO = 500;
int humedadSuelo;

void setup() {
  Serial.begin(9600);

  pinMode(SENSOR_HUMEDAD_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  humedadSuelo = analogRead(SENSOR_HUMEDAD_PIN);

  Serial.print("Humedad del suelo = ");
  Serial.println(humedadSuelo);

  if (humedadSuelo > MIN_HUMEDAD_SUELO) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(1000);
}
