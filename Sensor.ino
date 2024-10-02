#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "MiWiFi"; // Name WiFi Network
const char* password = "CualContrasena.24"; // Password WiFi Network
const char* serverIP = "192.168.31.149"; // Server IP 
const int serverPort = 12345; // Server Port

const int PIN_TRIGGER = 13;
const int PIN_ECHO = 12;

//Creation of Ultrasonic Sensor class
class UltrasonicSensor {
  private:
    int triggerPin;
    int echoPin;
  public:
    UltrasonicSensor(int triggerPin, int echoPin) : triggerPin(triggerPin), echoPin(echoPin) {}
    double readUltrasonicDistance() {
      pinMode(triggerPin, OUTPUT);  // Clear the trigger
      digitalWrite(triggerPin, LOW);
      delayMicroseconds(2);
      // Sets the trigger pin to HIGH state for 10 microseconds
      digitalWrite(triggerPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(triggerPin, LOW);
      pinMode(echoPin, INPUT);
      // Reads the echo pin, and returns the sound wave travel time in microseconds
      return 0.01723 * pulseIn(echoPin, HIGH);
    }
};

void internet_Connection(){
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conexion exitosa");
}

void server_Connection_sensor(double distance){
  // Conecction to server
  WiFiClient client;
  if (!client.connect(serverIP, serverPort)) {
    Serial.println("Error al conectar con el servidor");
    delay(1000);
    return;
  }
  // Send data to server ("set" command + distance)
  Serial.println("Enviando distancia al servidor...");
  String command = "set " + String(distance);
  client.println(command);
}

void setup() {
  Serial.begin(9600);
  internet_Connection();
}

void loop() {
  UltrasonicSensor sensor(PIN_TRIGGER, PIN_ECHO);
  double distance = sensor.readUltrasonicDistance();
  Serial.print(distance);
  Serial.println("cm");
  server_Connection_sensor(distance);
  delay(1000);
}