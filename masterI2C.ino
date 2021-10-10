// Comunicação com o Painel via I2C
// Exemplo de fragmento do código a ser executado pelo master do barramento (somente escrita no slave)

const byte endSlave = 8; //endereço do device slave
char buff[5]=""; //string de envio

#include <Wire.h>

void setup() 
{  
  Wire.begin();
}

void loop()
{
  Wire.beginTransmission(endSlave); // transmite para o device #8 (Painel)
  // simulando envio de velocidade
  Wire.write("v");        
  Wire.write(leituraVelocidade());   
  Wire.write(";"); //sinalizar fim da transmissão
  
  Wire.endTransmission();    // encerra transmissão

  delay(500); 
}

int leituraVelocidade() //simulando tratamento de dados para serem enviados via I2C
{
  // parte do codigo de leitura da velocidade
  int velocidade = 129; //exemplo

  return velocidade;  
}
