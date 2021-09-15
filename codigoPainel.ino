/*
 *  Código de funcionamento do Painel elétrico do J12
 *  Versão 0.9
 *  Atualizado em 15/09 por Alexandre Chicora
 *  Input: I2C ECU Central
 *  Output: Barras de LED e display 7 segmentos para visualização dos dados do carro pelo piloto
 *  Repositório: github.com/
 */

#include <Wire.h>

// Pinos conectados nos transistores que fecham o circuito das barras de LED: RPM e combustível
#define BAR_RPM 45
#define BAR_CMB 46
// Pinos conectados no CI 4514 (Multiplexador das Barras de LED: RPM e combustível)
#define MULT_A0 25
#define MULT_A1 26
#define MULT_A3 27
#define MULT_A2 28
// Pinos conectados no CI 7447 (Codificação do display de 7 segmentos)
#define CI_DISPB 19
#define CI_DISPC 18
#define CI_DISPD 17
#define CI_DISPA 16
// Pinos que fecham o circuito do transistor dos displays de 7 segmentos (velocidade)
#define VEL_DISP1 29
#define VEL_DISP2 30
// Pinos dos LED's individuais
#define LED1 4
#define LED2 3
#define LED3 2
#define LED4 39
#define LED5 40
#define LED6 41

#define N 10 //tamanho da string lida pela I2C


void setup() 
{
  Serial.begin(9600);
  Wire.begin();    //testar sem input
  Wire.onReceive(inputI2C);

  pinMode(BAR_RPM, OUTPUT);
  pinMode(BAR_CMB, OUTPUT);
  pinMode(MULT_A0, OUTPUT);
  pinMode(MULT_A1, OUTPUT);
  pinMode(MULT_A2, OUTPUT);
  pinMode(MULT_A3, OUTPUT);
  pinMode(CI_DISPB, OUTPUT);
  pinMode(CI_DISPA, OUTPUT);
  pinMode(CI_DISPD, OUTPUT);
  pinMode(CI_DISPC, OUTPUT);
  pinMode(VEL_DISP1, OUTPUT); //dezenas
  pinMode(VEL_DISP2, OUTPUT); //unidades
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);

}

void loop() {
}


void inputI2C()
{
  
  char leituraI2C[N] = Wire.read();
  
  switch(leituraI2C[0]) //leitura de acordo com a codificação
    case 'v':
      disp_velocidade(); //mandar a string inteira - primeiro caractere
      break;
    case 'r':
      disp_rpm();
      break;
    case 'l':
      disp_led();
      break;
    case 'c':
      disp_cmb();
      break;
    default
      break;
      
  Serial.println(leituraI2C); //para fins de debug
}

void disp_velocidade(leituraI2C)
{
  for(int i = 0; i < 2; i++) //usar string to int?
  {
    switch(leituraI2C[var])
      case '1':
      vel_multiplex(1,i);
      break;
      case '2':
      vel_multiplex(2,i);
      break;
      case '3':
      vel_multiplex(3,i);
      break;
      case '4':
      vel_multiplex(4,i);
      break;
      case '5':
      vel_multiplex(5,i);
      break;
      case '6':
      vel_multiplex(6,i);
      break;
      case '7':
      vel_multiplex(7,i);
      break;
      case '8':
      vel_multiplex(8,i);
      break;
      case '9':
      vel_multiplex(9,i);
      break;
      case '0':
      vel_multiplex(0,i);
      break;
  }
  
}

// Função que faz o acionamento da velocidade

void vel_multiplex(int valor, char variavel) // variavel 'u' -> unidade , variavel 'd' -> dezena
{
  // fazer mapeamento do display de 7 segmentos
  
}
