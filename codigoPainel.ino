/*
 *  Código de funcionamento do Painel elétrico do J12
 *  Versão 0.9
 *  Criado em 14/09 por Alexandre Chicora
 *  Input: I2C ECU Central
 *  Output: Barras de LED e display 7 segmentos para visualização dos dados do carro pelo piloto
 *  Repositório: github.com/
 */

#include <Wire.h> //biblioteca que implementa a I2C no arduino

// Pinos conectados nos transistores que fecham o circuito das barras de LED: RPM e combustível
#define BAR_RPM PA11
#define BAR_CMB PA10
// Pinos conectados no CI 4514 (Multiplexador das Barras de LED: RPM e combustível)
#define MULT_A0 PB12
#define MULT_A1 PB13
#define MULT_A3 PB14
#define MULT_A2 PB15
// Pinos conectados no CI 7447 (Codificação do display de 7 segmentos)
#define CI_DISPB PB1
#define CI_DISPC PB0
#define CI_DISPD PA7
#define CI_DISPA PA6
// Pinos que fecham o circuito do transistor dos displays de 7 segmentos (velocidade)
#define VEL_DISP1 PA8 //dezenas
#define VEL_DISP2 PA9 //unidades
// Pinos dos LED's individuais
#define LED1 PC15
#define LED2 PC14
#define LED3 PC13
#define LED4 PB3
#define LED5 PB4
#define LED6 PB5

char buf[6] = ""; //buffer de recebimento das mensagens via I2C
const byte endSlave = 8; //endereço do slave para o qual a I2C enviara os dados
bool flagBuffer = LOW;
int delayVeloc = 4; //tempo que o display de velocidade fica ligado
int delayBarLED = 2; //tempo de delay entre os acionamentos das barras de LED (combustivel e rpm)

