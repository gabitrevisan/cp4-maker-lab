# Arquivos de Modelagem 3D

Esta pasta contém os arquivos de modelagem e impressão do chassi personalizado. 

**Diretrizes de Projeto Adotadas:**
* **Tolerâncias:** Adicionou-se uma tolerância de +0,1 a 0,2 mm para encaixes que precisam ser muito justos, compensando imperfeições da impressão 3D.
* **Resistência:** As paredes estruturais foram projetadas com espessura mínima de 2 a 3 mm para evitar que quebrem facilmente ou empenem durante a impressão.
* **Manutenção:** O design garante acesso livre à porta USB do ESP32/Arduino, além de permitir a remoção rápida da bateria e dos motores sem a necessidade de desmontar todo o chassi.

**Arquivos:**
* `carrinho_v0.1.stl`: Versão inicial do chassi exportada para fatiamento.
* `carrinho_fonte.scad`: Código-fonte do modelo 3D (ex: OpenSCAD, Fusion360, etc.).