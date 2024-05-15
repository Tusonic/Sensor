#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN D7 // Zdefiniowanie wejścia danych z DHT
#define DHTTYPE DHT22 // Zdefiniowanie modelu DHT

DHT_Unified dht(DHTPIN, DHTTYPE); // Zdefiniowanie obiektu DHT


const char* ssid = "XXXXXXXXXX"; // Nazwa sieci WiFi
const char* password = "XXXXXXXXXX"; // Hasło do sieci WiFi
const char* newHostname = "XXXXXXXXXX"; // Custom hostname

// Zdefiniowanie serwera na ESP oraz jego portu
ESP8266WebServer server(80);

void setup() { // Funkcja bootowa kontrolera
  // Inicjalizacja komunikacji szeregowej oraz jej prędkości
  Serial.begin(9600);

  // Inicjalizacja czujnika DHT
  dht.begin();

  // Set custom hostname
  WiFi.hostname(newHostname);

  // Połączenie z wcześniej zdefiniowaną siecią WiFi
  WiFi.begin(ssid, password);

  // Czekanie na połączenie z WiFi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Łączenie z WiFi...");
  }

  Serial.println("Połączono z WiFi");
  Serial.println("Twoje IP");
  Serial.println(WiFi.localIP());
  Serial.println("--------------------");

  // Definiowanie endpointu /dane, który zwróci dane w formacie JSON
  server.on("/dane", HTTP_GET, []() {

    // Odczytaj temperaturę i wilgotność
    // Pobranie eventu
    sensors_event_t event;
    // Pobranie temperatury
    dht.temperature().getEvent(&event);
    float temperature = event.temperature;
    // Pobranie wilgotności
    dht.humidity().getEvent(&event);
    float humidity = event.relative_humidity;

    // Tworzenie JSON
    String jsonData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";

    // Wysłanie JSON jako odpowiedź na zapytanie
    server.send(200, "application/json", jsonData);
  });

  // Tworzenie serwera
  server.begin();
}

void loop() {
  // Nasłuch połączeń
  server.handleClient();
}
