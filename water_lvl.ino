#include <AdafruitIO_WiFi.h>
#include <AdafruitIO.h>

#define TRIG_PIN D3
#define ECHO_PIN D4
#define RELAY_PIN D7
#define AIO_USERNAME    "waterlevel0002"
#define AIO_KEY         "aio_aWuE60QPqiNWmwYb7JbR1nd3CHVn"
#define WIFI_SSID       "iotproject1"
#define WIFI_PASS       "iotproject1"

AdafruitIO_WiFi io(AIO_USERNAME, AIO_KEY, WIFI_SSID, WIFI_PASS);
AdafruitIO_Feed *level = io.feed("level");

void setup() {
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    Serial.print("Connecting to Adafruit IO");
    io.connect();

    // Wait for a connection
    while (io.status() < AIO_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("Connected to Adafruit IO!");
}

void loop() {
    io.run();

    long duration, distance;
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration / 58.2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    if (distance > 50)
      distance = 50;
    float percentage = map(distance, 0, 50, 0, 100);
    Serial.println(percentage);
    percentage = 100 - percentage;

    digitalWrite(RELAY_PIN, distance < 10 ? HIGH : LOW);

    static unsigned long previousMillis = 0;
    const unsigned long interval = 15000;
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        level->save(percentage);
    }

    delay(1000);
}
