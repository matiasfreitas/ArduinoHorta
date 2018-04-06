
#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <BH1750.h>
#include <DS1307.h>
#include <LiquidCrystal.h>

// Configurando LCD
// VSS no GND, VDD no 5v, V0 no 8, RS no 9, RW no GND, E no 10, D4 no 0, D5 no 1, D6 no 2, D7 no 3, A no 5v, e K no GND
LiquidCrystal lcd(9, 10, 0, 1, 2, 3);
#define constrastOutput 8

// Iniciliza o leitor de SD
//Ligue o GND ao GND, o MISO a entrada 12, o SCK a entrada 13,o MOSI a entrada 11, e 3.3V ao 3.3V
File myFile;
#define chipSelect 4 

//Modulo de relogio RTC DS1307 ligado as portas A4 e A5 do Arduino 
//Ligue o SDA a entrada A4, o SCL a entrada A5, o GND ao GND, e VCC ao 5V
DS1307 rtc(A0, A1); // A entrada do relogio

//Modulo de medição de luz LightMeter BH1750 ligado as portas A4 e A5 do Arduino
BH1750 lightMeter;

// Iniciliza o Sendor DHT22 de umidade e temperatura
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


// Pino do Arduino conectado ao pino CS do modulo SD
#define chipSelect 4
  
void setup(){
  
  Serial.begin(9600);
  pinMode(constrastOutput, OUTPUT);
  analogWrite(constrastOutput, 96);
  
  lcd.begin(16, 2);
  lcd.print("hello, world!");

  configureClock();
  configureSD();
  lightMeter.begin();
  //pinMode(grounHumidSignal, INPUT);
  dht.begin();
}
void loop() {
   
  File myFile = SD.open("MORANGO.txt", FILE_WRITE);
  //int sensorVal = digitalRead(9);
  float humid = dht.readHumidity();
  float temp = dht.readTemperature();
  //int groundHumid = analogRead(grounHumidSignal);
  uint16_t lux = lightMeter.readLightLevel();

  // if the file is available, write to it:
  if (myFile) {
    myFile.print(rtc.getDateStr(FORMAT_SHORT));
    myFile.print(",");
    myFile.print(rtc.getTimeStr());
    myFile.print(",");
    myFile.print(temp);
    myFile.print(",");
    myFile.print(lux);
    myFile.print(",");
    myFile.println(humid);
    //myFile.print(",");
    //myFile.println(groundHumid);
    myFile.close();
  delay(5000);

  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening morango.txt");
     while (1);
  }
  
  

  lcd.setCursor(0, 1);
  lcd.print("hello, world!");
  delay(5000);
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
  myFile.println("Dia,Horario,Temperatura em Celsius,Luz,Umidade,Umidade Do Solo");
  myFile.close();
}
