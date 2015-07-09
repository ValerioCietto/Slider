/*
SliderCam v0.2 - Rob Taylor (@robtaylorcase) June 2014 GPL 3.0
SliderCam v1.4 - Valerio Cietto (@ValerioCietto) jume 2015 GPL 3.0

 */

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);    //set LCD output pins

//define stepper driver pins
const int stp = 2;    //can't use pin 10 with the SS LCD as it's the backlight control.
//if it goes low, backlight turns off!
const int dir = 3;

const boolean debug = true;

//define trigger pin
const int trig = 13;

//BUTTONS
//define button values
const int btnUp = 0;
const int btnDn = 1;
const int btnL = 2;
const int btnR = 3;
const int btnSel = 4;
const int btnNone = 5;

//define button-reading variables
int btnVal = 5;
int adcIn = 0;
int adcPot = 0;

int delayButton = 200;

//declare button poll function
int readLcdButtons() {
  delay(90); //debounce delay, tuned experimentally. delay is fine as program shouldn't be doing anything else
  //at this point anyway
  adcIn = analogRead(0); //read value from pin A0
  
  /*threshold values confirmed by experimentation with button calibration sketch returning the following ADC read values:
   right: 0
   up: 143
   down: 328
   left: 504
   select: 741
   */
  if (debug) {
    Serial.print(adcIn);
  }
  if (adcIn > 1000) {
    if (debug) {
      Serial.println("none");
    }
    return btnNone;
  }
  if (adcIn < 50) { //giusto
    if (debug) {
      Serial.println("button right");
    }
    return btnR;
  }
  if (adcIn < 110) { //up 99 100 giusto
    if (debug) {
      Serial.println("button up");
    }

    return btnUp;
  }
  if (adcIn < 330) {
    if (debug) {
      Serial.println("button down");
    }
    return btnDn;
  }
  if (adcIn < 450) { //select 639
    if (debug) {
      Serial.println("button left");
    }
    return btnL;
  }
  if (adcIn < 850) {
    if (debug) {
      Serial.println("button select");//left
    }
    return btnSel;
  }

  return btnNone; //if it can't detect anything, return no button pressed
}

//MENU GUI
//define top-level menu item strings for numerical navigation
char* menuItemsTop[] = {
  "  01 Timelapse >", "< 02 Manual ctrl"
};
char* menuTimelapse[] = {
  "  1 Numero scatti >", "< 2 Intervallo>", "< 3 Direzione >", "< 4 go timelapse!"
};
char* menuManuale[] = {
  "usa le freccie"
};



int currentMenuLevel = 0;      //top or sub menu
int currentMenuItem = 0;       //x-axis position of menu selection
int currentTypeMenu = 0; //0 top menu, 1 timelapse menu, 2 manual menu

const int MENU_MAIN = 0;
const int MENU_TIMELAPSE = 1;
const int MENU_MANUAL = 2;

int currentCursorPos = 0;      //current lcd cursor position
int currentDistance[4] = {
  0, 0, 0, 0
};
int currentDuration[6] = {
  0, 0, 0, 0, 0, 0
};
int currentSteps[4] = {
  0, 0, 0, 1
};

//MENU FUNCTIONALITY
int currentChar = 0;        //global declarations of array-parsing variables
int update = 0;
double ThtuArr[] = {
  0000, 000, 00, 0
};
double HTThtuArr[] = {
  000000, 00000, 0000, 000, 00, 0
};
double currentDistanceInt = 0000;
double currentDurationInt = 000000;
double currentStepsInt = 0001;
int travelDir = 0;

int adjustDigit(int x, int dir) {     //digit adjust function
  if (dir == 0 && x > 0) x--;         //subtract from digit on btnDn
  if (dir == 1 && x < 9) x++;         // add to digit on btnUp
  lcd.setCursor(currentCursorPos, 1);
  lcd.print(x);
  currentChar = x;
  return currentChar;                 //return new digit
}



int parseNumber(int n) {
  for (int i = 0; i < n; i++) {
    currentChar = currentSteps[i];
    ThtuArr[i] = currentChar *  pow (10, (3 - i));
  }
  update = ThtuArr[0] + ThtuArr[1] + ThtuArr[2] + ThtuArr[3];
  return update;
}

