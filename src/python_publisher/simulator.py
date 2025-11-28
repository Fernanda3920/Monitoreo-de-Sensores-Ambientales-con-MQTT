import paho.mqtt.client as mqtt
import time
import random

# --- CONFIGURACIÓN DEL BROKER AWS ---
BROKER = "34.202.205.53"  # IP pública del broker Mosquitto en AWS EC2
PORT = 1883
TOPIC = "expo/wokwi"      # Tópico de publicación/suscripción

# --- CONEXIÓN INICIAL ---
client = mqtt.Client()
client.connect(BROKER, PORT, 60)
print(f"Conectado a {BROKER}. Iniciando simulación...")

# --- BUCLE DE SIMULACIÓN Y PUBLICACIÓN ---
while True:
    # 1. Generar datos simulados
    temperature = round(random.uniform(20.0, 30.0), 2)  # Temperatura (C)
    hr = round(random.uniform(40.0, 70.0), 2)           # Humedad (%)
    uv = round(random.uniform(0.0, 11.0), 1)            # Índice UV 

    # 2. Construir el payload JSON
    payload = f'{{"temperatura": {temperature}, "humedad": {hr}, "indice_uv": {uv}}}'

    # 3. Publicar en MQTT
    client.publish(TOPIC, payload)
    print("Enviado:", payload)

    # 4. Esperar 5 segundos
    time.sleep(5)
