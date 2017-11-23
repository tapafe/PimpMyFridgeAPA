// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include "math.h"

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)


DHT dht(DHTPIN, DHTTYPE);

//wanted temperature
int temp_want=18; // celsius


//pin MOSFET
int pinMosfet=7;

//begin PT100 conf
int valeur; //valeur retournée par la thermistance 
float temperature; //valeur finale 
int B=3975; 
int R1=10000; //resistance appliquée a notre arduino
float resistance; //resistance de la thermistance 
//

void setup() {
  Serial.begin(9600);
  //Serial.println("DHTxx test!");
  pinMode(pinMosfet,OUTPUT);
  dht.begin();
  pinMode(8,OUTPUT);
}

double Thermister(int RawADC) {
  double Temp;
    Temp = log(((10240000/RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;           // Convert Kelvin to Celcius
  //Serial.println(Temp);
 return Temp;
}

  


void loop() {
  
  delay(1000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  //point of rose calcul
  float k = ((237,7)*t)/((17,7+t)+log(h));
  float rose = (237,7*k ) /(17,7-k);
  //




  //PT100 calcul
 valeur=analogRead(0);
 resistance=(float)(1023-valeur)*R1/valeur; //calcul de la resistance de la thermistance 
 temperature=1/(log(resistance/R1)/B+1/298.15)-273.15; //calcul de la température en kelvin puis convertie en °C
  //

  float interval_temp = temp_want*0.03; // we want +/- 3%
  float temp_want_plus=temp_want+interval_temp;
  float temp_want_minus=temp_want-interval_temp;



  //Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" ");
  //Serial.print(" %\t");
  //Serial.print("Temperature interne: ");
  Serial.print(t);
  Serial.print(" ");
  //Serial.print("Temperature interne plaque: ");
  Serial.print(Thermister(analogRead(1)));
  Serial.print(" ");
  //Serial.print(" °C ");
  //Serial.print("Temperature externe: ");
  Serial.print(temperature);
  Serial.print(" ");
  //Serial.print(" °C ");
  //Serial.print("point de rosé ");
  Serial.println(rose);
  

  if (t>temp_want_plus)
    {
        //action when temperature is too hot
                
                digitalWrite(pinMosfet,LOW); //le moteur se lance
               digitalWrite(8,HIGH); //la led se lance
    }

  if (t<=temp_want_plus)
    {
        //action when temperature is correct or coldest
               
                
                digitalWrite(pinMosfet,HIGH); //le moteur se coupe
                digitalWrite(8,LOW); //la led se coupe
    }


//control by serial.read

     if (Serial.available() > 0) {
     
      
                // read the incoming byte:
               int incomingByte = Serial.parseInt();
               temp_want=incomingByte;
               
                        }
  //temporaire a supprimer , pour test
  
  //fin du temporaire
}
