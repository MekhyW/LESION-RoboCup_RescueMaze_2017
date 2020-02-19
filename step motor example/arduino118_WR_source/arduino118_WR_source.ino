/* ========================================================================================================
 
   Curso de Arduino e AVR 118
   
   WR Kits Channel


   Controle de Motor de Passo Unipolar com Biblioteca Stepper

    
   Autor: Eng. Wagner Rambo  Data: Maio de 2017
   
   www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits | dailymotion.com/wrkits
   
======================================================================================================== */


// ========================================================================================================
// --- Bibliotecas Auxiliares ---
#include <Stepper.h>   //biblioteca para controle de motor de passo


// ========================================================================================================
// --- Mapeamento de Hardware ---
#define   in1   8      //entrada 1 do ULN2003
#define   in2   9      //entrada 2 do ULN2003
#define   in3  10      //entrada 3 do ULN2003
#define   in4  11      //entrada 4 do ULN2003


// ========================================================================================================
// --- Constantes Auxiliares ---
const int stepsPerRevolution = 4096;  // change this to fit the number of steps per revolution


// ========================================================================================================
// --- Declaração de Objetos ---
Stepper myStepper(stepsPerRevolution, in1,in3,in2,in4); 



// ========================================================================================================
// --- Configurações Iniciais ---
void setup() 
{
    // set the speed at 60 rpm:
  myStepper.setSpeed(30);
  

} //end setup


// ========================================================================================================
// --- Loop Infinito ---
void loop() 
{
  
  
  myStepper.step(stepsPerRevolution);
  


/*
  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(1500);
  
*/
} //end loop


 
  













