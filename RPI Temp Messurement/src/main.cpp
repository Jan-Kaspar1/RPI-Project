#include <iostream>
#include <sys/time.h>
#include <signal.h>
#include "CI2cDevice.h"
#include "CThermostat.h"
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <pcf8574.h>
#include <lcd.h>
#include <time.h>

// Define some device parameters
#define I2C_ADDRADC	  0x4b //I2C Adresse des ADC's
#define ADC_Channel 0x47//Channel des ADC's siehe Control-Byte S.6 Datenblatt des CPcf8591

#define pcf8574_address 0x27 // default I2C address of Pcf8574A
#define BASE 64 // BASE any number above 64
//Define the output pins of the PCF8574, which are directly connected to the LCD1602 pin.
#define RS BASE+0
#define RW BASE+1
#define EN BASE+2
#define LED BASE+3
#define D4 BASE+4
#define D5 BASE+5
#define D6 BASE+6
#define D7 BASE+7

using namespace std;

extern float tempC;		//aktuelle Temp aus CThermostat

int lcdhd; // used to handle LCD

int main(void) {
	printf("Program is starting ... \n");

	// Objekt erzeugen, das den GPIO-Chip representiert
	gpiod::chip ioChip("/dev/gpiochip0");
	// (Kontrollausgaben)
	cout << ioChip.name() << endl;
	cout << ioChip.label() << endl;

	// Objekt holen, das den Anschluss 18 (GPI18) des Chips repräsentiert
	gpiod::line LEDB = ioChip.get_line(18);
	// Anschluss mit allen "Request-Objekten" als Ausgänge konfigurieren
	gpiod::line_request request;
	request.request_type = gpiod::line_request::DIRECTION_OUTPUT;
	LEDB.request(request);
	cout << "Gpio line wurde erzeugt" << endl;

	CThermostat Thermostat("1", I2C_ADDRADC, ADC_Channel, &LEDB);
	int i;
	printf("Program is starting ...\n");
	wiringPiSetup();

	pcf8574Setup(BASE, pcf8574_address);		//initialize PCF8574
	for (i = 0; i < 8; i++) {
		pinMode(BASE + i, OUTPUT); //set PCF8574 port to output mode
	}
	digitalWrite(LED, HIGH); //turn on LCD backlight
	digitalWrite(RW, LOW); //allow writing to LCD
	lcdhd = lcdInit(2, 16, 4, RS, EN, D4, D5, D6, D7, 0, 0, 0, 0); // initialize LCD and return “handle” used to handle LCD
	if (lcdhd == -1) {
		printf("lcdInit failed !");
		return 1;
	}

	while (1) {
		sleep(10); 	//wartet bis Die Temperatur aktualisiert wurde und läuft dann weiter

		lcdPosition(lcdhd, 0, 0); // set the LCD cursor position to (0,0)
		lcdPrintf(lcdhd, "Temperatur:%.2fC", tempC); // Display CPU temperature on LCD

		lcdPosition(lcdhd, 0, 1); // set the LCD cursor position to (0,1)
		if (Thermostat.isOn())
			lcdPrintf(lcdhd, "Led is on  ");
		else
			lcdPrintf(lcdhd, "Led is off ");


	}
	return 0;
}

