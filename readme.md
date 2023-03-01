# Industrial-grade Soil Remote Monitor

```c++
/*
Version:		V1.0
Author:			Vincent
Create Date:	2023/3/1
Note:
	
*/
```

![main](md_pic/front.jpg)


[toc]

# Makerfabs

[Makerfabs home page](https://www.makerfabs.com/)

[Makerfabs Wiki](https://wiki.makerfabs.com/)

# Industrial-grade Soil Remote Monitor

## Intruduce

Product Link ：[Industrial-grade Soil Remote Monitor(Humidity/ Temperature/ PH/ Nitrogen/ Phosphorus/ Potassium)](https://www.makerfabs.com/industrial-grade-soil-remote-monitor.html) 

Wiki Link : [RS485-LoRa Wireless Station](https://www.makerfabs.com/wiki/index.php?title=RS485-LoRa_Wireless_Station) 

With the development of agricultural modernization and the popularization of precision agriculture, the remote monitoring of soil parameters such as temperature, moisture, PH, and so on, becomes more important.

This Industry-grade soil remote monitor intends for these applications of agriculture, it is mainly composed of 2 parts:

**1. Remote monitor station with RS485:** developed by Makerfabs, it converts the RS485 signal, which is usually used in agricultural field applications, to wireless connections WIFI (if there is) or Lora, or Bluetooth.
**2. Industrial-grade Soil Sensor:** unlike some other maker-level soil sensors that only output analog voltage that reflects the soil status/changes but not the absolute value, this sensor measures the soil parameters such Temperature/Moisture/PH/Nitrogen/ Phosphorus/Potassium.., with calculated true output, so users can get to know the real soil parameters.




## Feature

- ESP32 WROOM module onboard with the WiFi and Bluetooth
- Arduino Compatible: You can play it with Arduino IDE
- Onboard LoRa module (433Mhz or 868Mhz or 915Mhz)
- 0.91inch I2C OLED display
- Sensor part is waterproof, completely sealed
- Measuring range: -40℃~80℃，0~100% moisture
- Measuring accuracy: ±0.5℃，±3% moisture
- Communication distance: Lora: 2km or more; WIFI: No such parameter
- Power by Micro USB, battery, or DC 5V
- 1000mAh battery, charger IC onboard



# Code



## Compiler Options

**If you have any questions，such as how to install the development board, how to download the code, how to install the library. Please refer to :[Makerfabs_FAQ](https://github.com/Makerfabs/Makerfabs_FAQ)**

- Install board : ESP32 .

- Install library : Adafruit GFX library (V1.10.11) , Adafruit SSD1306 library (V2.4.6) and RadioLib library 4.0.6.

- Upload codes, select "ESP32 Dev Module"

  

## Example



