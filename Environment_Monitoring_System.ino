/*
AQI = 5 | A1 
Gas Sensor = 4 | A0
DHT11 = 2
LDR = 3
Rain Drop = A5
*/

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define DHTPIN 2         // DHT11 data pin
#define DHTTYPE DHT11    // DHT11 sensor type
#define LDRPIN A0        // LDR sensor pin
#define RAINPIN 3        // Rain Drop Sensor pin
#define MQ6PIN A0        // MQ6 Gas Sensor pin
#define MQ135PIN 5      // MQ135 Air Quality Sensor pin

#define BLYNK_TEMPLATE_ID "TMPL3ri6UtCcJ" //Template ID
#define BLYNK_TEMPLATE_NAME "Tejas_Kadam" //Templete Name (or try Tejas kadam)
#define BLYNK_AUTH_TOKEN "LYl9k_uFZBnYy-LdQqwLfRzL5IR029QX" //Authentication Token
#define DHT11_PIN 2

char auth[] = "LYl9k_uFZBnYy-LdQqwLfRzL5IR029QX";
char ssid[] = "teja_2002"; //Wifi username
char pass[] = "teja2002"; //wifi password

DHT dht_(DHTPIN, DHTTYPE);

int sensorValue;
int digitalValue;
int sensorPin1=A0;
int aqiData;
DHT dht11(DHT11_PIN, DHT11);
#define DO_PIN 3  // Arduino's pin connected to D3 pin of the ldr module
#define ledPin 6
#define sensorPin A0

void setup()
{
  Serial.begin(9600); // sets the serial port to 9600
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  pinMode(sensorPin1,INPUT); //Air Quality Index                         
  dht11.begin(); // initialize the sensor
  pinMode(DO_PIN, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop()
{
  aqiSensor();
  gasSensor();
  dht();
  ldrsensor();
  rainDrop();
  delay(1000);
  Serial.println("********************************************************************************************************");
  blynk_cloud();

}

void aqiSensor()
{
  aqiData = analogRead(sensorPin1);       
  Serial.print("The Air Quality Index is:");
  Serial.println(aqiData);
}


void gasSensor(){
  sensorValue = analogRead(0); // read analog input pin 0
  digitalValue = digitalRead(2);
  if (sensorValue > 400)
  {
    digitalWrite(13, HIGH);
  }
  else
  digitalWrite(13, LOW);
  Serial.print("Gas Value is: ");
  Serial.println(sensorValue, DEC); // prints the value read
  //delay(1000); // wait 100ms for next reading
}

void dht(){
 // read humidity
  float humi  = dht11.readHumidity();
  // read temperature as Celsius
  float tempC = dht11.readTemperature();
  // read temperature as Fahrenheit
  float tempF = dht11.readTemperature(true);

  // check if any reads failed
  if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT11 sensor!");
  } else {
    Serial.print("DHT11.... Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  "); 

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    Serial.print(tempF);
    Serial.println("°F");
}
}

void ldrsensor(){
  int lightState = digitalRead(DO_PIN);

  if (lightState == HIGH)
    Serial.println("The light is NOT present");
  else
    Serial.println("The light is present");

    //delay(1000);
}

void rainDrop(){
   int sensorValue = readSensor(); // Read the sensor value

  Serial.print("Rain Status: ");
  //Serial.println(sensorValue);  // to display the value of raindrop sensor, uncomment this line.

  if (sensorValue > 100) {
    Serial.println("It's raining");
  } 
  else {
    Serial.println("It's not raining");
  }
  //delay(1000);
}

int readSensor() {
  int sensorValue = analogRead(sensorPin);  // Read the analog value from sensor
  int outputValue = map(sensorValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data
  analogWrite(ledPin, outputValue); // generate PWM signal
  return outputValue;             // Return analog rain value
}

 


void dht_setup() {
  Serial.begin(9600);
  Blynk.begin('TMPL3ri6UtCcJ*', 'tejas2002', 'tejas2002');
  dht.begin();
}

void blynk_cloud() {
  Blynk.run();
  
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int lightIntensity = analogRead(LDRPIN);
  int rainValue = digitalRead(RAINPIN);
  int gasValue = analogRead(MQ6PIN);
  int airQuality = analogRead(MQ135PIN);

  Blynk.virtualWrite(V1, temperature);   // Send temperature data to V1
  Blynk.virtualWrite(V2, humidity);      // Send humidity data to V2
  Blynk.virtualWrite(V3, lightIntensity);// Send light intensity data to V3
  Blynk.virtualWrite(V4, rainValue);     // Send rain value data to V4
  Blynk.virtualWrite(V5, gasValue);      // Send gas value data to V5
  Blynk.virtualWrite(V6, airQuality);    // Send air quality data to V6

  delay(2000); // Update sensor readings every 2 seconds
}
