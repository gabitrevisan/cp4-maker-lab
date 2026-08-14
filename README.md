# Projeto Start-up One — Carrinho-Robô Autônomo / Controlado

## 👥 Integrantes do Grupo
* **Gabriela Trevisan** - RM99500
* **Breno Silva** - RM99275
* **Gustavo Akio** - RM550241
* **Rafael Franck** - RM550875
* **Eduardo Araujo** - RM99758

---

## 📋 Ficha de Requisitos Técnicos

### 1. Dimensões e Estrutura Esperadas
* **Comprimento:** 200 mm
* **Largura:** 135 mm (incluindo as rodas)
* **Altura Total (com Carenagem):** 75 mm
* **Distância do Solo (Ground Clearance):** 15 mm
* **Método de Fabricação:** Impressão 3D em filamento PLA
* **Conceito Estrutural:** Fixação modular prioritariamente por encaixes por pressão (*press-fit* / guias de trilho), eliminando a dependência de parafusos.

### 2. Atuadores e Mecânica
* **Quantidade de Motores:** 2x Motores DC 3V–6V com caixa de redução
* **Rodas de Tração:** 2x Rodas emborrachadas (65 mm de diâmetro) montadas nas laterais
* **Apoio Direcional:** 1x Roda Boba (Caster Ball) frontal fixada por encaixe inferior
* **Driver dos Motores:** Módulo Ponte H L298N (controle de sentido e velocidade via PWM)

### 3. Sistema de Controle e Eletrônica
* **Placa Controladora:** ESP32 DevKit V1 (30 pinos)
  * Processamento de comandos e leitura de sensores
  * Conectividade sem fio nativa (Bluetooth / Wi-Fi) para telemetria e controle
* **Sensores:** 1x Sensor Ultrassônico HC-SR04 posicionado no parachoque dianteiro para desvio de obstáculos
* **Alimentação do Sistema:**
  * Suporte para 4 pilhas AA (6V) conectado ao driver e regulado para o ESP32
  * Chave gangorra liga/desliga inserida no painel traseiro do chassi

### 4. Distribuição Espacial e Posicionamento dos Componentes
* **Região Traseira / Inferior:** Alojamento para o pack de baterias/pilhas para manter o centro de gravidade baixo e compartimento para a chave geral.
* **Região Central / Superior:** Encaixes sob medida para o microcontrolador ESP32 e a Ponte H, permitindo fácil cabeamento e ventilação.
* **Região Frontal:** Suporte inferior para a roda boba e suporte vertical na carenagem para o sensor ultrassônico voltado para frente.

### 5. Carenagem (Cobertura)
* **Design:** Cobertura aerodinâmica removível acoplada ao chassi principal via travas laterais (*snap-fit*).
* **Aberturas Estratégicas:**
  * Dois orifícios frontais de 16 mm para os emissores do sensor ultrassônico
  * Recorte superior/traseiro para acesso ao interruptor de energia
  * Abertura para conexão do cabo micro-USB sem necessidade de desmontar a estrutura

---

## 📐 Croqui do Chassi e Carenagem

### 1. Vista Superior — Disposição dos Componentes e Encaixes
*Visualize o Croqui Chassi Superior no link abaixo*
![Croqui Chassi Superior](docs/croqui-chassi-superior.png)

### 2. Vista Isométrica / Modelo com Carenagem
*Visualize o Croqui Carenagem Superior no link abaixo*
![Croqui Carenagem](docs/croqui-carenagem.png)

---

## 🛠️ Lista de Componentes (BOM)

| Item | Componente | Quantidade | Função |
| :---: | :--- | :---: | :--- |
| 1 | Placa de Desenvolvimento ESP32 | 1 | Microcontrolador central e comunicação Bluetooth |
| 2 | Driver de Motor Ponte H L298N | 1 | Controle dos motores DC |
| 3 | Motor DC com Caixa de Redução | 2 | Tração das rodas |
| 4 | Roda com pneu de borracha (65mm) | 2 | Rodas motrizes |
| 5 | Roda Boba (Caster Wheel) | 1 | Ponto de apoio e manobra frontal |
| 6 | Sensor Ultrassônico HC-SR04 | 1 | Detecção de obstáculos |
| 7 | Chave Gangorra Liga/Desliga | 1 | Interruptor geral do circuito |
| 8 | Suporte de 4 pilhas AA | 1 | Fonte de alimentação do robô |
| 9 | Conjunto Chassi + Carenagem (PLA 3D) | 1 | Estrutura mecânica modular com encaixes |

---

## 📅 Roadmap de Desenvolvimento
- [x] Definição de requisitos, dimensões e arquitetura eletrônica
- [x] Elaboração dos croquis digitais do chassi e carenagem
- [ ] Modelagem 3D final (arquivos `.STL`) para impressão
- [ ] Teste de encaixe dos motores e eletrônica
- [ ] Programação do firmware no ESP32 (Lógica de movimentação + Sensor)