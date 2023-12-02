#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define motor11 2 // Motor 1 control pin 1
#define motor12 3 // Motor 1 control pin 2
#define motor21 4 // Motor 2 control pin 1
#define motor22 5 // Motor 2 control pin 2
#define btn1 6 //Button 1 pin
#define btn2 7 //Button 1 pin
#define ledPin 8

SoftwareSerial espSerial(11, 12); // Chân 0 (RX), 1 (TX) của Arduino kết nối với chân TX, RX của ESP32-CAM
LiquidCrystal_I2C lcd(0x27, 16, 2); // Địa chỉ LCD và kích thước 

const int tempThreshold = 25; // Temperature threshold
const int humidityThreshold = 60; // Humidity threshold
int tt1 = 0, tt2 = 0;
float temperature = 0;
float humidity = 0;
 


void setup() {
  Serial.begin(9600);       // Khởi tạo Serial Monitor của Arduino
  espSerial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(motor11, OUTPUT);
  pinMode(motor12, OUTPUT);
  pinMode(motor21, OUTPUT);
  pinMode(motor22, OUTPUT);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  digitalWrite(motor11, LOW);
  digitalWrite(motor12, LOW);
  digitalWrite(motor21, LOW);
  digitalWrite(motor22, LOW);  
  pinMode(ledPin, OUTPUT); // Set LED pin as output
  digitalWrite(ledPin, LOW); // Turn off LED initially
}

void loop() {

if (espSerial.available()) {
    String receivedData = espSerial.readString();
    Serial.println("Received data: " + receivedData);
    
    int tempStart = receivedData.indexOf("T:") + 2; // Vị trí bắt đầu của nhiệt độ
    int tempEnd = receivedData.indexOf(",", tempStart); // Vị trí kết thúc của nhiệt độ
    int humStart = receivedData.indexOf("H:") + 2; // Vị trí bắt đầu của độ ẩm
    int humEnd = receivedData.indexOf("%", humStart); // Vị trí kết thúc của độ ẩm
    
    if (tempStart != -1 && tempEnd != -1 && humStart != -1 && humEnd != -1) {
      String tempString = receivedData.substring(tempStart, tempEnd);
      String humidityString = receivedData.substring(humStart, humEnd);
      
      float temperature = tempString.toFloat();
      float humidity = humidityString.toFloat();
      if(temperature>24.1){
        tt1 = 1;        
      }
      else{
        tt1 =0;
      }
      if(humidity<70.0){
        tt2 = 1;        
      }
      else{
        tt2 =0;
      }
      // Add LED control based on temperature and humidity thresholds
  if (temperature > 24.0 || humidity < 70.0) {
    digitalWrite(ledPin, HIGH); // Turn on the LED
  } else {
    digitalWrite(ledPin, LOW); // Turn off the LED
  }
      // Hiển thị nhiệt độ và độ ẩm lên LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temperature);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Humidity: ");
      lcd.print(humidity);
      lcd.print("%");
    }
}
  if(digitalRead(btn1)==LOW){
    delay(50);
    if(digitalRead(btn1)==LOW){
      tt1 = !tt1;
      while(digitalRead(btn1)==LOW);
    }
  }
  if(digitalRead(btn2)==LOW){
    delay(50);
    if(digitalRead(btn2)==LOW){
      tt2 = !tt2;
      while(digitalRead(btn2)==LOW);
    }
  }
  if(tt1 == 1){
    digitalWrite(motor11, HIGH);
    digitalWrite(motor12, LOW);
  }
  else{
     digitalWrite(motor11, LOW);
  }
    if(tt2 == 1){
    digitalWrite(motor21, HIGH);
    digitalWrite(motor22, LOW);
  }
  else{
    digitalWrite(motor21, LOW);
    digitalWrite(motor22, LOW); 
  }
  
}