//MOTION CONTROL
double totalMotorSteps = 10000;
double pulseDelay = 0;
int intervalDistance = 0;           //number of motor steps contained within a camera step
int currentStep = 0;        //number of motor steps thus far
int motion = 0;             // motion = 1 if stop hasn't been pressed
int shutterDuration = 2;   //length of time for the camera to stop at shot steps in seconds
int numberPhoto = 5;
int delayTime = 1000;
int speed1 = 10; //velocità controllo manuale
int loopManuale = 10; // numero di microstep da fare
int speedMax = 1;
int speedMin = 10000;

int startTimelapse() {
  //once per overall run
  if (travelDir == 0) digitalWrite(dir, LOW);
  else if (travelDir == 1) digitalWrite(dir, HIGH);

  int stepsBetweenPhotos = totalMotorSteps / numberPhoto;
/*
  for (int i = 0; i < numberPhoto; i++) {
    for (int j = 0; j < stepsBetweenPhotos; j++) {
      digitalWrite(stp, HIGH); //fire motor driver step
      delayMicroseconds(speed1);
      digitalWrite(stp, LOW); //reset driver
    }
    digitalWrite(trig, HIGH); //trigger camera shutter
    delayMicroseconds(80 * 1000);
    digitalWrite(trig, LOW);    //reset trigger pin
    delay((shutterDuration * 1000) - 80); //delay needs changing to timer so stop button can be polled
  }

*/
} //end motion control
//###########################################################################   SETUP
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);               // initialise LCD lib full-screen
  lcd.setCursor(0, 0);            // set cursor position

  pinMode(stp, OUTPUT);           //initialise stepper pins
  pinMode(dir, OUTPUT);

  pinMode(trig, OUTPUT);           //initialise trigger pin
  digitalWrite(trig, LOW);         //ensure trigger is turned off

  lcd.print("Welcome to");  //welcome screen
  lcd.setCursor(0, 1);
  lcd.print("Q-radi slider v1");
  delay(1000);
  lcd.clear();
  lcd.print(menuItemsTop[0]);
  lcd.setCursor(0,1);
  lcd.print("scegli modo");
  
  digitalWrite(12, HIGH);
  for (int i = 0; i < 1000; i++) {
          
          digitalWrite(13, HIGH);
          delayMicroseconds(5000);
          digitalWrite(13, LOW);
          delayMicroseconds(5000);}
}

//#############################################################################   MAIN LOOP
void loop() {
  //lcd.clear();
  do {
    btnVal = readLcdButtons();      //continually read the buttons...
  }
  while (btnVal == 5);            //...until something is pressed
  if (currentTypeMenu == MENU_MAIN) {
    lcd.setCursor(0, 0);
    lcd.print(menuItemsTop[currentMenuItem]);    //print top level menu item
    if (currentMenuItem == 0) {
      switch (btnVal) {
        case btnR:
          {
            currentMenuItem = 1;
            break;
          }
        case btnSel:
          {
            currentTypeMenu = MENU_TIMELAPSE;
            currentMenuItem = 0;
            break;
          }
      }    //end switch
    }
    else if (currentMenuItem == 1) {
      switch (btnVal) {
        case btnL:
          {
            currentMenuItem = 0;
            break;
          }
        case btnSel:
          {
            currentTypeMenu = MENU_MANUAL;
            currentMenuItem = 0;
            break;
          }
      }    //end switch
    }
  }
  else if (currentTypeMenu == MENU_TIMELAPSE) {
    lcd.setCursor(0, 0);
    lcd.print(menuTimelapse[currentMenuItem]);    //print top level menu item
    switch (currentMenuItem) {
      case 0: { //numero scatti
          menuScatti(btnVal);
          lcd.setCursor(0, 1);
          lcd.print(numberPhoto);
          break;
        }
      case 1: { //intervallo
          menuIntervallo(btnVal);
          lcd.setCursor(0, 1);
          lcd.print(delayTime);
          break;
        }
      case 2: { //direzione
          menuDirezione(btnVal);

          break;
        }
      case 3: { //go!
          menuPartenza(btnVal);
          break;
        }

    }
  }
  else if (currentTypeMenu == MENU_MANUAL) {
    lcd.setCursor(0, 1);
    lcd.print(menuManuale[currentMenuItem]);    //print top level menu item
    switch (currentMenuItem) {
      case 0: {
          menuMuoviManuale(btnVal);
          break;
        }
      case 1: { //intervallo
          currentMenuItem = 0;
          break;
        }
      case 2: { //direzione
          currentMenuItem = 0;
          break;
        }
      case 3: { //go!
          currentMenuItem = 0;
          break;
        }

    }
  }



}
void menuMuoviManuale(int buttonVal) {
  

  switch (buttonVal) {
    case btnUp:
      {

        if (speed1 > speedMax) {
          speed1 = speed1 - 1;
        }
        break;
      }

    case btnDn:
      {

        if (speed1 < speedMin) {
          speed1 = speed1 + 1;
        }
        break;
      }

    case btnL:
      {
        /*
        for (int i = 0; i < loopManuale; i++) {
          digitalWrite(dir, HIGH);
          digitalWrite(stp, HIGH);
          delayMicroseconds(speed1);
          digitalWrite(stp, LOW);
          delayMicroseconds(speed1);
        }*/
        break;
      }

    case btnR:
      {
        /*
        for (int i = 0; i < loopManuale; i++) {
          digitalWrite(dir, LOW);
          digitalWrite(stp, HIGH);
          delayMicroseconds(speed1);
          digitalWrite(stp, LOW);
          delayMicroseconds(speed1);
        }
        */
        break;
      }
    case btnSel:
      {

        break;
      }
  }    //end switch

}
void menuScatti(int buttonVal) { //0
  lcd.setCursor(4,1);
  lcd.print("scatti");
  switch (buttonVal) {
    case btnUp:
      {
        if(numberPhoto<9999){
          if(numberPhoto>9){
            numberPhoto = numberPhoto + 5;
          }
          else if(numberPhoto<=9){
            numberPhoto = numberPhoto + 1;
          }
        }
        
        delay(delayButton);
        break;
      }

    case btnDn:
      {
        if(numberPhoto>1){
          numberPhoto = numberPhoto - 1;
        }
        delay(delayButton);
        break;
      }

    case btnL:
      {
        currentMenuItem = 3;
        delay(delayButton);
        break;
      }

    case btnR:
      {
        currentMenuItem = 1;
        delay(delayButton);
        break;
      }
    case btnSel:
      {

        //currentMenuLevel--;
        break;
      }
  }    //end switch

}

