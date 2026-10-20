// ==============================================================================
// Projeto: Carenagem Funcional do Carrinho-Robô 2WD (Modelo B)
// Autores: Veela - StartUp ONE
// Curso: Engenharia de Software - FIAP
// ==============================================================================

// Parâmetros Gerais e Diretrizes (Conforme PDF Aula 14)
espessura_parede = 3;   // Espessura mínima para evitar empenamento e quebra
tolerancia = 0.2;       // Tolerância para encaixe rápido (+0.2mm)
furo_m3 = 3.4;          // Furo com folga testada para parafusos M3 (3.2 - 3.4mm)

// Dimensões da Carenagem Baseadas no Chassi de Acrílico (Valores em mm)
comp_chassi = 210;
largura_chassi = 140;
altura_carenagem = 55;

module carenagem_funcional() {
    difference() {
        // 1. Corpo principal (Bloco sólido da carenagem)
        cube([comp_chassi, largura_chassi, altura_carenagem]);

        // 2. Oco interno (Subtrai um bloco menor para deixar a parede com 3mm)
        translate([espessura_parede, espessura_parede, -espessura_parede])
            cube([
                comp_chassi - (2 * espessura_parede), 
                largura_chassi - (2 * espessura_parede), 
                altura_carenagem
            ]);

        // 3. Recorte Frontal: Sensor Ultrassônico HC-SR04
        // Furos de 16mm (+ tolerância) com espaçamento centralizado
        translate([comp_chassi, (largura_chassi/2) - 13, altura_carenagem/2])
            rotate([0, 90, 0]) 
            cylinder(h=20, d=16 + tolerancia, center=true, $fn=50);
            
        translate([comp_chassi, (largura_chassi/2) + 13, altura_carenagem/2])
            rotate([0, 90, 0]) 
            cylinder(h=20, d=16 + tolerancia, center=true, $fn=50);

        // 4. Recorte Superior (Traseira): Acesso à Bateria Removível
        translate([20, (largura_chassi/2) - 35, altura_carenagem - 10])
            cube([70, 70, 20]);

        // 5. Recorte Superior (Frente): Acesso à porta USB do ESP32
        translate([110, (largura_chassi/2) - 25, altura_carenagem - 10])
            cube([50, 50, 20]);

        // 6. Recorte Lateral: Motores Expostos para Manutenção Fácil
        // Lado Esquerdo
        translate([30, -10, -10])
            cube([65, espessura_parede + 15, 45]);
        // Lado Direito
        translate([30, largura_chassi - espessura_parede - 5, -10])
            cube([65, espessura_parede + 15, 45]);
            
        // 7. Furos M3 para fixação da carenagem na base de acrílico
        // Furo Traseiro Esquerdo
        translate([15, 15, 0]) 
            cylinder(h=20, d=furo_m3, center=true, $fn=30);
        // Furo Traseiro Direito
        translate([15, largura_chassi - 15, 0]) 
            cylinder(h=20, d=furo_m3, center=true, $fn=30);
        // Furo Dianteiro Esquerdo
        translate([comp_chassi - 20, 15, 0]) 
            cylinder(h=20, d=furo_m3, center=true, $fn=30);
        // Furo Dianteiro Direito
        translate([comp_chassi - 20, largura_chassi - 15, 0]) 
            cylinder(h=20, d=furo_m3, center=true, $fn=30);
    }
}

// cad/carrinho_fonte.scad

// --- Parâmetros Gerais ---
chassi_comprimento = 150;
chassi_largura = 100;
espessura = 3;
raio_furo_motor = 1.5;

module chassi_base() {
    difference() {
        // Base do carrinho
        cube([chassi_comprimento, chassi_largura, espessura], center = true);
        
        // Furações de fixação dos motores
        translate([30, 40, 0]) cylinder(h = espessura + 2, r = raio_furo_motor, center = true, $fn=20);
        translate([30, -40, 0]) cylinder(h = espessura + 2, r = raio_furo_motor, center = true, $fn=20);
        translate([-30, 40, 0]) cylinder(h = espessura + 2, r = raio_furo_motor, center = true, $fn=20);
        translate([-30, -40, 0]) cylinder(h = espessura + 2, r = raio_furo_motor, center = true, $fn=20);
        
        // Recorte para passagem de fiação
        translate([0, 0, 0]) cube([20, 40, espessura + 2], center = true);
    }
}

// Renderiza a peça final
carenagem_funcional();

chassi_base();