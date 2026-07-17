// ESP32 Serial -> Blynk bridge + Alerts
// Reads lines from Serial2 (RX=21, TX=19), parses temperature floats
// Sends numeric temperature to Blynk AND fires multiple event notifications

#define BLYNK_TEMPLATE_ID "TMPL38w3LGMoa"
#define BLYNK_TEMPLATE_NAME "Temperaturedata"
#define BLYNK_AUTH_TOKEN "xWVw1fbPrW4UahFX1OqjMcMFAuz9ErJw"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "Token";
char ssid[] = "user";
char pass[] = "password";

// UART pins
#define UART_RX 21
#define UART_TX 19
#define UART_BAUD 115200

// Virtual pins
const int VPIN_TEMP = V0;
const int VPIN_TERM = V3;

String lineBuffer = "";
unsigned long lastBlynkSend = 0;
const unsigned long BLYNK_SEND_INTERVAL_MS = 500;

// -----------------------------------------------------------------------
// 🔔 FUNCTION: Fire Blynk alerts based on temperature
// -----------------------------------------------------------------------
void checkTemperatureForAlerts(float temp) {

  // Dummy event 1: general data notification
  Blynk.logEvent("data_received", String("New data: ") + temp);

  // Dummy event 2: custom alert #1
  if (temp > 20) {
    Blynk.logEvent("custom_alert_1", "Temp crossed 20°C");
  }

  // Dummy event 3: custom alert #2
  if (temp > 22) {
    Blynk.logEvent("custom_alert_2", "Temp crossed 22°C");
  }

  // Real alert thresholds (edit as needed)
  if (temp > 40) {
    Blynk.logEvent("temp_high", String("High Temp: ") + temp);
  }

  if (temp > 60) {
    Blynk.logEvent("temp_critical", String("CRITICAL Temp: alert help!!!!! "));
  }

  // Sensor error testing
 // if (temp < -10 || temp > 150) {
   // Blynk.logEvent("sensor_error", "Sensor sent invalid data");
  //}
}

// -----------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("\nESP32 UART -> Blynk with Alerts");

  Serial2.begin(UART_BAUD, SERIAL_8N1, UART_RX, UART_TX);

  WiFi.mode(WIFI_STA);
  Blynk.begin(auth, ssid, pass);
}

// Extract last float from a string
bool extractLastFloat(const String &s, float &result) {
  int n = s.length();
  int end = -1;
  int start = -1;

  for (int i = n - 1; i >= 0; i--) {
    char c = s.charAt(i);
    if ((c >= '0' && c <= '9') || c == '.') {
      end = i;
      break;
    }
  }
  if (end == -1) return false;

  for (int i = end; i >= 0; i--) {
    char c = s.charAt(i);
    if ((c >= '0' && c <= '9') || c == '.') {
      start = i;
    } else break;
  }

  if (start < 0) return false;

  String token = s.substring(start, end + 1);
  token.trim();

  result = token.toFloat();
  return true;
}

// -----------------------------------------------------------------------

void loop() {
  if (Blynk.connected()) Blynk.run();

  while (Serial2.available()) {
    char c = Serial2.read();
    Serial.write(c);

    if (c == '\n' || c == '\r') {
      String ln = lineBuffer;
      ln.trim();
      if (ln.length() > 0) {
        processLine(ln);
      }
      lineBuffer = "";
    } else {
      lineBuffer += c;
    }
  }

  while (Serial.available()) {
    Serial2.write(Serial.read());
  }
}

void processLine(const String &ln) {
  Serial.print("[LINE] ");
  Serial.println(ln);

  if (Blynk.connected()) {
    Blynk.virtualWrite(VPIN_TERM, ln);
  }

  float temp = 0;
  if (extractLastFloat(ln, temp)) {
    unsigned long now = millis();
    if (now - lastBlynkSend >= BLYNK_SEND_INTERVAL_MS) {
      lastBlynkSend = now;

      Blynk.virtualWrite(VPIN_TEMP, temp);

      // 🔔 send alerts
      checkTemperatureForAlerts(temp);
    }
  }
}