void setup() 
{
  Serial.begin(9600);
  Wire.begin(endSlave);    //testar sem input
  Wire.onRequest(inputI2C);//funcao alvo que sera executada quando o master enviar dados
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

void loop() 
{
  if(flagBuffer == HIGH)  //se for recebido dado na I2C
  {
    tratamentoBuf(buf[0]);  //funcao invocada para tratar o dado recebido
  }

}

void inputI2C(int nEventos) //parâmetro de quantos data frames foram enviados
{
  for (int i = 0; i < nEventos; i++)
  {
    buf[i] = Wire.read();  //copia para o buffer enquanto tiver dados sendo transmitidos
  }
  flagBuffer = HIGH; // sinaliza que novos dados foram recebidos e a recepção foi finalizada
}

void disp_velocidade()
{
  char bufAux[N]; //buffer auxiliar para copiar os dados do buffer original sem altera-lo
  int i = 1; //começa na segunda posição da string pois o primeiro dado já foi lido
  int j = 0; 
  
  while(buf[i] =! ';') //leitura dos dados até o final da string
    {
      bufAux[j] = buf[i];
      j++;
      i++;
    }
    
 }

void tratamentoBuf(char destino)
{
  char bufAux[N]; //buffer auxiliar para copiar os dados do buffer original sem altera-lo
  int i = 1; //começa na segunda posição da string pois o primeiro dado já foi lido
  int j = 0; 
  int valorLido = 0;
  
  while(buf[i] =! ';') //leitura dos dados até o final da string
    {
      bufAux[j] = buf[i];
      j++;
      i++;
    }
    
  valorLido = atoi(BufAux); //conversão do numero lido de string para inteiro

  switch(destino) //leitura de acordo com a codificação;
    case 'v':
      disp_velocidade(valorLido); 
      break;
    case 'r':
      disp_rpm(valorLido);
      break;
    case 'l':
      disp_led(valorLido);
      break;
    case 'c':
      disp_cmb(valorLido);
      break;
    default
      //caracter invalido
      imprimeBuffer();
      break;
}

void disp_velocidade(int velocidade)
{
    int unidade = velocidade % 10; //separar o valor recebido em dezena e unidade
    int dezena = velocidade / 10;
    
    digitalWrite(VEL_DISP2, LOW);  //desativa o transistor que fecha o circuito do display da unidade
    digitalWrite(VEL_DISP1, HIGH); //ativa o transistor que fecha o circuito do display da dezena
    codificacaoDisplay(dezena);
    delay(delayVeloc);
    
    digitalWrite(VEL_DISP1, LOW);   //desativa o transitor do display das dezenas
    digitalWrite(VEL_DISP2, HIGH);  //ativa o transistor do display das unidades
    codificacaoDisplay(unidade);
    delay(delayVeloc);              //intervalo que ficara ligado
    
    digitalWrite(VEL_DISP2, LOW);
}

void codificacaoDisplay(int valor)  //codificacao do display no CI 7447 de acordo com o valor recebido individualmente
{                                   // tabela verdade no datasheet do CI 7447
  switch(valor)
    case 0:
        digitalWrite(CI_DISPA, LOW);
        digitalWrite(CI_DISPB, LOW);
        digitalWrite(CI_DISPC, LOW);
        digitalWrite(CI_DISPD, LOW);
        break;
    case 1:
        digitalWrite(CI_DISPA, HIGH);
        digitalWrite(CI_DISPB, LOW);
        digitalWrite(CI_DISPC, LOW);
        digitalWrite(CI_DISPD, LOW);
        break; 
    case 2:
        digitalWrite(CI_DISPA, LOW);
        digitalWrite(CI_DISPB, HIGH);
        digitalWrite(CI_DISPC, LOW);
        digitalWrite(CI_DISPD, LOW);
        break;  
    case 3:
        digitalWrite(CI_DISPA, HIGH);
        digitalWrite(CI_DISPB, HIGH);
        digitalWrite(CI_DISPC, LOW);
        digitalWrite(CI_DISPD, LOW);
        break;
    case 4:
        digitalWrite(CI_DISPA, LOW);
        digitalWrite(CI_DISPB, LOW);
        digitalWrite(CI_DISPC, HIGH);
        digitalWrite(CI_DISPD, LOW);
        break;
    case 5:
        digitalWrite(CI_DISPA, HIGH);
        digitalWrite(CI_DISPB, LOW);
        digitalWrite(CI_DISPC, HIGH);
        digitalWrite(CI_DISPD, LOW);
        break;
    case 6:
        digitalWrite(CI_DISPA, LOW);
        digitalWrite(CI_DISPB, HIGH);
        digitalWrite(CI_DISPC, HIGH);
        digitalWrite(CI_DISPD, LOW);
        break;
   case 7:
        digitalWrite(CI_DISPA, HIGH);
        digitalWrite(CI_DISPB, HIGH);
        digitalWrite(CI_DISPC, HIGH);
        digitalWrite(CI_DISPD, LOW);
        break;
   case 8:
        digitalWrite(CI_DISPA, LOW);
        digitalWrite(CI_DISPB, LOW);
        digitalWrite(CI_DISPC, LOW);
        digitalWrite(CI_DISPD, HIGH);
        break;
  case 9:
        digitalWrite(CI_DISPA, HIGH);
        digitalWrite(CI_DISPB, LOW);
        digitalWrite(CI_DISPC, LOW);
        digitalWrite(CI_DISPD, HIGH);
        break;
  default:
        break;
}

void disp_rpm(int nroLED)
{
  digitalWrite(BAR_CMB, LOW);
  digitalWrite(BAR_RPM, HIGH); //transistor do acionamento da barra de leds do RPM
  barraLeds(nroLED); //valor de 1 a 12 correspondente ao led que será acendido
  delay(delayBarLED);
}

void disp_cmb(int nroLED)
{
  digitalWrite(BAR_RPM, LOW);
  digitalWrite(BAR_CMB, HIGH);
  barraLeds(nroLED); //valor de 1 a 12 correspondente ao led que será acendido
  delay(delayBarLED);
}

void barraLeds(int nroLed)  //acende o respectivo led na barra de LED de acordo com a tabela de multiplexação do CI 4514
{                           //atentar para o mapeamento dos pinos da placa e no datasheet do CI (não são os mesmos)
  switch(nroLed)   
    case 12:    //acender o primeiro LED (mais alto da barra), mapeada no Q7 da placa
      digitalWrite(MULT_A0, HIGH);
      digitalWrite(MULT_A1, HIGH);
      digitalWrite(MULT_A2, HIGH);
      digitalWrite(MULT_A3, LOW);
      break;
    case 11:    //mapeado no Q6 da placa
      digitalWrite(MULT_A0, LOW);
      digitalWrite(MULT_A1, HIGH);
      digitalWrite(MULT_A2, HIGH);
      digitalWrite(MULT_A3, LOW);
      break;
    case 10:    //mapeado no Q5 da placa
      digitalWrite(MULT_A0, HIGH);
      digitalWrite(MULT_A1, LOW);
      digitalWrite(MULT_A2, HIGH);
      digitalWrite(MULT_A3, LOW);
      break;
    case 9:    //mapeado no Q4 da placa
      digitalWrite(MULT_A0, LOW);
      digitalWrite(MULT_A1, LOW);
      digitalWrite(MULT_A2, HIGH);
      digitalWrite(MULT_A3, LOW);
      break;
    case 8:    //mapeado no Q3 da placa
      digitalWrite(MULT_A0, HIGH);
      digitalWrite(MULT_A1, HIGH);
      digitalWrite(MULT_A2, LOW);
      digitalWrite(MULT_A3, LOW);
      break;
   case 7:    //mapeado no Q1 da placa
      digitalWrite(MULT_A0, HIGH);
      digitalWrite(MULT_A1, LOW);
      digitalWrite(MULT_A2, LOW);
      digitalWrite(MULT_A3, LOW);
      break;
   case 6:    //mapeado no Q2 da placa
      digitalWrite(MULT_A0, LOW);
      digitalWrite(MULT_A1, HIGH);
      digitalWrite(MULT_A2, LOW);
      digitalWrite(MULT_A3, LOW);
      break;
   case 5:    //Q13 da placa
      digitalWrite(MULT_A0, HIGH);
      digitalWrite(MULT_A1, LOW);
      digitalWrite(MULT_A2, HIGH);
      digitalWrite(MULT_A3, HIGH);
      break;
   case 4:    //Q12 da placa
      digitalWrite(MULT_A0, LOW);
      digitalWrite(MULT_A1, LOW);
      digitalWrite(MULT_A2, HIGH);
      digitalWrite(MULT_A3, HIGH);
      break;
   case 3:    //Q15 da placa
      digitalWrite(MULT_A0, HIGH);
      digitalWrite(MULT_A1, HIGH);
      digitalWrite(MULT_A2, HIGH);
      digitalWrite(MULT_A3, HIGH);
      break;
   case 2:    //Q9 da placa
      digitalWrite(MULT_A0, HIGH);
      digitalWrite(MULT_A1, LOW);
      digitalWrite(MULT_A2, LOW);
      digitalWrite(MULT_A3, HIGH);
      break;  
   case 1:    //mapeado no Q14 da placa
      digitalWrite(MULT_A0, HIGH);
      digitalWrite(MULT_A1, HIGH);
      digitalWrite(MULT_A2, HIGH);
      digitalWrite(MULT_A3, LOW);
      break;
}

void disp_led(int nLED)
{ 
  int nLED1 = 0, nLED2 = 0;
  nLED1 = nLED / 10; //primeiro bit ; nro do led que irá acender ou apagar
  nLED2 = nLED % 10; //segundo bit ; se for igual a 1 acende, se for igual a 0 apaga
  
  if(nLED2 =! 0 || nLED2 =! 1)
    imprimeBuffer(); //erro
    
  
  switch(nLED1) //qual led irá acender ou apagar
    case 1:
      digitalWrite(LED1, nLED2);
      break;
    case 2:
      digitalWrite(LED2, nLED2);
      break;
    case 3:
      digitalWrite(LED3, nLED2);
      break;  
    case 4:
      digitalWrite(LED4, nLED2);
      break;      
    case 5:
      digitalWrite(LED5, nLED2);
      break;  
    case 6:
      digitalWrite(LED6, nLED2);
      break;
}


void imprimeBuffer() //funcao teste para verificar funcionamento do código
{
  while(buf[i] =! ';') //leitura dos dados até o final da string
    {
      Serial.print(buff[i]);
      Serial.println();
    }  
}
