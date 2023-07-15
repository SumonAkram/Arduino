#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#include <SoftwareSerial.h>
SoftwareSerial ser(2, 3);              // RX, TX


int gas_sensor = A0; //Sensor pin 
float m = -0.3376; //Slope 
float b = 0.7165; //Y-Intercept 
float R0 = 7.84; //Sensor Resistance in fresh air from previous code


int CO_sensor = A1; //Sensor pin 
float m1 = -0.6527; //Slope 
float b1 = 1.30; //Y-Intercept 
float R01 = 2.33; //Sensor Resistance


String apiKey = "99QYEPWUESBWFB7R";    // Edit this API key according to your Account


#include <dht.h>
dht DHT;
#define DHT11_PIN A2
void setup() {
  
      // PC to Arduino Serial Monitor
  Serial.begin(115200);   // Arduino to ESP01 Communication
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  Air Quality & ");
  lcd.setCursor(0, 1);
  lcd.print("Weather  Station");
  delay(2000);
  lcd.clear();
  ser.begin(115200);                     // reset ESP8266
ser.println("AT+RST");               // Resetting ESP8266
char inv ='"';
String cmd = "AT+CWJAP";
String Host_Name = "Anas";         // Edit Host_Name
String Password = "112233445566778811";          // Edit 
cmd+= "=";
cmd+= inv;
cmd+= Host_Name;
cmd+= inv;
cmd+= ",";
cmd+= inv;
cmd+= Password;
cmd+= inv;
ser.println(cmd);                    // Connecting ESP8266 to your WiFi Router
  pinMode(gas_sensor, INPUT);
  pinMode(CO_sensor,INPUT);
  pinMode(A3,INPUT); //For DHT Sensor
 } 


void loop() { 
  // put your main code here, to run repeatedly:
  float sensor_volt; //Define variable for sensor voltage 
  float RS_gas; //Define variable for sensor resistance  
  float ratio; //Define variable for ratio
  float sensorValue = analogRead(gas_sensor); //Read analog values of sensor  
  sensor_volt = sensorValue*(5.0/1023.0); //Convert analog values to voltage 
    RS_gas = ((5.0*10.0)/sensor_volt)-10.0; //Get value of RS in a gas
  ratio = RS_gas/R0;  // Get ratio RS_gas/RS_air
  double ppm_log = (log10(ratio)-b)/m; //Get ppm value in linear scale according to the the ratio value  
  double ppm = pow(10, ppm_log); //Convert ppm value to log scale 
  Serial.print("Air Quality = ");
  Serial.println(ppm);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Air Quality:");
      lcd.print(ppm);
      lcd.print(" ppm ");
      delay(2000);
      lcd.clear();


  float sensor_volt1; //Define variable for sensor voltage 
  float RS_gas1; //Define variable for sensor resistance  
  float ratio1; //Define variable for ratio
  float sensorValue1 = analogRead(CO_sensor); //Read analog values of sensor  
  sensor_volt1 = sensorValue1*(5.0/1023.0); //Convert analog values to voltage 
  RS_gas1 = ((5.0*10.0)/sensor_volt1)-10.0; //Get value of RS in a gas
  ratio1 = RS_gas1/R01;  // Get ratio RS_gas/RS_air
  double ppm_log1 = (log10(ratio1)-b1)/m1; //Get ppm value in linear scale according to the the ratio value  
  double ppm1 = pow(10, ppm_log1); //Convert ppm value to log scale 
  Serial.print("CO PPM = ");
  Serial.println(ppm1);


      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("CO:");
      lcd.print(ppm1);
      lcd.print(" ppm ");
      delay(2000);
      lcd.clear();
      
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  float temp = DHT.temperature;
  Serial.println(temp);
  float hum = DHT.humidity;
  Serial.print("Humidity = ");
  Serial.println(hum);


      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temperature:");
      lcd.print(temp);
      lcd.print(" Celcius ");
      lcd.setCursor(0,1);
      lcd.print("Humidity:");
      lcd.print(hum);
      lcd.print(" % ");
      delay(2000);
      lcd.clear();


    String state1=String(ppm);                 // Converting them to string 
  String state2=String(ppm1); 
    String state3=String(temp); 
  String state4=String(hum);
     
  String cmd = "AT+CIPSTART=\"TCP\",\"";          // Establishing TCP connection
  cmd += "184.106.153.149";                       // api.thingspeak.com
  cmd += "\",80\r\n\r\n";                                 // port 80


  ser.println(cmd);
  Serial.println(cmd);


  if(ser.find("Error")){


    Serial.println("AT+CIPSTART error");
    return;
}


String getStr = "GET /update?api_key=";         // prepare GET string


  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(state1);                       // Humidity Data
  getStr +="&field2=";
  getStr += String(state2); 
    getStr +="&field3=";
  getStr += String(state3); 
    getStr +="&field4=";
  getStr += String(state4); 
  getStr += "\r\n\r\n";


  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());                // Total Length of data


  ser.println(cmd);
  Serial.println(cmd);


if(ser.find(">")){


    ser.print(getStr);
    Serial.print(getStr);


  }


  else{


    ser.println("AT+CIPCLOSE");                  // closing connection


    // alert user


    Serial.println("AT+CIPCLOSE");


  }
                                // Update after every 15 seconds


} 
