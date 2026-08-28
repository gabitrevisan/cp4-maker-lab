# Arquitetura Eletrônica e Diagrama de Blocos

A organização do hardware e o fluxo de alimentação do carrinho-robô seguem a seguinte arquitetura técnica:

* **Alimentação:** A energia flui inicialmente da Bateria para a Chave (liga/desliga).
* **Distribuição de Potência:** A partir da chave, a tensão é direcionada simultaneamente para a Ponte H e para o regulador de tensão do ESP32.
* **Controle:** A placa controladora ESP32 processa as instruções enviadas pelo módulo Bluetooth e gerencia a telemetria do Sensor Ultrassônico.
* **Atuação:** A placa controladora envia os sinais lógicos para a Ponte H, que por sua vez aciona e controla a direção e velocidade dos Motores.