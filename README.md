# WLAN Stick

## Introduction

The WLAN stick is a WebServer, where you can download files
from a SD-card.<br/>
The WLAN stick is running on a M5Stack device (see ESP32 Basic Core IoT Development Kit)
with an integrated battery, ESP32 module, LCD display, 3 buttons and SDcard slot.<br/>
For more information see http://m5stack.com.

## Software

I use the arduino framework to program the WLAN Stick, because all
required libraries are available to control the buttons, LCD and 
SDcard. 

## How to use

1. Install the Arduino IDE with the M5Stack environment
2. Download the WLANStick.ino file 
3. Change the WLAN SSID (const char* ssid) and the WLAN password (const char* password) to your favourite values
4. Compile and download the software to your M5Stack device
5. Prepare a FAT formatted SD card for the WLAN Stick
6. Copy the files, you want to share, to the SD card. All files must be in the root folder!
7. Plug the SD-card into the M5Stack slot
8. Switch on the M5Stack
9. On the LCD of the M5Stack you will see the WLAN settings and the IP address of the WLAN Stick
10. Now, a WLAN network with the name given in the WLANStick.ino exists
11. Connect with your computer, tablet or mobile phone to this WLAN network. Use the password
12. Open a browser and put the IP address of the WLAN Stick (see 9.) as http address
13. You will see the files stored on SD card of the M5Stack.
14. ENJOY!
15. Press the button 'A' on the M5Stack to switch-off the WLAN Stick.
