#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

// --- CONFIGURACIÓN WIFI ---
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// --- CONFIGURACIÓN MQTT ---
const char* mqtt_server = "52.201.244.137"; // tu EC2
const int mqtt_port = 1883;
const char* mqtt_topic = "expo/prueba";

WiFiClient espClient;
PubSubClient client(espClient);

// --- CONFIGURACIÓN DHT22 ---
int pinDHT = 15;
DHTesp dht;

void setup() {
  Serial.begin(115200);

  // Inicializamos DHT
  dht.setup(pinDHT, DHTesp::DHT22);

  // Conectamos al WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado al WiFi");

  // Configuramos el cliente MQTT
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  // Reconectar MQTT si se cae
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // Leer datos DHT22
  TempAndHumidity data = dht.getTempAndHumidity();
  float temperature = data.temperature;
  float hr = data.humidity;

  // Simular índice UV (0 a 11+)
  float uv = random(0, 120) / 10.0; // genera 0.0 a 11.9

  // Crear payload en formato JSON
  String payload = "{";
  payload += "\"temperatura\": " + String(temperature, 2) + ",";
  payload += "\"humedad\": " + String(hr, 1) + ",";
  payload += "\"indice_uv\": " + String(uv, 1);
  payload += "}";

  // Publicar mensaje
  if(client.publish(mqtt_topic, payload.c_str())) {
    Serial.println("Mensaje enviado: " + payload);
  } else {
    Serial.println("Error al enviar mensaje");
  }

  delay(2000); // Espera 2 segundos
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Error, rc=");
      Serial.print(client.state());
      Serial.println(" intentando de nuevo en 2 segundos");
      delay(2000);
    }
  }
}
