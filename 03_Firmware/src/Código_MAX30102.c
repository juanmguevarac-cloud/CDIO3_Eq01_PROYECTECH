/****************************************************
 * ESP32 + MAX30105 + Blynk IoT
 * Medición activada por botón (V6)
 * Envía:
 *  V0 = SpO2 promedio
 *  V1 = BPM promedio
 *  V2 = Estado general
 *  V3 = Estado SpO2
 *  V4 = Estado BPM
 *  V5 = Riesgo (bajo/medio/alto)
 *  V7 = Tiempo restante (30s,20s,10s,finalizando)
 *  V8 = Calidad de contacto (bueno/mal)
 *
 * Optimizado para bajo consumo de mensajes.
 * SIN deep sleep. Requiere botón para iniciar medición.
 ****************************************************/

#define BLYNK_TEMPLATE_ID "TMPL2rg2VihW0"
#define BLYNK_TEMPLATE_NAME "PROYECTECH"
#define BLYNK_AUTH_TOKEN "LMd8lfxlwVdbG6N9VeSj741bfzC6TnTl"

//#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Samuel";
char pass[] = "12345678";

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

MAX30105 particleSensor;

// Tiempo de medición
const unsigned long MEASURE_TIME_MS = 30000;

// Control
bool measuring = false;
unsigned long measureStart = 0;

// Variables de contacto
String calidadContacto = "Bueno";

// BPM
unsigned long lastBeatTime = 0;
double bpmSum = 0;
int bpmSamples = 0;

// SpO2
double sumRed = 0, sumIR = 0;
double sumSqRed = 0, sumSqIR = 0;
unsigned long sampleCount = 0;

// === BOTÓN V6 ===
BLYNK_WRITE(V6) {
  int valor = param.asInt();
  if (valor == 1 && !measuring) {
    Blynk.virtualWrite(V6, 0);  // auto-reset del botón
    startMeasurement();
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Wire.begin();
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 no detectado.");
    while (1);
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x3F);
  particleSensor.setPulseAmplitudeIR(0x3F);
  particleSensor.setPulseAmplitudeGreen(0);

  Serial.println("Sistema listo. Esperando botón...");
}

void loop() {
  Blynk.run();
  if (measuring) measureLoop();
}

void startMeasurement() {
  Serial.println("=== INICIANDO MEDICIÓN ===");

  measuring = true;
  measureStart = millis();

  lastBeatTime = 0;
  bpmSum = 0;
  bpmSamples = 0;

  sumRed = sumIR = 0;
  sumSqRed = sumSqIR = 0;
  sampleCount = 0;

  calidadContacto = "Bueno";
  Blynk.virtualWrite(V8, "Bueno");

  Blynk.virtualWrite(V2, "Midiendo...");
  Blynk.virtualWrite(V3, "Midiendo SpO2...");
  Blynk.virtualWrite(V4, "Midiendo BPM...");

  Blynk.virtualWrite(V7, "30s restantes");
}

void measureLoop() {
  unsigned long now = millis();
  unsigned long elapsed = now - measureStart;

  // ================= TIME RESTANTE (cada 10s) ================
  if (elapsed >= 1000 && elapsed < 2000) Blynk.virtualWrite(V7, "30s restantes");
  if (elapsed >= 10000 && elapsed < 11000) Blynk.virtualWrite(V7, "20s restantes");
  if (elapsed >= 20000 && elapsed < 21000) Blynk.virtualWrite(V7, "10s restantes");
  if (elapsed >= 29000 && elapsed < 30000) Blynk.virtualWrite(V7, "Finalizando...");

  // ================= LEER SENSOR =============================
  long ir = particleSensor.getIR();
  long red = particleSensor.getRed();

  evaluarContacto(ir, red); // Actualiza V8 si cambia

  // Continuamos medición aunque haya mal contacto

  // SpO2
  sumRed += red;
  sumIR += ir;
  sumSqRed += (double)red * red;
  sumSqIR += (double)ir * ir;
  sampleCount++;

  // BPM
  if (checkForBeat(ir)) {
    unsigned long beatTime = now;
    if (lastBeatTime > 0) {
      unsigned long delta = beatTime - lastBeatTime;
      double bpmInst = 60000.0 / delta;
      if (bpmInst > 35 && bpmInst < 180) {
        bpmSum += bpmInst;
        bpmSamples++;
      }
    }
    lastBeatTime = beatTime;
  }

  delay(20);

  // ================= FIN DE MEDICIÓN ==========================
  if (elapsed >= MEASURE_TIME_MS) {
    measuring = false;
    computeResults();
  }
}

