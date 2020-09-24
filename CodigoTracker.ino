/*  Descricao:    Este codigo tem a funcao de ler os valores de tensao de 2 LDR's (Light Depedent Resistor) 
 *  e fazer com que um motor de passo 28BYJ-48 acionado com o driver ULN 2003 possa girar para
 *  posicionar uma placa fotovoltaica na direção em que ha maior incidencia de luz solar.
 *  
 *  Platatorma:   Este programa foi feito para o funcionamento na placa Arduino Mega e não é garantido que 
 *  funcionara em outro microcontrolador.
 *  
 *  Autores:
 *      - Douglas Fernandes da Costa
 *      - Larissa de Castro Braga
 *      - Lucas Coelho de Lima
 *      - Pedro de Souza Batista Campos
 *  
 *  Data:
 *      v1 - 14/11/2018: Larissa de Castro Braga 
 *      v2 - 17/11/2018: Larissa de Castro Braga
 *      v3 - 21/11/2018: Larissa de Castro Braga
 */
//========== Bibliotecas usadas ==========
#include <Stepper.h>
#include <LiquidCrystal.h>
//colocar a do display
//========================================

//=============== Defines =================
#define STEPS 2038 // the number of steps in one revolution of your motor (28BYJ-48)
#define REDUCAO 100

#define RS 38
#define E 40
#define D4 42
#define D5 44
#define D6 46
#define D7 48

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

//============= Construtores =============
Stepper stepper(STEPS, 8, 10, 9, 11); //Stepper stepper(STEPS, 8, 9, 10, 11); EH TROCADO MESMO
//========================================

//=============== Variaveis ===============
float leitura1, leitura2, leitura, passo, i;
int flag1, flag2;
//=========================================

void setup() {  
  //=============== Configuracao dos pinos ===============
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  //======================================================

  //============= Configuracao da interrupcao ============
  attachInterrupt(0,fimdecurso1,FALLING); //pino 3 - numero da interurpcao/ funcao a ser executada / quando ocorre
  attachInterrupt(1,fimdecurso2,FALLING); //pino 2 - numero da interurpcao/ funcao a ser executada / quando ocorre
  //======================================================

  //=============== Configuracao do motor ===============
  stepper.setSpeed(12); // 10 rpm
  //=====================================================

  lcd.begin(20,4);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Inicializando");
}

//=================== Interrupcoes ===================
void fimdecurso1(){
  // tem que fazer o motor da alguns passos para tras para desencostar do fim de curso
  i=passo+20;
  flag1=1;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Fim de curso 1");
  delay(1000);
}

void fimdecurso2(){
  // tem que fazer o motor da alguns passos para tras para desencostar do fim de curso
  i=passo+20;
  flag2=1;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Fim de curso 2");
  delay(1000);
}
//====================================================

void loop() {
  //===== Leitura de Vout dos 2 divisores de tensao =====
  leitura1=analogRead(A1);
  leitura2=analogRead(A0);
  
  leitura=leitura1-leitura2;
  passo=(STEPS/1023)*leitura*REDUCAO; //conversao do valor lido para numero de passos  MULTIPLICAR PELO VALOR DE REDUCAO
  
  if(passo>0) {
    //stepper.step(passo); //passo no sentido horario
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Direita");
    for(i=0;i<passo;i++){
      stepper.step(1);
    }
  }
  else{
    lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Esquerda");
    passo*=(-1);
    for(i=0;i<passo;i++){
      stepper.step(-1);
    }  
  }
  //===== O delay e para atrasar o codigo e o motor nao ficar girando toda hora sem necessidade =====
  if(flag2==1){
      passo=(6*REDUCAO);
      stepper.step(passo);
      flag2=0;
    }

  if(flag1==1){
      passo=(6*REDUCAO);
      stepper.step(-passo);
      flag1=0;
    }
  delay(500); 
}
