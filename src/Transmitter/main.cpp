#ifdef transmitter

#include <Arduino.h>
#include <LoRa.h>

#define SS 18
#define RST 14
#define DI0 26
#define BAND 915E6
#define PIN_MIC 36

int sampleWindow = 50;

int lastPeak = -1;         // Guarda o último valor enviado
float threshold = 20.0;    // Diferença mínima entre leituras para enviar

void setup() {
  Serial.begin(115200);
  while (!Serial);

  LoRa.setPins(SS, RST, DI0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  pinMode(PIN_MIC, INPUT);
  Serial.println("LoRa initialized");
}

void loop() {
  unsigned long startMillis = millis();
  unsigned int signalMax = 0;
  unsigned int signalMin = 4095;
  unsigned int sample;

  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(PIN_MIC);

    if (sample < 4096) {
      if (sample > signalMax) signalMax = sample;
      if (sample < signalMin) signalMin = sample;
    }
  }

  int peakToPeak = signalMax - signalMin;
  float dB = 20 * log10(peakToPeak / 512.0);

  Serial.print("Amplitude: ");
  Serial.println(peakToPeak);
  Serial.print("dB: ");
  Serial.println(dB);

  // --- NÃO ENVIAR SE MUITO PRÓXIMO DO ANTERIOR ---
  if (lastPeak != -1 && abs(peakToPeak - lastPeak) < threshold) {
    Serial.println("Variação pequena demais. Não enviando...");
    delay(1000);
    return;
  }

  // --- Envia pelo LoRa ---
  LoRa.beginPacket();
  LoRa.print("Mic Val: ");
  LoRa.print(peakToPeak);
  LoRa.print(" | dB: ");
  LoRa.print(dB);
  LoRa.endPacket();

  Serial.println("Packet sent!");

  // Atualiza último valor enviado
  lastPeak = peakToPeak;

  delay(1000);
}

#endif
