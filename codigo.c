#include <Ultrasonic.h>

Ultrasonic ultrassom(8, 7);
Ultrasonic ultrassom2(5, 4);

int distanciaDeteccao = 70;

int contadorEntradas = 0;
int contadorSaidas = 0;
int pessoasPresentes = 0;

int estadoSistema = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("=============================================");
  Serial.println("Sistema de Contagem de Fluxo Iniciado");
  imprimirStatus();
}

void loop() {
  long distancia = ultrassom.Ranging(CM);
  long distancia2 = ultrassom2.Ranging(CM);

  switch (estadoSistema) {
    case 0:
      if (distancia < distanciaDeteccao && distancia2 > distanciaDeteccao) {
        estadoSistema = 1;
        Serial.println("--> Movimento de entrada detectado. Aguardando confirmação...");
      } else if (distancia2 < distanciaDeteccao && distancia > distanciaDeteccao) {
        estadoSistema = 2;
        Serial.println("<-- Movimento de saída detectado. Aguardando confirmação...");
      }
      break;

    case 1:
      if (distancia2 < distanciaDeteccao) {
        contadorEntradas++;
        pessoasPresentes = contadorEntradas - contadorSaidas;
        Serial.println("*");
        Serial.println("   ENTRADA CONFIRMADA  ");
        imprimirStatus();
        estadoSistema = 3;
      } else if (distancia > distanciaDeteccao) {
        Serial.println("-> Passagem de entrada cancelada. Rearmando.");
        estadoSistema = 0;
      }
      break;

    case 2:
      if (distancia < distanciaDeteccao) {
        contadorSaidas++;
        pessoasPresentes = contadorEntradas - contadorSaidas;
        if (pessoasPresentes < 0) {
          pessoasPresentes = 0;
        }
        Serial.println("*");
        Serial.println("    SAÍDA CONFIRMADA   ");
        imprimirStatus();
        estadoSistema = 3;
      } else if (distancia2 > distanciaDeteccao) {
        Serial.println("<- Passagem de saída cancelada. Rearmando.");
        estadoSistema = 0;
      }
      break;

    case 3:
      if (distancia > distanciaDeteccao && distancia2 > distanciaDeteccao) {
        Serial.println("Área livre. Sistema pronto para nova contagem.");
        Serial.println("---------------------------------------------");
        estadoSistema = 0;
      }
      break;
  }

  delay(100);
}

void imprimirStatus() {
  Serial.print("   ENTRADAS: ");
  Serial.print(contadorEntradas);
  Serial.print(" | SAÍDAS: ");
  Serial.print(contadorSaidas);
  Serial.print(" | PRESENTES: ");
  Serial.println(pessoasPresentes);
  Serial.println("*");
}
