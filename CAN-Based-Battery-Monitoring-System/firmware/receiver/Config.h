// Config.h
#ifndef CONFIG_H
#define CONFIG_H

// ——— Blynk Template & Auth ———
// Must come _before_ any Blynk header include!
#define BLYNK_TEMPLATE_ID    "TMPL3iPD8ULcC"
#define BLYNK_TEMPLATE_NAME  "Batterymonitor"
#define BLYNK_AUTH_TOKEN     "OaAUyK_scBHwVcgTsj5tVppTqJl3BgoQ"

// ——— Wi-Fi Credentials ———
static constexpr char WIFI_SSID[] = "username";
static constexpr char WIFI_PASS[] = "password";

// ——— Virtual Pin Numbers ———
// Use literal ints so you don’t depend on V0…V5 macros
static constexpr int VP_VOLTAGE = 0;
static constexpr int VP_CURRENT = 1;
static constexpr int VP_ENERGY  = 2;
static constexpr int VP_BILL    = 3;
static constexpr int VP_RATE    = 4;
static constexpr int VP_SOC     = 5;

// ——— CAN IDs ———
static constexpr uint32_t CAN_ID_VI  = 0x101;
static constexpr uint32_t CAN_ID_SOC = 0x102;

// ——— Billing ———
static constexpr float BILLING_RATE         = 8.0f;  // ₹/kWh
static constexpr float HIGH_BILL_THRESHOLD  = 10.0f; // ₹

// ——— Sensor Calibration ———
static constexpr float V_MIN = 0.0f;
static constexpr float V_MAX = 5.0f;
static constexpr float I_MIN = -10.0f;
static constexpr float I_MAX = 10.0f;

#endif // CONFIG_H
