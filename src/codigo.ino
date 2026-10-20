// src/codigo.ino

#include <Arduino.h>

// nome exato da biblioteca gerada pelo Edge Impulse
#include <carrinho-tinyml_inferencing.h>

// Definição dos pinos dos motores (Ponte H)
#define IN1 32
#define IN2 33
#define IN3 25
#define IN4 26
// #define ENA 6
// #define ENB 9

// Definição dos pinos do Sensor Ultrassônico
#define TRIG_PIN 22
#define ECHO_PIN 23

// Buffer para o TinyML
static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

void setup() {
    Serial.begin(115200);
    
    // Configuração dos Pinos
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    // pinMode(ENA, OUTPUT);
    // pinMode(ENB, OUTPUT);
    
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Serial.println("Sistema inicializado.");
}

float lerDistancia() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long duracao = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout em 30ms
    if (duracao == 0) return 400.0; // Sem eco = distância máxima
    
    return (duracao * 0.0343) / 2.0;
}

void moverFrente(uint8_t velocidade) {
    // analogWrite(ENA, velocidade);
    // analogWrite(ENB, velocidade);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void parar() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void desviar() {
    // Exemplo simples de curva para a direita
    // analogWrite(ENA, 150);
    // analogWrite(ENB, 150);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(500);
}

void loop() {
    // 1. Coleta dados para o buffer do modelo
    for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix++) {
        features[ix] = lerDistancia();
        delay(EI_CLASSIFIER_INTERVAL_MS);
    }

    // 2. Prepara a estrutura do sinal
    signal_t signal;
    int err = numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
        Serial.print("Erro ao criar sinal para o classificador: ");
        Serial.println(err);
        return;
    }

    // 3. Executa a inferência TinyML
    ei_impulse_result_t result = { 0 };
    err = run_classifier(&signal, &result, false);
    if (err != EI_IMPULSE_OK) {
        Serial.print("Erro ao executar classificador: ");
        Serial.println(err);
        return;
    }

    // 4. Tomada de decisão com base nas classes treinadas
    // Exemplo considerando as classes: "LIVRE" e "OBSTACULO"
    float pred_obstaculo = 0.0;
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        if (strcmp(result.classification[ix].label, "OBSTACULO") == 0) {
            pred_obstaculo = result.classification[ix].value;
        }
    }

    if (pred_obstaculo > 0.7) {
        Serial.println("Decisão TinyML: Obstáculo detectado. Desviando...");
        parar();
        delay(200);
        desviar();
    } else {
        Serial.println("Decisão TinyML: Caminho livre.");
        moverFrente(180);
    }
}
