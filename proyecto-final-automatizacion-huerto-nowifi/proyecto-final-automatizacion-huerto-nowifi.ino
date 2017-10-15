/**
 * I2C/IIC LCD Serial Adapter Module Example
 * Tutorial by http://mklec.com
 * 
 * Instructions at http://blog.mklec.com/how-to-use-iici2c-serial-interface-module-for-1602-lcd-display
 *
 * This uses the Liquid Crystal library from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads GNU General Public License, version 3 (GPL-3.0)
 * Pin Connections: 
 *      SCL = A5
 *      SDA = A4
 *      VCC = 5V
 *      GND = GND
 */

 //=========== LIBRARIES =====================
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include<DHT.h>
#include<OneWire.h>
#include<DallasTemperature.h>



//=========== DEFINE =====================
#define bomba1 2
#define bomba2 3
#define Sensor_de_nivel 0

//=========== DECLARATIONS =====================

DHT sensor_DHT(4,DHT11);
OneWire pin(11); // conectado en pin 11
DallasTemperature sensor_temperatura(&pin);
float Temperatura = 0;
int humedad_del_suelo;
int nivel_de_agua = 0;
LiquidCrystal_I2C  lcd(0x3F,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified module

//=========== SETUP =====================

void setup()
{
    lcd.setBacklightPin(3,POSITIVE);
    lcd.setBacklight(HIGH); // NOTE: You can turn the backlight off by setting it to LOW instead of HIGH
    lcd.begin(16, 2);
    lcd.clear();
    pinMode(bomba1,OUTPUT);
    pinMode(bomba2,OUTPUT);
    Serial.begin(9600);
    digitalWrite(bomba1,LOW);
    digitalWrite(bomba2,LOW);
    sensor_DHT.begin();
    sensor_temperatura.begin();
}

//=========== LOOP =====================

void loop()
{
    Llenado_del_Tanque();
    Luz();
    Humedad_y_Temperatura();
    Riego();
    Temperatura_de_la_tierra();
    delay(500);
}

//=========== FUNCTIONS =====================

void Riego(){
  humedad_del_suelo=analogRead(1);
  //Convertimos el valor analogo 0-1023 a porcentaje 100-0
  humedad_del_suelo=map(humedad_del_suelo,0,1023,100,0);
  Serial.print("Porcentaje de Humedad de la Tierra");
  Serial.print(humedad_del_suelo);
  Serial.print("%");
  if(humedad_del_suelo <=40){
    Serial.println("Tierra muy seca, RIEGO ACTIVADO");
    digitalWrite(bomba1,1);
  }
  if((humedad_del_suelo > 40) and (humedad_del_suelo <=65)){
    digitalWrite(bomba1,0);
    Serial.println("Tierra con buena humedad");
  }
  if(humedad_del_suelo >65){
    digitalWrite(bomba1,0);
    Serial.println("Demasiada Agua, REQUIERE DRENAR AGUA");
  }
}

void Luz(){
  int luz = analogRead(2);
  luz = map(luz,0,1023,0,100);
  Serial.print("Porcentaje de Luz= ");
  Serial.print(luz);
  Serial.println("%");
  lcd.setCursor(9,0);
  lcd.print("Luz:");
  lcd.print(luz);
  lcd.print("%");  
}

void Humedad_y_Temperatura(){
  int humedad = sensor_DHT.readHumidity();
  int temperatura_ambiente = sensor_DHT.readTemperature();
  Serial.print("Humedad= ");
  Serial.print(humedad);
  Serial.print(" ,Temperatura ambiente= ");
  Serial.println(temperatura_ambiente);
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temperatura_ambiente);
  lcd.setCursor(0,1);
  lcd.print("H:");
  lcd.print(humedad);
  lcd.print("%");
}

void Temperatura_de_la_tierra(){
  sensor_temperatura.requestTemperatures();
  Temperatura = sensor_temperatura.getTempCByIndex(0);
  Serial.print("Temperatura de la tierra= ");
  Serial.println(Temperatura);
  lcd.setCursor(6,1);
  lcd.print("TT:");
  lcd.print(Temperatura);
}

void Llenado_del_Tanque(){
  nivel_de_agua=analogRead(Sensor_de_nivel);
  Serial.print("Nivel de agua ");
  Serial.println(nivel_de_agua);

  if(nivel_de_agua > 400){
    digitalWrite(bomba2,LOW);
    Serial.println("Tanque lleno");
  }
  else{
    digitalWrite(bomba2,HIGH);
    Serial.println("Bomba Activada");
  }
}



