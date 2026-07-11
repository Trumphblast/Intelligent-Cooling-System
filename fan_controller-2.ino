/*************************************************
   ESP32 Smart Fan Controller
   Compatible with:
   - ESP32 Arduino Core 3.x
   - Blynk IoT
   - Adafruit DHT Library
*************************************************/

/*************** Blynk Settings ******************/
#define BLYNK_TEMPLATE_ID "TMPL4n3wdaUF0"
#define BLYNK_TEMPLATE_NAME "ESP32 Fan Controller"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

/*************** WiFi Credentials ****************/
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

/*************** Libraries ***********************/
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

/*************** Pin Configuration ***************/
#define DHTPIN      4
#define DHTTYPE     DHT11
#define RELAY_PIN   5

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

/*************** Virtual Pins ********************/
#define V_TEMP      V1
#define V_HUM       V2
#define V_MANUAL    V3
#define V_FEEDBACK  V4

/*************** Temperature Limits **************/
const float ON_TEMP  = 30.0;
const float OFF_TEMP = 28.0;

/*************** Variables ***********************/
bool manualOverride = false;
bool fanState = false;

/*************************************************
   Read Sensor & Automatic Fan Control
*************************************************/
void sendReadings()
{
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println("[ERROR] Failed to read DHT11 sensor.");
        return;
    }

    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.print(" °C");

    Serial.print("   Humidity : ");
    Serial.print(humidity);
    Serial.println(" %");

    // Send values to Blynk if connected
    if (Blynk.connected())
    {
        Blynk.virtualWrite(V_TEMP, temperature);
        Blynk.virtualWrite(V_HUM, humidity);
    }

    // Automatic mode
    if (!manualOverride)
    {
        if (temperature >= ON_TEMP)
        {
            fanState = true;
        }
        else if (temperature <= OFF_TEMP)
        {
            fanState = false;
        }

        digitalWrite(RELAY_PIN, fanState ? LOW : HIGH);

        Serial.print("Fan (AUTO): ");
        Serial.println(fanState ? "ON" : "OFF");

        if (Blynk.connected())
        {
            Blynk.virtualWrite(V_FEEDBACK, fanState);
        }
    }
}

/*************************************************
   Manual Fan Control from Blynk
   V3:
   0 = OFF
   1 = ON
   2 = AUTO
*************************************************/
BLYNK_WRITE(V_MANUAL)
{
    int value = param.asInt();

    if (value == 2)
    {
        manualOverride = false;
        Serial.println("Switched to AUTO mode");
        return;
    }

    manualOverride = true;

    fanState = value;

    digitalWrite(RELAY_PIN, fanState ? LOW : HIGH);

    Serial.print("Fan (MANUAL): ");
    Serial.println(fanState ? "ON" : "OFF");

    if (Blynk.connected())
    {
        Blynk.virtualWrite(V_FEEDBACK, fanState);
    }
}

/*************************************************
   Sync Blynk after reconnect
*************************************************/
BLYNK_CONNECTED()
{
    Serial.println("Connected to Blynk");
    Blynk.syncVirtual(V_MANUAL);
}

/*************************************************
   Setup
*************************************************/
void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("==================================");
    Serial.println(" ESP32 Smart Fan Controller");
    Serial.println("==================================");

    dht.begin();
    delay(2000);

    pinMode(RELAY_PIN, OUTPUT);

    // Relay OFF (Active LOW relay)
    digitalWrite(RELAY_PIN, HIGH);

    Serial.println("Connecting to WiFi...");

    WiFi.begin(ssid, pass);

    unsigned long startAttempt = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - startAttempt < 15000)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi Connected");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());

        Blynk.config(BLYNK_AUTH_TOKEN);
        Blynk.connect(5000);
    }
    else
    {
        Serial.println("WiFi connection failed.");
        Serial.println("Running in OFFLINE automatic mode.");
    }

    sendReadings();

    timer.setInterval(4000L, sendReadings);
}

/*************************************************
   Loop
*************************************************/
void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        static unsigned long lastReconnect = 0;

        if (millis() - lastReconnect > 10000)
        {
            lastReconnect = millis();

            Serial.println("Reconnecting WiFi...");
            WiFi.disconnect();
            WiFi.begin(ssid, pass);
        }
    }
    else
    {
        if (!Blynk.connected())
        {
            Blynk.connect(1000);
        }

        Blynk.run();
    }

    timer.run();
}
