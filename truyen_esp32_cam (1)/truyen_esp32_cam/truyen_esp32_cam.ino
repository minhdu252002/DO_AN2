#include "DHT.h"
#include "esp_camera.h"
#include <FirebaseESP32.h>
#include <HardwareSerial.h>

// Define Wi-Fi and Firebase credentials
#define WIFI_SSID "C678-D678"
#define WIFI_PASSWORD "88888888"
#define DATABASE_URL "ttiot-4f1af-default-rtdb.firebaseio.com"

// Define pins and sensor type for DHT sensor
#define DHTPIN 2       // Pin connected to DHT11 on ESP32-CAM
#define DHTTYPE DHT11  // Type of DHT sensor

// Initialize sensor object
DHT dht(DHTPIN, DHTTYPE);

// Initialize pump and light states
//bool maybom1 = false;
//bool maybom2 = false;
int maybom1 = 0;
int maybom2 = 0;

//bool den1 = false;
int den1 = 0;
// Initialize Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Initialize UART for camera
HardwareSerial serialcam(0);
unsigned long sendDataPrevMillis = 0;

void setup() {
  Serial.begin(9600);
  serialcam.begin(9600);
  
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  
  // Initialize Firebase
  config.database_url = DATABASE_URL;
  config.signer.test_mode = true;
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.begin(&config, &auth);
  
  // Start DHT sensor
  dht.begin();
}

void loop() {
  delay(1000);  // Wait 1 second between readings

  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Control pump 1 based on temperature
  if (temperature > 24) {
    maybom1 = 1;
  } else {
    maybom1 = 1;
  }

  // Control pump 2 based on humidity
  if (humidity < 70) {
    maybom2 = 1;
  } else {
    maybom2 = 0;
  }

  // Control light based on conditions
  if (temperature > 24.0 || humidity < 70.0) {
    den1 = 1;
  } else {
    den1 = 0;
  }

  // Update Firebase with sensor readings and control states
  if (millis() - sendDataPrevMillis > 1000) {
    sendDataPrevMillis = millis();
    
    Firebase.setFloat(fbdo, "/Vuon_cai/Nhietdo", temperature);
    Firebase.setFloat(fbdo, "/Vuon_cai/Doam", humidity);
  }

  // Update Firebase with pump and light states
  Firebase.setBool(fbdo, "/thietbi1/maybom1", maybom1);
  Firebase.setBool(fbdo, "/thietbi3/maybom2", maybom2);
  Firebase.setBool(fbdo, "/thietbi2/den1", den1);

  // Send data to Arduino Uno via UART
  serialcam.print("T:");
  serialcam.print(temperature);
  serialcam.print(", H:");
  serialcam.print(humidity);
  serialcam.println("%");
}
