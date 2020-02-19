/* ========================================================================================================
 
   Curso de Arduino e AVR 123
   
   WR Kits Channel


   Controle de Motor com Encoder

    
   Autor: Eng. Wagner Rambo  Data: Junho de 2017
   
   www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits | dailymotion.com/wrkits
   
======================================================================================================== */


// ========================================================================================================
// --- Mapeamento de Hardware ---
#define    encoder_C1   2                     //Conexão C1 do encoder
#define    encoder_C2   4                     //Conexão C2 do encoder
#define    pwm_out      5                     //Saída pwm para controle de velocida do motor
#define    motor1       6                     //Controle In1 do driver L298N
#define    motor2       7                     //Controle In2 do driver L298N
#define    pot          A0                    //Entrada para leitura de potenciômetro


// ========================================================================================================
// --- Variáveis Globais ---
void motor_control();                         //Função para controle do motor


// ========================================================================================================
// --- Variáveis Globais ---
byte      Encoder_C1Last;
int       pulse_number,
          adc,
          pwm_value = 128;
          
boolean direction_m;


// ========================================================================================================
// --- Configurações Iniciais ---
void setup()
{

  Serial.begin(115200);                       //Inicializa comunicação serial
 
  pinMode(pot,         INPUT);                //Configura entrada para potenciômetro
  pinMode(encoder_C1,  INPUT);                //Configura entrada C1 para leitura do encoder
  pinMode(encoder_C2,  INPUT);                //Configura entrada C2 para leitura do encoder
  pinMode(motor1,     OUTPUT);                //Configura saída para controle do motor (in1 do driver)
  pinMode(motor2,     OUTPUT);                //Configura saída para controle do motor (in3 do driver)
 
  
  attachInterrupt(0, count_pulses, CHANGE);   //Interrupção externa 0 por mudança de estado

  
} //end setup


// ========================================================================================================
// --- Loop Infinito ---
void loop()
{
//|||||||||||||||||||||||||||||||||||||||||||| 
   motor_control();                           //chama função para controle do motor
   delay(100);                                //taxa de atualização

} //end loop


// ========================================================================================================
// --- Função para controle do Motor ---
void motor_control()
{
  
  Serial.print("Num. Pulsos: ");              //Imprime
  Serial.print(pulse_number);                 //Imprime
  adc = analogRead(pot);                      //Lê entrada analógica do potenciômetro e armazena em adc
  
  if (adc >= 512)                             //adc maior ou igual a 512? (metade do valor de 10 bits 1024)
  {                                           //Sim...
    digitalWrite(motor1, LOW);                //Desliga bit de controle motor1
    digitalWrite(motor2, HIGH);               //Liga bit de controle motor2
    Serial.println(" Sentido: Anti-horario"); //Imprime
    pwm_value = map(adc, 512, 1023, 0, 255);  //normaliza valor do pwm de acordo com potenciômetro
    analogWrite(pwm_out, pwm_value);          //gera pwm proporcional
    
  } //end if adc
  
  else                                        //Senão...
  {                                           //adc menor que 512
    digitalWrite(motor1, HIGH);               //Liga bit de controle motor1
    digitalWrite(motor2, LOW);                //Desliga bit de controle motor2
    Serial.println(" Sentido: Horario");      //Imprime
    pwm_value = map(adc, 511, 0, 0, 255);     //normaliza valor do pwm de acordo com potenciômetro
    analogWrite(pwm_out, pwm_value);          //gera pwm proporcional
  }
  
  pulse_number = 0x00;                        //reinicia pulse_number
  
  
} //end motor_control


// ========================================================================================================
// --- Função de contagem de pulsos do encoder ---
//
// *baseada no artigo do site http://www.arduinoecia.com.br/2016/10/motor-dc-com-encoder-arduino.html
//
//
void count_pulses()
{

  int Lstate = digitalRead(encoder_C1);       //Lê estado de encoder_C1 e armazena em Lstate
  
  if(!Encoder_C1Last && Lstate)               //Encoder_C1Last igual a zero e Lstate diferente de zero?
  {                                           //Sim...
    int val = digitalRead(encoder_C2);        //Lê estado de encoder_C2 e armazena em val
    
    if(!val && direction_m) direction_m = false;      //Sentido reverso
    
    else if(val && !direction_m) direction_m = true;  //Sentido direto
    
  } //end if 
 
  Encoder_C1Last = Lstate;                    //Encoder_C1Last recebe o valor antigo

  if(!direction_m)  pulse_number++;           //incrementa número do pulso se direction limpa
  else              pulse_number--;           //senão decrementa

  
} //end count_pulses





