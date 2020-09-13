#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const char buttonPin = 7;
const char sensorPin = A0;
const char buzzer = 9;
const char powerLCD = 13;

void setup() {

  pinMode(buttonPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(powerLCD, OUTPUT);
  digitalWrite(powerLCD, LOW);

  lcd.noDisplay();
  noTone(buzzer);

  lcd.begin(16, 2);

  Serial.begin(9600);

}

static float minTemp = 126.00;
static float maxTemp = -41.00;

float calc = 0.0;
float temperature = 0.0;
float tempSum = 0.0;
float readingTemp = 0.0;

char buttonState = 0;
char stationState = 0;
char ok = 0;
char cntReadings = 0;
float newValue;
int start = 0;
float media = 0.0;
int cycles = 0;
int okDif = 0;
int readTemp = 0;
int medCount = 0;

void loop() {

  char crtState = digitalRead(buttonPin);

  if (buttonState != crtState && crtState == LOW) {

    stationState = !stationState;
    digitalWrite(powerLCD, stationState);

  }

  if (stationState == HIGH && ok == 0) {

    lcd.display();
    digitalWrite(powerLCD, HIGH);
    ok = 1;

  } else if (stationState == LOW && ok == 1) {

    lcd.noDisplay();
    digitalWrite(powerLCD, LOW);
    noTone(buzzer);
    ok = 0;

  }

  buttonState = crtState;

  if (stationState == HIGH) {

    //while(cntReadings < 5){
    
    
  
    if(readTemp == 7 && start == 1){
      readingTemp = analogRead(sensorPin);
        
        tempSum += readingTemp;
        cntReadings++;
        //   delay(490);
        readTemp = 0;
        medCount++;
    }else{
        readTemp++;
    }
    
    
  
    if (start == 0) {
      newValue = temperature;
      start = 1;
    }

    // }
  
    if (medCount == 5) {
      medCount = 0;
      temperature = tempSum / 5.0;
      calc = temperature * 5.0;
        calc /= 1024.0;
        newValue = (calc - 0.5) * 100;
      tempSum = 0.0;

    }

    //cntReadings = 0;
    //temperature = tempSum / 5.0;
    //tempSum = 0.0;

    if (newValue < minTemp) {

      minTemp = newValue;

    }
    if (newValue > maxTemp) {

      maxTemp = newValue;

    }

    if (newValue < 7.5 && newValue >= 5.0) {

      if (okDif != 1) {
        cycles = 0;
        okDif = 1;
      }

      if (cycles == 75 && okDif == 1) {

        tone(buzzer, 550, 250);
        
        cycles = 0;
       
      }else{
         //delay(250);
        noTone(buzzer);
        //  delay(125); 32
        
        
      }

    } else if (newValue > 40.0 && newValue <= 75.0) {

      if (okDif != 2) {
        cycles = 0;
        okDif = 2;
      }

      if (cycles == 150 && okDif == 2) {

        tone(buzzer, 197000, 500);
       

        cycles = 0;

      }else{
         //delay(500); 
        noTone(buzzer);
        //  delay(250);
        
      }

    } else if (newValue < 5.0) {

      if (okDif != 3) {
        cycles = 0;
        okDif = 3;
      }

      if (cycles == 60 && okDif == 3) {

        tone(buzzer, 550, 250);
     
        cycles = 0;

      }else{
        
        //delay(250);
        noTone(buzzer);
        //delay(50);
      }

    } else if (newValue > 75.0) {

      if (okDif != 4) {
        cycles = 0;
        okDif == 4;
      }

      if (cycles == 130 && okDif == 4) {

        tone(buzzer, 197000, 500);
    
        cycles = 0;

      }else{
        //delay(500); 
        noTone(buzzer);
        //  delay(150);
    
      }

    }

    lcd.setCursor(1, 0);
    lcd.print(minTemp, 2);
    lcd.write(176);

    lcd.setCursor(9, 0);
    lcd.print(maxTemp, 2);
    lcd.write(176);

    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.setCursor(8, 1);
    lcd.print(newValue, 2);
    lcd.write(176);

    cycles++;

    Serial.println(cycles);

    delay(70);

  }

}
