#include <DHT.h> //libreria sensor de temperatura
#include <LiquidCrystal_I2C.h> //libreria pantalla 


#define DHTPIN 2  // Pin al que está conectado el sensor DHT11 (humedad)
#define DHTTYPE DHT11  //modelo e sensor de humedad

//Caracteristicas de la pantalla
#define ANCHURA_LCD 16
#define ALTURA_LCD 2
#define DIRECCION_LCD 0x27
LiquidCrystal_I2C pantalla(DIRECCION_LCD, ANCHURA_LCD, ALTURA_LCD);


int sensorLluvia = A1; //pin al que está conectado el sensor de lluvia
int sensorHumedad = A0; //pin del sensor de humedad
const int bomba = A3; //pin de la bomba
int buzzer = 3; //pin el buzzer


DHT dht(DHTPIN, DHTTYPE); // Inicializar el sensor DHT

void setup() {
  Serial.begin(9600);
  dht.begin();  // Inicializar el sensor DHT

//inicializar la pantalla
  pantalla.init();
  pantalla.backlight();
  pantalla.clear();

  pinMode(bomba, OUTPUT); //definir que la bomba es salida
  pinMode(buzzer, OUTPUT); //definir que el buzzer es salida

}
  
void loop() {
  // Leer la temperatura y humedad del sensor DHT
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  int valorLluvia = analogRead(sensorLluvia); // Leer el valor del sensor de lluvia
  int valorHumedad = analogRead(sensorHumedad); //Leer el valor del sensor de humedad

  // Imprimir en el monitor los valores de humead y temperatura
  Serial.print("Temperatura °C: ");
  Serial.println(temperatura);
  Serial.print("Humedad %: ");
  Serial.println(humedad);
  
  
  //Sernsor de lluvia: especificar los valores del sensor para que imprima texto en el monitor sobre estos valores
  //Serial.print(valorLluvia); <-- en caso de querer imprimir el valor del sensor
  if (valorLluvia > 950) {
    Serial.println("No hay lluvia");
  } else if(valorLluvia <=950 && valorLluvia >=600) {
    Serial.println("LLovizna");
  } else if (valorLluvia <=600 && valorLluvia >300) {
    Serial.println("Llueve");
  } else {
    Serial.println("Lluvia fuerte");
  }



//Sensor de Humedad: especificar los valores del sensor para que imprima texto en el monitor sobre estos valores
 // Serial.print(valorHumedad); <-- en caso de querer imprimir el valor del sensor
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
  pantalla.setCursor(0, 0); // X, Y  posicionar el cursor en el recuadro requerido
  pantalla.print("Temp: "); //texto a mostrar en la pantalla
  pantalla.print(temperatura); //información mostrada en la pantalla
  pantalla.print("C ");

  pantalla.setCursor(0, 1);  // X, Y  posicionar el cursor en el recuadro requerido
  pantalla.print("Hum: ");
  pantalla.print(humedad);
  pantalla.print("% ");

  
//BOMBA
  if (valorLluvia < 950) {
    digitalWrite(bomba, HIGH); //si el sensor de lluvia detecta lluvia la bomba se apaga
  } else {
      if (valorHumedad > 325) {// si el sensor de humedad tiene poca humedad, la bomba se prende
    digitalWrite(bomba, HIGH);
  } else {
    digitalWrite(bomba, LOW);
  }

  }

//Buzzer
  if (valorLluvia <=950 ) { //si el sensor de lluvia detecta lluvia, el buzzer lanza una alarma
    tone(buzzer, 10); // Tono del buzzer
  } else{
    noTone(buzzer); //sino, buzzer apagado
  }
  
  delay(1000); // retardo de 1 segundo entre loops
}