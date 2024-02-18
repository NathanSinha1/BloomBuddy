#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include<dht11.h>

#define DHT11PIN 2

dht11 DHT11;

LiquidCrystal_I2C lcd(0x27, 16, 2); // Format -> (Address,Width,Height )

const int redPinLight = 4;
const int greenPinLight = 12;

const int redPinTemp = 5;     //notted
const int greenPinTemp = 6;   //notted

const int redPinSoil = 8;
const int greenPinSoil = 7;

const int redPinWater = 9;     //notted
const int greenPinWater = 10;    //notted


const int lightPin = A0;
const int tempPin = 2;
const int soilPin = A1;
const int waterPin = A2;
const int servoPin = 1;

int count = 0;
int test = 0;

Servo servo;

// pins 11,10,9 and 6,5,3 are inverted (0 for HIGH)

void setup() 
{
  pinMode(redPinLight  , OUTPUT);
  pinMode(greenPinLight, OUTPUT);    

  pinMode(redPinTemp   , OUTPUT);
  pinMode(greenPinTemp , OUTPUT);

  pinMode(redPinSoil   , OUTPUT);
  pinMode(greenPinSoil , OUTPUT);

  pinMode(redPinWater  , OUTPUT);
  pinMode(greenPinWater, OUTPUT);


  pinMode(lightPin     , INPUT);
  pinMode(tempPin      , INPUT);
  pinMode(soilPin      , INPUT);
  pinMode(waterPin     , INPUT);
  pinMode(servoPin     , OUTPUT);

  servo.attach(servoPin);
  servo.writeMicroseconds(1500); // Start with a midpoint position

  // set up the LCD's number of columns and rows:
      // initialize the lcd
  lcd.init();
    // Turn on the Backlight
  lcd.backlight();
  // Print a message to the LCD.
  delay(1000);
  Serial.begin(9600);
}

void CheckTemp()
{
  lcd.clear();
  Serial.println();
  int reading = DHT11.read(DHT11PIN);
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature  (C): ");
  Serial.println((float)DHT11.temperature, 2);

  int tempRaw = (float)DHT11.temperature;
  
  if (tempRaw > 35 || tempRaw < 5)
  {
    digitalWrite(greenPinTemp,LOW);
    digitalWrite(redPinTemp,HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Temperature is: ");
    lcd.print(tempRaw);
    lcd.setCursor(0, 1);
    if(tempRaw>35){
      lcd.print("Move to a colder place");
      for (int positionCounter = 0; positionCounter < 22; positionCounter++) {
        // scroll one position right:
        lcd.scrollDisplayRight();
        // wait a bit:
        delay(150);
      }
    }
    else{
      lcd.print("Move to a warmer place");
      for (int positionCounter = 0; positionCounter < 22; positionCounter++) {
        // scroll one position right:
        lcd.scrollDisplayRight();
        // wait a bit:
        delay(150);
      }
    }
    count++;
    delay(3000);
  }
  else if (tempRaw > 30 || tempRaw < 10)
  {

    digitalWrite(greenPinTemp,HIGH);
    digitalWrite(redPinTemp,HIGH);
  }
  else 
  {

    digitalWrite(greenPinTemp,HIGH);
    digitalWrite(redPinTemp,LOW);
    
  }
  
}

void CheckSoil()
{
  lcd.clear();
  int soilRaw = analogRead(soilPin);
  Serial.print("Soil (ADC): ");
  Serial.println(soilRaw);

  int soil = map(soilRaw, 0, 1023, 0, 100);

  
  if (soil > 70 || soil < 20)
  {
    digitalWrite(greenPinSoil,LOW);
    digitalWrite(redPinSoil,HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Soil Moisture is: ");
    lcd.print(soil);
    lcd.setCursor(0, 1);
    if(soil>70){
      lcd.print("Soil is too wet");
    }
    else{
      lcd.print("Soil is too dry");
    }
    count++;
    delay(3000);
  }

  else if (soil > 60 || soil < 40)
  {

    digitalWrite(greenPinSoil,HIGH);
    digitalWrite(redPinSoil,HIGH);
    
  }

  else 
  {

    digitalWrite(greenPinSoil,HIGH);
    digitalWrite(redPinSoil,LOW);
    
  }

}

void CheckWater()
{
  lcd.clear();
  Serial.println();
  int waterRaw = analogRead(waterPin);
  Serial.print("Sensor value: ");
  Serial.println(waterRaw);

  int water = map(waterRaw, 0, 1023, 0, 100);

  
  if (water < 10.0)
  {
    digitalWrite(greenPinWater,LOW);
    digitalWrite(redPinWater,HIGH);
    lcd.setCursor(0,0);
    lcd.print("Water level: "); 
    lcd.print(water);
    lcd.setCursor(0,1);
    lcd.print("Add water");  
    count++;
    delay(3000);
  }
  else if (water < 50)
  {

    digitalWrite(greenPinWater,HIGH);
    digitalWrite(redPinWater,HIGH);
    
  }
  else 
  {

    digitalWrite(greenPinWater,HIGH);
    digitalWrite(redPinWater,LOW);
    
  }

}

void CheckLight()
{
  lcd.clear();
  int raw_light = analogRead(lightPin);
  int light = map(raw_light, 0, 1023, 0, 100);

  Serial.print("Light Level: ");
  Serial.println(light);

  
  if (light < 10)
  {
    digitalWrite(greenPinLight,LOW);
    digitalWrite(redPinLight,HIGH);
    lcd.setCursor(0,0);
    lcd.print("Light level: ");
    lcd.print(light);
    lcd.setCursor(0,1);
    lcd.print("Need more light");
    count++;
    delay(3000);
  }

  else if (light < 25)
  {

    digitalWrite(greenPinLight,HIGH);
    digitalWrite(redPinLight,HIGH);
    
  }

  else 
  {

    digitalWrite(greenPinLight,HIGH);
    digitalWrite(redPinLight,LOW);
    
  }
}

void loop() 
{
  CheckSoil();
  CheckTemp();
  CheckLight();
  CheckWater();
  if (count==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Plant is Happy and Healthy");
    for (int positionCounter = 0; positionCounter < 26; positionCounter++) {
      // scroll one position right:
      lcd.scrollDisplayRight();
      // wait a bit:
      delay(150);
    }
  }
  count = 0;
  delay(3000);
}