void evaluarContacto(long ir, long red) {
  String nuevoEstado;

  if (ir < 8000 || red < 5000) {
    nuevoEstado = "Mal contacto";
  } else {
    nuevoEstado = "Bueno";
  }

  if (nuevoEstado != calidadContacto) {
    calidadContacto = nuevoEstado;
    Blynk.virtualWrite(V8, calidadContacto);
  }
}

double calcularSpO2() {
  if (sampleCount == 0) return 0;

  double meanRed = sumRed / sampleCount;
  double meanIR = sumIR / sampleCount;

  double varRed = (sumSqRed / sampleCount) - (meanRed * meanRed);
  double varIR = (sumSqIR / sampleCount) - (meanIR * meanIR);

  double acRed = sqrt(max(0.0, varRed));
  double acIR = sqrt(max(0.0, varIR));

  if (acIR <= 0 || meanIR <= 0) return 0;

  double R = (acRed / meanRed) / (acIR / meanIR);
  double spo2 = 110.0 - 25.0 * R;
  return constrain(spo2, 0, 100);
}

String analizarSpO2(double spo2) {
  if (spo2 >= 95) return "Normal";
  if (spo2 >= 90) return "Ligeramente baja";
  return "Baja";
}

String analizarBPM(double bpm) {
  if (bpm == 0) return "Sin dato BPM";
  if (bpm < 60) return "Baja";
  if (bpm <= 100) return "Normal";
  return "Alta";
}

String combinarEstados(String eS, String eB) {
  if (eS == "Normal" && eB == "Normal") return "Normal";
  if (eS != "Normal" && eB == "Normal") return "Anormal: SpO2";
  if (eS == "Normal" && eB != "Normal") return "Anormal: BPM";
  return "Anormal: SpO2 y BPM";
}

String evaluarRiesgo(double spo2, double bpm, String contacto) {
  if (contacto == "Mal contacto") return "Alto";
  if (spo2 <= 90 || bpm >= 130) return "Alto";
  if ((spo2 < 95 && spo2 > 90) || bpm > 100) return "Medio";
  return "Bajo";
}

void computeResults() {
  double bpmAvg = (bpmSamples > 0) ? bpmSum / bpmSamples : 0;
  double spo2 = calcularSpO2();

  String eSpo2 = analizarSpO2(spo2);
  String eBpm = analizarBPM(bpmAvg);
  String eGral = combinarEstados(eSpo2, eBpm);
  String riesgo = evaluarRiesgo(spo2, bpmAvg, calidadContacto);

  Blynk.virtualWrite(V0, spo2);
  Blynk.virtualWrite(V1, bpmAvg);
  Blynk.virtualWrite(V2, eGral);
  Blynk.virtualWrite(V3, eSpo2);
  Blynk.virtualWrite(V4, eBpm);
  Blynk.virtualWrite(V5, riesgo);

  Blynk.virtualWrite(V7, "Listo");

  Serial.println("===== RESULTADOS =====");
  Serial.println("SpO2: " + String(spo2));
  Serial.println("BPM: " + String(bpmAvg));
  Serial.println("Estado SpO2: " + eSpo2);
  Serial.println("Estado BPM: " + eBpm);
  Serial.println("Riesgo: " + riesgo);
  Serial.println("Estado general: " + eGral);
}
