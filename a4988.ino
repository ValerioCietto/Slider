//simple A4988 connection
//jumper reset and sleep together
//connect  VDD to Arduino 3.3v or 5v
//connect  GND to Arduino GND (GND near VDD)
//connect  1A and 1B to stepper coil 1
//connect 2A and 2B to stepper coil 2
//connect VMOT to power source (9v battery + term)
//connect GRD to power source (9v battery - term)


int stp = 11;  //connect pin 13 to step
int dir = 12;  // connect pin 12 to dir
int a = 0;     //  gen counter

int analog = A0;
int analog_value = 0;
int speed1 = 50;
int speedInit= 1000;

int delayRead=1000;
void setup() 
{      

  Serial.begin(9600);  
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT); 
  pinMode(analog, INPUT);  

  for(int i =1000; i>0;i--){
    digitalWrite(stp, HIGH);   
    delayMicroseconds(i+speed1);               
    digitalWrite(stp, LOW);  
    delayMicroseconds(i+speed1);    
  }  
}


void loop() 
{
  if (a <  2000)  //sweep 200 step in dir 1
   {
    a++;
    digitalWrite(dir, LOW);
    digitalWrite(stp, HIGH);   
    delayMicroseconds(speed1);               
    digitalWrite(stp, LOW);  
    delayMicroseconds(speed1);              
   }
   else{
    digitalWrite(dir, HIGH);
    digitalWrite(stp, HIGH);   
    delayMicroseconds(speed1);               
    digitalWrite(stp, LOW);  
    delayMicroseconds(speed1);
     
   }
  
    
}
