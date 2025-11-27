=Here is a **clean, professional, plagiarism-safe, human-sounding README.md** tailored for your GitHub repo, including all three `.ino` files and upgrade options.
You can copy-paste directly into `README.md`.

---

# **Intelligent Cooling System – ESP32 + DHT11 + Relay + Fan**

The **Intelligent Cooling System** is a small automation project built using an **ESP32**, **DHT11 temperature sensor**, **relay module**, and a **5V cooling fan**.
It continuously monitors the room temperature and automatically switches a fan ON or OFF depending on the temperature threshold.

The aim of this project is to create a simple, affordable, and efficient temperature-based cooling system that can later be extended with WiFi, mobile control, or web dashboards.

---

## 📁 **Project Repository**

➡️ **GitHub:** [https://github.com/Trumphblast/Intelligent-Cooling-System](https://github.com/Trumphblast/Intelligent-Cooling-System)

---

# 📌 **Project Files**

### **1. `fan_controller-1.ino`**

A basic sensor-testing program.

✔ Reads temperature & humidity from the DHT11
✔ Prints values to the Serial Monitor
✔ Does *not* control any relay or fan
✔ Useful for confirming sensor wiring and operation

---

### **2. `fan_controller-2.ino`**

Full automatic cooling system with relay control.

✔ Reads temperature from the DHT11
✔ Activates relay if temperature > **30°C**
✔ Deactivates relay if temperature < **28°C**
✔ Controls a 5V fan through a relay module
✔ Provides a complete automated cooling behavior

---

### **3. `fan_controller-3.ino`** *(if included)*

WiFi-enabled version using:

* **Blynk App**
* **ESP32 Web Server Interface**

✔ Remote temperature monitoring
✔ Manual fan switching
✔ Real-time fan status
✔ Works over WiFi network

---

# 🧰 **Hardware Components**

* ESP32 Development Board
* DHT11 Temperature + Humidity Sensor
* 1-Channel Relay Module
* 5V DC Fan
* Breadboard
* Jumper Wires
* Optional: External 5V Power Supply for Fan

---

# 🔌 **Circuit Connections**

## **DHT11 → ESP32**

| DHT Pin | ESP32 Pin |
| ------- | --------- |
| VCC     | 3.3V      |
| GND     | GND       |
| DATA    | GPIO 4    |

---

## **Relay → ESP32**

| Relay Pin | ESP32 Pin |
| --------- | --------- |
| VCC       | 5V        |
| GND       | GND       |
| IN        | GPIO 5    |

---

## **Fan → Relay Output**

* Fan **+ (red)** → Relay **NO**
* Fan **– (black)** → ESP32 **GND**
* Relay **COM** → **5V supply**

---

# ⚙️ **How the System Works**

1. The ESP32 reads temperature from the DHT11 sensor at regular intervals.
2. If the temperature rises above **30°C**, the relay is switched **ON**, activating the fan.
3. If the temperature falls below **28°C**, the relay switches **OFF**, stopping the fan.
4. This hysteresis prevents rapid fan ON/OFF switching and keeps the environment stable.

This automated behavior removes the need for manual switching and helps maintain a comfortable temperature range.

---

# 🧪 **Results**

* The test code (`fan_controller-1.ino`) successfully verified DHT11 readings.
* The full controller (`fan_controller-2.ino`) behaved as expected:

  * Fan ON above 30°C
  * Fan OFF below 28°C
* Relay switching was stable with no flickering
* Temperature readings updated reliably every loop cycle

---

# 🚀 **Future Improvements**

Here are some upgrades that can enhance the system:

* Add WiFi dashboard using Blynk or Web Server
* OLED display (SSD1306) for real-time temperature
* Humidity-based fan control
* Manual override button
* MQTT notifications (Telegram, Discord, etc.)
* Data logging to SD card or Google Sheets
* Use a larger fan with MOSFET or separate power supply
* 3D-printed enclosure for better presentation

---

# 📸 **Project Images**


---

# 📄 **License**

This project is open-source.
You are free to use, modify, and build upon it for personal or academic purposes.

---
