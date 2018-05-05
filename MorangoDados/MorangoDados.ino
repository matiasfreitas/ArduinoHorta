
#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <BH1750.h>
#include <DS1307.h>
#include <LiquidCrystal.h>

//Sensor de Umidade
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot

// Configurando LCD
// VSS no GND, VDD no 5v, V0 no 8, RS no 9, RW no GND, E no 10, D4 no 0, D5 no 1, D6 no 2, D7 no 3, A no 5v, e K no GND
LiquidCrystal lcd(9, 10, 4, 5, 6, 7);
#define constrastOutput 8

// Iniciliza o leitor de SD
//Ligue o GND ao GND, o MISO a entrada 12, o SCK a entrada 13,o MOSI a entrada 11, e 3.3V ao 3.3V
File myFile;
// Pino do Arduino conectado ao pino CS do modulo SD
#define chipSelect 3

//Modulo de relogio RTC DS1307 ligado as portas A4 e A5 do Arduino 
//Ligue o SDA a entrada A4, o SCL a entrada A5, o GND ao GND, e VCC ao 5V
DS1307 rtc(A0, A1); // A entrada do relogio

//Modulo de medição de luz LightMeter BH1750 ligado as portas A4 e A5 do Arduino
BH1750 lightMeter;

// Iniciliza o Sensor DHT22 de umidade e temperatura
#define DHTPIN A3 // pino que estamos conectado
#define DHTTYPE DHT22 // O modelo exato

//
//#define grounHumidSignal A3

// Conecte pino 1 do sensor (esquerda) ao +5V
// Conecte pino 2 do sensor ao pino de dados definido em seu Arduino
// Conecte pino 4 do sensor ao GND
// Conecte o resistor de 10K entre pin 2 (dados) 
// e ao pino 1 (VCC) do sensor
DHT dht(DHTPIN, DHTTYPE);

#define butControl 2

float measuredValues[]{
    0,
    0,
    0,
    0
  };
String nameMeasuredValues[] = {
    "Temp: ",
    "Light: ",
    "Humid: ",
    "Ground Humid: ",
  };
int displayValue = 0;
void setup(){
  
  Serial.begin(9600);
  pinMode(constrastOutput, OUTPUT);
  analogWrite(constrastOutput, 80);

  pinMode(butControl, INPUT_PULLUP);
  
  lcd.begin(16, 2);
  lcd.print("Iniciando...");

  configureClock();
  configureSD();
  lightMeter.begin();
  //pinMode(grounHumidSignal, INPUT);
  dht.begin();
}
void loop() {
  
  File myFile = SD.open("MORANGO.txt", FILE_WRITE);
  if (!digitalRead(butControl)){
    displayValue = displayValue + 1;
    
    if (displayValue > 3){
     displayValue = 0;
    }
    delay(1000);
  }
  
  //int groundHumid = analogRead(grounHumidSignal)
  measuredValues[0] = {dht.readTemperature()};
  measuredValues[1] = {lightMeter.readLightLevel()};
  measuredValues[2] = {dht.readHumidity()};
 // read the analog in value:
   measuredValues[3] = map(analogRead(analogInPin), 0, 1023, 1023, 0);

  

  // if the file is available, write to it:
  if (myFile) {
    myFile.print(rtc.getDateStr(FORMAT_SHORT));
    myFile.print(",");
    myFile.print(rtc.getTimeStr());
    myFile.print(",");
    myFile.print(measuredValues[0]);
    myFile.print(",");
    myFile.print(measuredValues[1]);
    myFile.print(",");
    myFile.println(measuredValues[2]);
    myFile.print(",");
    myFile.println(measuredValues[3]);
    myFile.close();
    showData(displayValue);
  }
  // if the file isn't open, pop up an error:
  else {
    lcd.clear();
    lcd.setCursor(0,0);  
    lcd.print("Erro. Reiniciar");
     while (1);
  }
  
  
  delay(1000);
}
void configureClock(){ 
  
  //Aciona o relogio
  rtc.halt(false);
  
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);

 }
void configureSD(){
  
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
  }
  Serial.println("card initialized.");

  File myFile = SD.open("MORANGO.txt", FILE_WRITE);
  myFile.println("Dia,Horario,Temperatura em Celsius,Luz,Umidade,Umidade do Solo");
  myFile.close();
}

void showData(int displayValue){
  lcd.clear();
  lcd.setCursor(0,0);  
  lcd.print("LENDO SENSOR DE:");
  lcd.setCursor(0,1);  
  lcd.print(nameMeasuredValues[displayValue]);
  lcd.print(measuredValues[displayValue]);
  }
