/*
 * CPcf8591.cpp
 *
 *  Created on: 31.05.2020
 *      Author: esy
 */

#include "CPcf8591.h"
/**
 * Erstellt ein Pcf8591 Device mit der übergeben slaveAdresse als I2CDevice
 * @see CI2cDevice
 *
 * @param <slaveAddress> Adresse des Pcf8591 Devices
 */
CPcf8591::CPcf8591(uint8_t slaveAddress) :
		CI2cDevice("1", slaveAddress) {	//Konstruktor der Basisklasse wird aufgerufen
	//zusätliche implementierung für den Kindkonstruktor

}
/**
 * Holt den Wert gelesen Wert des Pcf8591 und gibt diesen zu weiteren Verarbeitung als float zurück
 *
 *
 * @return Gibt das gelesene Byte zurück
 */
float CPcf8591::acquireValue(uint8_t CommandCode) {
	//uint8_t CommandCode = 0x47;	//ADC-Channel auswählen mit Control-Byte S.6 Datenblatt
	return readByte(CommandCode);
}

