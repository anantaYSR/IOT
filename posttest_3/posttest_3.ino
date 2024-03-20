#include <ThingSpeak.h> //library platform thingspeak
#include <ESP8266WiFi.h> //library Wifi NodeMCU ESP8266
#include <WiFiClient.h> //library client
#include <DHT.h> //library DHT
#include <ESP8266WebServer.h> //library webserver


#define DHTPIN D5 // mendefinisikan pin D4 sebagai pin DHT
#define DHTTYPE DHT11 //mendefinisikan type dari DHT
DHT dht(DHTPIN, DHTTYPE);


ESP8266WebServer server(80);
String webpage;  //mengatur pin DHT dan type DHT


const int BUZZER = D1;
const int LED_RED = D2;
const int LED_GREEN = D3;
const int LED_YELLOW = D4;

unsigned int value;
const char *ssid = "Bayu"; //setting nama wifi 
const char *password = "bayu1234"; //password wifi 
WiFiClient client; //mengecek jika client sudah terhubung

unsigned long channel = 2477486; //ID akun ThingSpeak
const char * myWriteAPIKey = "UDVQWIUCH3VRRF0I"; //memasukan apikey

void setup() {
  Serial.begin(115200); //memulai komunikasi serial dengan baud rate 115200
  dht.begin(); //memulai sensor DHT.
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUZZER, OUTPUT);


  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //menghubungkan dengan wifi router
  while (WiFi.status() != WL_CONNECTED){ //Cek koneksi Wifi.
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  
  ThingSpeak.begin(client);
  noTone(BUZZER);
}

void loop() {
  float temperature = dht.readTemperature(); //membaca suhu pada sensor DHT11
  float tempFahrenheit = dht.readTemperature(true);
  float tempKelvin = temperature + 273.15;
  Serial.print("Temperature Value is :");
  Serial.println(temperature);
  ThingSpeak.writeField(channel, 1, temperature, myWriteAPIKey);
  ThingSpeak.writeField(channel, 2, tempFahrenheit, myWriteAPIKey);
  ThingSpeak.writeField(channel, 3, tempKelvin, myWriteAPIKey);
  if(temperature <= 30){
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER);
  } else if(temperature > 30 && temperature < 36){
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER);
  } else if(temperature >= 36) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    tone(BUZZER, 1000);
  }
}
