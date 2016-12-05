Pullonkaulan Drinkkirobotin langaton IoT-RGB-spottivalo

Arduino IDE:n asetukset:
NodeMCU 1.0 (ESP-12E Module)
CPU Frequency: "80 MHz"
Flash Size: "4M (3M SPIFFS)"
Upload speed: "115200"
Port: valitse oikea

Asenna Adafruitin NeoPixel-kirjasto ja ESP8266WiFi

Demo-skripti Pythonilla:

import requests

requests.get('http://192.168.1.105/LED=pkerikoinen')

requests.get('http://192.168.1.105/LED=gintonic')

requests.get('http://192.168.1.105/LED=tuplajallu')

requests.get('http://192.168.1.105/LED=ttyspecial')
