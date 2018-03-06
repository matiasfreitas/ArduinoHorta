// Projeto F – Controle de Luminosidade

int sensorLight = A0;
int sensorTemp = A1;
int sensorUmity = A2;
int snoozer = 11;
int valUmity = 0;
int valTemp = 0;
int valLight = 0;

unsigned long time;

void setup() {
  pinMode(8, OUTPUT);
  pinMode(sensorLight, INPUT);
  pinMode(sensorLight, INPUT);
  pinMode(sensorLight, INPUT);
  Serial.begin(300);
}

void loop() {
  time = millis();
  convertSensor;
  extremeControl;
  printInformation;
  
}
void convertSensor(){
  valTemp = analogRead(sensorTemp);
  valLight = analogRead(sensorLight);
  valUmity = analogRead(sensorUmity);
  valTemp = map(valTemp, 0, 1023, 0, 100);
  valLight = map(valLight, 0, 1023, 100, 0);
  valUmity = map(valUmity, 0, 1023, 0, 100);
  }
void extremeControl(){
  if( !8>valTemp>26 || !5>valLight>75 || !8>valUmity>40){
    tone(snoozer,200,200);  

    delay(200);
    }
     
  }

void printInformation(){
  Serial.println("Level Light:  ");
  Serial.println(valLight);
  Serial.println("Level Temperatura:  " + valTemp);
  Serial.println(valTemp);
  Serial.println("Level Umity:  " + valUmity);
  Serial.println(valUmity);
  }
