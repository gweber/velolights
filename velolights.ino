// pins 
int iBlinkLeft = 10;
int iDayLight = 11;
int iRearLight = 12;
int iBlinkRight = 13;

// default brightness, 0 dark, 255 max

int defFrontBright = 16;
int defRearBright = 16;
int defBreakBright = 16;
int defBlinkBright = 16;

int defWarnTimeout = 1000; // ms between switches for warning lights

// switches
#define LEFTBLINKBUTTON A2
#define RIGHTBLINKBUTTON A1
#define BREAKBUTTON A3

unsigned long blinkStarted = 0;
boolean currentlyBlinking = 0;
unsigned long warnTimeout = 0;


void lightBright(int led, int brightness = 0) {
  analogWrite(led, 255 - brightness);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Velolights started");
  pinMode(iBlinkLeft, OUTPUT);
  lightBright(iBlinkLeft, defBlinkBright);
  pinMode(iBlinkRight, OUTPUT);
  lightBright(iBlinkRight, defBlinkBright);
  pinMode(iDayLight, OUTPUT);
  lightBright(iDayLight, defFrontBright);
  pinMode(iRearLight, OUTPUT);
  lightBright(iRearLight, defFrontBright);
  warnTimeout = millis();
}

void breakLight(int brightness = 255) {
  lightBright(iRearLight, brightness);
}

void turnOn(int led){
  lightBright(led,255);
}

void turnOff(int led){
  lightBright(led, 0);
}

void blinking() {
  boolean doBlinkRight = !digitalRead(LEFTBLINKBUTTON);
  boolean doBlinkLeft = !digitalRead(RIGHTBLINKBUTTON);

  if ( doBlinkLeft && millis()-warnTimeout < defWarnTimeout ) { //    
        Serial.println("doBlinkLeft");
        //Serial.println(millis() - warnTimeout);
        doBlinkRight = 1;
        warnTimeout = millis();
  }

  if ( doBlinkRight || doBlinkLeft ) {
    if (!currentlyBlinking) {
      Serial.println(millis() - warnTimeout);
      currentlyBlinking = 1;
      blinkStarted = millis();
      turnOff(iDayLight);    
      turnOff(iBlinkLeft);   
      turnOff(iBlinkRight);
      
    }
  } else {
    if (currentlyBlinking){
      currentlyBlinking = 0;
      warnTimeout = millis();
      lightBright(iBlinkLeft, defBlinkBright);
      lightBright(iBlinkRight, defBlinkBright);
      lightBright(iDayLight, defFrontBright); // turn front light to default
    }
  }


  if ( currentlyBlinking && ( (millis() - blinkStarted) / 500 ) % 2 ) {
    if ( doBlinkRight ) {
      lightBright(iBlinkRight, 255);
      if ( ! doBlinkLeft ) {
        lightBright(iBlinkLeft, 0);
      }
    }
    if ( doBlinkLeft ) {
      lightBright(iBlinkLeft, 255);
      if ( ! doBlinkRight ) {
        lightBright(iBlinkRight, 0);
      }
    }
  } else {
    if ( doBlinkRight ) {
      lightBright(iBlinkRight, 0);
    }
    if ( doBlinkLeft ) {
      lightBright(iBlinkLeft, 0);
    }
  }
}


void breaking(){
  if (digitalRead(BREAKBUTTON)) { breakLight(defRearBright); }
  if (!digitalRead(BREAKBUTTON)) breakLight( 255 );
}


void loop() {
  breaking();
  blinking();
}

