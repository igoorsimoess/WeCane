#include <Servo.h>

#define TRIG 12  //Conexao da porta TRIG do ultrassonico
#define ECHO 11 //Conexao da porta ECHO do ultrassonico
#define QUANT_SERVO 3 //Quantidade de microservos utilizados

#define MENP_SERVO 2 /*Valor da porta de menor numero*/
                     /*em que um servo esta conectado*/
                     /*(admite-se   que   os   servos*/
                     /*estao conectados em sequencia)*/

#define DIST_ON 320 /*Distancia em que o primeiro motor*/
                    /*sera acionado                    */

#define CONST_DIST  0.017175 /*Constante  utilizada  na */
                             /*conversao   do   valor   */
                             /*detectado do ultrassonico*/
                             /*para a distancia (em cm) */

#define DIF_ESPACO 100 /*Diferenca de espaco para*/
                       /*acionamento dos motores */

unsigned int duracao;
unsigned int distancia;   //Distancia detectada
unsigned int quant;       //Numero de deteccoes proximas (<= DIST_ON)
unsigned int menorDist;   //Menor distancia detectada 
Servo servo[QUANT_SERVO]; /*Array com a quantidade de servos   */
                          /*(cada indice eh um servo diferente)*/
unsigned int i;

void setup(){
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  
  for (i = 0; i < QUANT_SERVO; i++){
    servo[i].attach(i+MENP_SERVO);
    servo[i].write(0);
  }
  
  Serial.begin(9600);
}

void loop()
{
  //Retorno dos motores para posicao inicial
  for (i = 0; i < QUANT_SERVO; i++){
    servo[i].write(0);
  }
  delay(120);
  
  
  ///////Leitura de distancia e impressao no Monitor serial//////
  quant = 0;
  for (i = 0; i < 20; i++){
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duracao = pulseIn(ECHO, HIGH);
    distancia = duracao * CONST_DIST;
    if (distancia <= DIST_ON){
      quant++;
    }
    
    if (i == 0){
      menorDist = distancia;
    }else if (distancia < menorDist){
      menorDist = distancia;
    }
    
    //Serial.print(distancia);
    //Serial.println("cm");
  }
  ///////////////////////////////////////////////////////////////
  
  //Movimentacao dos motores, caso necessario
  for (i = 0; i < QUANT_SERVO; i++)
  {
    if((menorDist <= DIST_ON - (i*DIF_ESPACO)) && (quant >= 10)){
      servo[i].write(90);
    }
  }


  delay(120); //delay para permitir o movimento dos motores
}