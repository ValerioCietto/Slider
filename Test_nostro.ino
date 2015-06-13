//simple A4988 connection
//jumper reset and sleep together
//connect  VDD to Arduino 3.3v or 5v
//connect  GND to Arduino GND (GND near VDD)
//connect  1A and 1B to stepper coil 1
//connect 2A and 2B to stepper coil 2
//connect VMOT to power source (9v battery + term)
//connect GRD to power source (9v battery - term)


int stp = 3;  //connect pin 13 to step
int dir = 2;  // connect pin 12 to dir
int a = 0;     //  gen counter
int speed1 = 19;
int speedInit= 1000;
void setup() 
{                
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);     

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
    digitalWrite(stp, HIGH);   
    delayMicroseconds(speed1);               
    digitalWrite(stp, LOW);  
    delayMicroseconds(speed1);              
   }
  else 
   {
    digitalWrite(dir, HIGH);
    a++;
    digitalWrite(stp, HIGH);  
    delayMicroseconds(speed1);               
    digitalWrite(stp, LOW);  
    delayMicroseconds(speed1);
    
    if (a>40)    //sweep 200 in dir 2
     {
      a = 0;
      digitalWrite(dir, LOW);
     }
    }
    
}
