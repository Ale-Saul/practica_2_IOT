#include <WiFi.h>
#include <WiFiClient.h>
#include <array>

const char* ssid = "MiWiFi"; // Name WiFi Network
const char* password = "CualContrasena.24"; // Password WiFi Network
const char* serverIP = "192.168.31.149"; // Server IP 
const int serverPort = 12345; // Server Port

const int PIN_RED = 27;
const int PIN_WHITE = 12;
const int PIN_BLUE = 14;

//Creation of LED class
class LED {
private:
    int pin;
public:
    LED(int pin) : pin(pin) {
        pinMode(pin, OUTPUT);
    }
    void turn_On_or_OFF(bool value) {
        digitalWrite(pin, value);
    }
};

void internet_Connection() {
    Serial.println("Conectando a WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Conexion exitosa");
}

std::array<int, 3> server_Connection_actuator() {
    // Conecction to server
    WiFiClient client;
    if (!client.connect(serverIP, serverPort)) {
        Serial.println("Error al conectar con el servidor");
        delay(1000);
    }
    client.println("get");
    // Wait and read server answer
    while (client.available() == 0) {
        delay(1);
    }
    String response = client.readStringUntil('\n');
    // Eliminate [ ]
    response.replace("[", "");
    response.replace("]", "");
    // Divide answer in tokens separated by comas
    std::array<int, 3> array_actuators;
    int index = 0;
    char* token = strtok((char*)response.c_str(), ",");
    while (token != NULL && index < 3) {
        array_actuators[index++] = atoi(token);
        token = strtok(NULL, ",");
    }
    return array_actuators;
}

void manageLEDs(std::array<int, 3> array_actuators, LED& red, LED& white, LED& blue) {
    red.turn_On_or_OFF(array_actuators[0]);
    white.turn_On_or_OFF(array_actuators[1]);
    blue.turn_On_or_OFF(array_actuators[2]);
}

void setup() {
    Serial.begin(9600);
    internet_Connection();
}

void loop() {
    LED red(PIN_RED), white(PIN_WHITE), blue(PIN_BLUE);
    std::array<int, 3> array_actuators = server_Connection_actuator();
    manageLEDs(array_actuators, red, white, blue);
}
