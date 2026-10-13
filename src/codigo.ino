#include <Arduino.h>

#include <carrinho_tinyml_inferencing.h>

// Definicao dos Pinos da Ponte H L298N (Motores)
#define IN1 27
#define IN2 26
#define ENA 14
#define IN3 33
#define IN4 25
#define ENB 32

// Definicao dos Pinos do Sensor Ultrassônico HC-SR04
#define TRIG_PIN 5
#define ECHO_PIN 18

// Configuracao do PWM para Controle de Velocidade
const int freq = 5000;
const int resolution = 8; // Faixa de 0 a 255

// Declaracao de Funcoes Auxiliares
void moverFrente(int velocidade);
void moverTras(int velocidade);
void parar();
float lerDistanciaNormalizada();

void setup()
{
    Serial.begin(115200);

    // Configuração dos Pinos dos Motores
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    // PWM para os canais ENA e ENB
    ledcAttach(ENA, freq, resolution);
    ledcAttach(ENB, freq, resolution);

    // Configuração dos Pinos do Sensor Ultrassônico
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    parar();
    Serial.println("================================================");
    Serial.println("  ESP32 Inicializado - Teste TinyML Carrinho   ");
    Serial.println("================================================");
}

void loop()
{
    // --- ETAPA 1: Leitura e Normalizacao do Dado do Sensor ---
    float dado_normalizado = lerDistanciaNormalizada();

    // --- ETAPA 2: Preparacao do Buffer de Entrada para a Inferencia ---
    signal_t signal;
    float features[1] = {dado_normalizado};

    signal.total_length = 1;
    signal.get_data = [](size_t offset, size_t length, float *out_ptr) -> int
    {
        out_ptr[0] = features[0];
        return 0;
    };

    // --- ETAPA 3: Execucao da Inferencia Embarcada ---
    ei_impulse_result_t result = {0};
    EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);

    if (res != EI_IMPULSE_OK)
    {
        Serial.printf("Erro ao executar a inferencia TinyML (Codigo: %d)\n", res);
        delay(500);
        return;
    }

    // --- ETAPA 4: Identificacao da Classe com Maior Probabilidade ---
    int classe_predita = -1;
    float maior_probabilidade = -1.0;

    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++)
    {
        if (result.classification[ix].value > maior_probabilidade)
        {
            maior_probabilidade = result.classification[ix].value;
            classe_predita = ix;
        }
    }

    String nome_classe = result.classification[classe_predita].label;

    // Imprime os resultados no Serial Monitor
    Serial.print("[TinyML] Entrada Normalizada: ");
    Serial.print(dado_normalizado, 4);
    Serial.print(" | Classe Predita: ");
    Serial.print(nome_classe);
    Serial.print(" | Confianca: ");
    Serial.println(maior_probabilidade, 2);

    // --- ETAPA 5: Acao no Carrinho com base na Classe Identificada ---
    // A tomada de decisão verifica o nome exato da classe vinda do Edge Impulse
    if (nome_classe == "LIVRE")
    {
        Serial.println("-> Acao: Caminho Livre! Avancando a 100% de velocidade.");
        moverFrente(255);
    }
    else if (nome_classe == "OBSTACULO_PROXIMO")
    {
        Serial.println("-> Acao: Obstaculo Proximo! Reduzindo velocidade para 50%.");
        moverFrente(130);
    }
    else if (nome_classe == "COLISAO_IMINENTE")
    {
        Serial.println("-> Acao: Perigo de Colisao! Parando e executando recuo.");
        parar();
        delay(150);
        moverTras(180);
        delay(500);
        parar();
    }
    else
    {
        parar();
    }

    delay(200); // Intervalo entre leituras e inferências
}

// --- FUNÇÕES AUXILIARES DE HARDWARE ---

float lerDistanciaNormalizada()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duracao = pulseIn(ECHO_PIN, HIGH, 30000);
    float distancia_cm = duracao * 0.034 / 2.0;

    if (distancia_cm == 0 || distancia_cm > 400)
    {
        distancia_cm = 100.0; // Distância máxima padrão
    }

    // Escala de normalização (0 a 100 cm -> 0.0 a 1.0)
    float normalizado = distancia_cm / 100.0;
    if (normalizado > 1.0)
        normalizado = 1.0;
    if (normalizado < 0.0)
        normalizado = 0.0;

    return normalizado;
}

void moverFrente(int velocidade)
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    ledcWrite(ENA, velocidade);
    ledcWrite(ENB, velocidade);
}

void moverTras(int velocidade)
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    ledcWrite(ENA, velocidade);
    ledcWrite(ENB, velocidade);
}

void parar()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    ledcWrite(ENA, 0);
    ledcWrite(ENB, 0);
}