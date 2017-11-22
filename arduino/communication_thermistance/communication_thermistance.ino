


void setup(void) {
  Serial.begin(9600);
}

double Thermister(int RawADC) {
  double Temp;
  
  Temp = log(((10240000/RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;           // Convert Kelvin to Celcius
  Serial.println(Temp);
 
}



void loop(void) {
  Thermister(analogRead(1));
  //printTemp();
  delay(500);
}


