
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <BH1750.h>
#include <DS1307.h>

File myFile;
//Carrega a biblioteca do RTC DS1307

 
//Modulo RTC DS1307 ligado as portas A4 e A5 do Arduino 
DS1307 rtc(A0, A1);
BH1750 lightMeter;
 
#define DHTPIN A2 // pino que estamos conectado
#define DHTTYPE DHT22 // DHT 22
#define grounHumidSignal A3

// Conecte pino 1 do sensor (esquerda) ao +5V
// Conecte pino 2 do sensor ao pino de dados definido em seu Arduino
// Conecte pino 4 do sensor ao GND
// Conecte o resistor de 10K entre pin 2 (dados) 
// e ao pino 1 (VCC) do sensor
DHT dht(DHTPIN, DHTTYPE);


// Pino do Arduino conectado ao pino CS do modulo

#define chipSelect 10
  
void setup()
{
  Serial.begin(9600);


  lightMeter.begin();
  
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  File myFile = SD.open("MORANGO.txt", FILE_WRITE);
  myFile.println("Dia,Horario,Temperatura em Celsius,Luz,Umidade,Umidade Do Solo");
  myFile.close();

  //Aciona o relogio
  rtc.halt(false);
  pinMode(grounHumidSignal, INPUT);
  //As linhas abaixo setam a data e hora do modulo
  //e podem ser comentada apos a primeira utilizacao
  rtc.setDOW(MONDAY);      //Define o dia da semana
  rtc.setTime(15, 42, 00);     //Define o horario
  rtc.setDate(02, 03, 2018);   //Define o dia, mes e ano
  pinMode(LED_BUILTIN, OUTPUT);
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
  pinMode(9, INPUT_PULLUP);
  dht.begin();
}
void loop() 
{ 
  File myFile = SD.open("MORANGO.txt", FILE_WRITE);
  int sensorVal = digitalRead(9);
  float humid = dht.readHumidity();
  float temp = dht.readTemperature();
  int groundHumid = analogRead(grounHumidSignal);
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
    myFile.print(humid);
    myFile.print(",");
    myFile.println(groundHumid);
    myFile.close();

  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening morango.txt.txt");
  }
  if (sensorVal == LOW) {
    Serial.print("File Closed");
    while (1) {}
  } 
  delay(1000);

}
