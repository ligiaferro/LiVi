#ifdef receptor

#include <Arduino.h>
#include <WiFi.h>
#include <LoRa.h>
#include <FirebaseESP32.h>

// --- Configurações do LoRa ---
#define SS 18
#define RST 14
#define DI0 26
#define BAND 915E6

// --- Configurações de WiFi e Firebase ---
#define WIFI_SSID "CINGUESTS"
#define WIFI_PASSWORD "acessocin"

#define FIREBASE_HOST "redes-vivi-lili-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "xusgkJhs60fOrczP6zgs6A7zZtJ1yxIWiW3BBlQF" 

// Objetos
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

// --- VARIÁVEIS DE CONTROLE DE TEMPO (NOVO!) ---
unsigned long lastSendTime = 0;
unsigned long sendInterval = 2000; // Envia para o Firebase a cada 2000ms (ajuste se precisar)
float latestDbValue = 0;          // Guarda o último valor lido
bool newValueAvailable = false;   // Avisa se tem novidade pra enviar

void setup() {
  Serial.begin(115200);

  // 1. WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nConectado WiFi!");

  // 2. Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  
  // Aumenta o tempo limite de espera do Firebase para evitar erros rápidos
  config.timeout.serverResponse = 10 * 1000; 

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // 3. LoRa
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Erro LoRa!");
    while (1);
  }
  Serial.println("Sistema Pronto. Limitando envios para a nuvem...");
}

void loop() {
  // --- PARTE 1: RECEBER DO LORA (O MAIS RÁPIDO POSSÍVEL) ---
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    String receivedText = "";
    while (LoRa.available()) {
      receivedText += (char)LoRa.read();
    }

    // Processa a string "Mic Val: ... | dB: ..."
    if (receivedText.startsWith("Mic Val: ")) {
      int indexDB = receivedText.indexOf(" | dB: ");
      if (indexDB != -1) {
        String stringDB = receivedText.substring(indexDB + 7);
        
        // Apenas ATUALIZA a variável global, não envia ainda!
        latestDbValue = stringDB.toFloat();
        newValueAvailable = true; 
        
        // Debug local (pode deixar, pois Serial é rápido)
        Serial.print("Lido do LoRa: ");
        Serial.println(latestDbValue);
      }
    }
  }

  // --- PARTE 2: ENVIAR PARA O FIREBASE (COM CALMA) ---
  // Só entra aqui se já passou o tempo (sendInterval) E se tem dado novo
  if (millis() - lastSendTime > sendInterval && newValueAvailable) {
    
    Serial.print(">>> Enviando para Nuvem: ");
    Serial.println(latestDbValue);

    // Usa setFloat (Modo Volátil)
    if (Firebase.setFloat(firebaseData, "/sensores/agora", latestDbValue)) {
      Serial.println("Sucesso!");
    } else {
      Serial.print("Falha no envio: ");
      Serial.println(firebaseData.errorReason());
    }

    // Reseta o cronômetro e a flag
    lastSendTime = millis();
    newValueAvailable = false;
  }
}

#endif