#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <HX711.h>

//Keypad------------------------------------
#define ROWS 4                           /*|*/
#define COLS 4                           /*|*/
char keys[ROWS][COLS] = {                /*|*/
  {'1','2','3','A'},                     /*|*/
  {'4','5','6','B'},                     /*|*/
  {'7','8','9','C'},                     /*|*/
  {'*','0','#','D'}                      /*|*/
};                                       /*|*/
byte rowPins[ROWS] = {p8, p7, p6, p5};   /*|*/
byte colPins[COLS] = {p4, p3, p2, p1};   /*|*/
Keypad keypad = Keypad(                  /*|*/
  makeKeymap(keys),                      /*|*/
 rowPins, colPins, ROWS, COLS            /*|*/
 );                                      /*|*/
//------------------------------------------

//LCD-----------------------------------
LiquidCrystal_I2C lcd(0x27,16,2);    /*|*/
//--------------------------------------

//Load Cell ----------------------------
HX711 scale;                         /*|*/
//--------------------------------------
void Init();
int input();
void getinput();

long int totalWeight  = 0 , share1 = 0;
float tank1 = 0.f;
float scale1 = -108.f;
void setup() {
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  lcd.init();
  lcd.backlight();
}
void loop() {
  Init();
  getinput();
}
void Init(){
  totalWeight =0;
  share1 = 0;
  tank1 = 0.f;
}
int input(){
  int key = keypad.getKey();
  if(key)
  {
    if(key == 35){
      return -1;
      }
    else if(key == 42){
      return -2;
    }
    else if(key>=48 && key<=57){
      return key-48;
    }
  }
  return -3;
}
void getinput(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("OUT Wight:0");
  lcd.setCursor(0, 1);
  lcd.print("T1 Share:0");
  while(true){
    int temp = input();
    if(temp == -1)
      break;
    else if(temp == -2)
      {
        lcd.setCursor(10, 0);
        lcd.print("      ");
        totalWeight = 0;
        temp=0;
      }
      else if(temp != -3){
        totalWeight*=10;
        totalWeight+=temp;
      }
    lcd.setCursor(10, 0);
    lcd.print(totalWeight);
  }
  while(true){
    int temp = input();
    if(temp == -1)
      break;
    else if(temp == -2)
      {
        lcd.setCursor(9, 1);
        lcd.print("      ");
        share1 = 0;
        temp=0;
      }
      else if(temp != -3){
        share1*=10;
        share1+=temp;
      }
    lcd.setCursor(9, 1);
    lcd.print(share1);
  }
  tank1 = ((totalWeight * share1)*(1.0))/(100.0);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("wight :");
  scale.begin(A1, A0);
  scale.set_scale(scale1);
  scale.tare();

  while(true)
  {

    float wight = -1*scale.get_units(10);
    Serial.println(wight);
    if (wight <=-10)
    {
      digitalWrite(10, HIGH);
    }
    else
    {
      lcd.setCursor(7, 0);
      lcd.print(wight);
      digitalWrite(10, LOW);
      if(wight >= tank1)
      {
        digitalWrite(10, HIGH);
        wight = -1*scale.get_units(10);
        Serial.print("flag :");
        Serial.println(wight);
        if(wight >= tank1)
        {
          digitalWrite(10, HIGH);
          break;
        }
      }
    }
  }
  while(true)
  {
    float wight = -1*scale.get_units(10);
    Serial.println(wight);
    if (wight <=-10)
    {
      digitalWrite(11, HIGH);
    }
    else
    {
      lcd.setCursor(7, 0);
      lcd.print(wight);
      digitalWrite(11, LOW);
      if(wight >= totalWeight)
      {
        digitalWrite(11, HIGH);
        wight = -1*scale.get_units(10);
        Serial.print("flag :");
        Serial.println(wight);
        if(wight >= totalWeight)
        {
          digitalWrite(11, HIGH);
          break;
        }
      }
    }
  }
}