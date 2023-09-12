#include <DHT.h>
#include <LiquidCrystal_I2C.h>


#define DHTPIN 2         // Pin al que está conectado el sensor DHT11
#define DHTTYPE DHT11   

//PAntalla
#define ANCHURA_LCD 16
#define ALTURA_LCD 2
#define DIRECCION_LCD 0x27
LiquidCrystal_I2C pantalla(DIRECCION_LCD, ANCHURA_LCD, ALTURA_LCD);


int sensorLluvia = A1;
int sensorHumedad = A0;
const int bomba = A3;
int buzzer = 3;


DHT dht(DHTPIN, DHTTYPE); // Inicializar el sensor DHT

void setup() {
  Serial.begin(9600);
  dht.begin();  // Inicializar el sensor DHT

  pantalla.init();
  pantalla.backlight();
  pantalla.clear();

  pinMode(bomba, OUTPUT);
  pinMode(buzzer, OUTPUT);

}
  
void loop() {
  // Leer la temperatura y humedad del sensor DHT
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();



  // Leer el valor del sensor de lluvia
  int valorLluvia = analogRead(sensorLluvia);
  int valorHumedad = analogRead(sensorHumedad);

  // Imprimir los valores en el Monitor Serie
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  //Serial.print("°C");
  Serial.print("Humedad: ");
  Serial.println(humedad);
  //Serial.print("%");
  
  //Sensor de lluvia
  Serial.print(valorLluvia);
  if (valorLluvia > 950) {
    Serial.println("No hay lluvia");
  } else if(valorLluvia <=950 && valorLluvia >=600) {
    Serial.println("LLovizna");
  } else if (valorLluvia <=600 && valorLluvia >300) {
    Serial.println("Llueve");
  } else {
    Serial.println("Lluvia fuerte");
  }



//Sensor de Humedad
  Serial.print(valorHumedad);
  if (valorHumedad < 162) {
    Serial.println("Humedad de la tierra al 0-25%");
  } else if(valorLluvia >=162 && valorLluvia <325) {
    Serial.println("Humedad de la tierra al 26-50%");
  } else if (valorLluvia >=325 && valorLluvia <487) {
    Serial.println("Humedad de la tierra al 51-75%");
  } else {
    Serial.println("Humedad de la tierra al 76-100%");
  }

//Pantalla
  pantalla.setCursor(0, 0); // X, Y
  pantalla.print("Temp: ");
  pantalla.print(temperatura);
  pantalla.print("C ");

  pantalla.setCursor(0, 1);
  pantalla.print("Hum: ");
  pantalla.print(humedad);
  pantalla.print("% ");

  
//BOMBA
  if (valorLluvia < 950) {
    digitalWrite(bomba, HIGH);
  } else {
      if (valorHumedad > 325) {
    digitalWrite(bomba, HIGH);
  } else {
    digitalWrite(bomba, LOW);
  }

    
    //digitalWrite(bomba, LOW);
  }

//Buzzer
  if (valorLluvia <=950 ) {
    tone(buzzer, 10);
  } else{
    noTone(buzzer);
  }
  

  // Agrega aquí la lógica para controlar actuadores o tomar decisiones basadas en los valores de los sensores

  delay(1000); // Puedes ajustar el retardo según tus necesidades
}