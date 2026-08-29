''' este é o código para implementar o Controle Sem Fio via ESP32 e BLE(Bluetooth Low Energy) usando o aplicativo Dabble.
'''

#include <DabbleESP32.h>

// Pinos da Ponte H L298N
#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19
#define ENA 5
#define ENB 6

// Pinos do Sensor Ultrassônico HC-SR04
#define TRIG_PIN 32
#define ECHO_PIN 33

    // Variáveis de leitura do sensor
    long duration;
int distance;
const int DISTANCIA_SEGURA = 15; // Distância mínima de parada em cm

void setup()
{
    Serial.begin(115200);

    // Configuração dos pinos da Ponte H como saída
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    // Configuração dos pinos do Sensor Ultrassônico
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Inicializa a comunicação Bluetooth (BLE)
    Dabble.begin("My Bluetooth Car");
}

void loop()
{
    Dabble.processInput();

    // 1. Faz a leitura da distância do obstáculo frontal
    distance = medirDistancia();

    // 2. Verifica os comandos do aplicativo (GamePad) e aplica a trava de segurança
    if (GamePad.isUpPressed())
    {
        // Só permite andar para frente se o caminho estiver livre (>15cm) ou se a leitura falhar (0)
        if (distance > DISTANCIA_SEGURA || distance == 0)
        {
            moveForward();
        }
        else
        {
            stopMotors();
            Serial.println("Obstáculo detectado! Trava de segurança ativada.");
        }
    }
    else if (GamePad.isDownPressed())
    {
        moveBackward();
    }
    else if (GamePad.isLeftPressed())
    {
        turnLeft();
    }
    else if (GamePad.isRightPressed())
    {
        turnRight();
    }
    else
    {
        stopMotors();
    }

    delay(50); // Atraso curto para estabilizar a leitura do sensor
}

// Sub-rotina para medir a distância com o HC-SR04
int medirDistancia()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Lê o tempo de retorno do eco
    duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout em 30ms (alcance máximo de ~5m)

    if (duration == 0)
        return 0; // Se não houver eco

    // Cálculo: Velocidade do som é ~340 m/s ou 0.034 cm/us. Divide por 2 (ida e volta).
    return duration * 0.034 / 2;
}

// Sub-rotinas de Movimentação
void moveForward()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}
void moveBackward()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}
void turnLeft()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}
void turnRight()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
void stopMotors()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}