void menuIntervallo(int buttonVal) {
  lcd.setCursor(4,1);
  lcd.print("secondi");
  switch (buttonVal) {
    case btnUp:
      {
        delayTime = delayTime + 1;
        break;
      }

    case btnDn:
      {
        delayTime = delayTime - 1;
        break;
      }

    case btnL:
      {
        currentMenuItem = 0;
        delay(delayButton);
        break;
      }

    case btnR:
      {
        currentMenuItem = 2;
        delay(delayButton);
        break;
      }
    case btnSel:
      {

        //currentMenuLevel--;
        break;
      }
  }    //end switch

}
void menuDirezione(int buttonVal) {
  
  if (travelDir = 0) {
    lcd.setCursor(0, 1);
    lcd.print("verso il motore");
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print("da il motore");
  }
  switch (buttonVal) {
    case btnUp:
      {
        travelDir = 0;

        break;
      }

    case btnDn:
      {
        travelDir = 1;
        break;
      }

    case btnL:
      {
        currentMenuItem = 1;
        delay(delayButton);
        break;
      }

    case btnR:
      {
        currentMenuItem = 3;
        delay(delayButton);
        break;
      }
    case btnSel:
      {

        //currentMenuLevel--;
        break;
      }
  }    //end switch

}
void menuPartenza(int buttonVal) {
  switch (buttonVal) {
    case btnUp:
      {

        break;
      }

    case btnDn:
      {
        for (int j = 0; j < 100; j++) {
          for (int i = 0; i < 1000; i++) {
            /*
            digitalWrite(dir, HIGH);
            digitalWrite(stp, HIGH);
            delayMicroseconds(speed1 + 500);
            digitalWrite(stp, LOW);
            delayMicroseconds(speed1 + 50);
            */
          }
          delay(1000);
        }
        break;
      }

    case btnL:
      {
        currentMenuItem = 2;
        delay(delayButton);
        break;
      }

    case btnR:
      {
        currentMenuItem = 0;
        delay(delayButton);
        break;
      }
    case btnSel:
      {
        lcd.setCursor(0, 1);
        lcd.print("inizio timelapse!");
        /*
        for (int i = 0; i < 100000; i++) {
          digitalWrite(dir, HIGH);
          digitalWrite(stp, HIGH);
          delayMicroseconds(speed1 + 50);
          digitalWrite(stp, LOW);
          delayMicroseconds(speed1 + 50);
        }
        */
        startTimelapse();
        break;
      }
  }    //end switch

}
