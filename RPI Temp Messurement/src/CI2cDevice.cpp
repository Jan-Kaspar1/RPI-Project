/*
 * CI2cDevice.cpp
 *
 *  Created on: 31.05.2020
 *      Author: esy
 */

#include "CI2cDevice.h"

/**
 * Konstruktor Auswahl der Schnittstellen über Parameter
 *
 * muss open aufrufen
 *
 *
 * @param <i2CBus>		AdapterNr des I2c-Bus
 * @param <slaveAddress>		Adresse des Slave Teilnehmers

 * @return kein return
 */

CI2cDevice::CI2cDevice(const string &i2CBus, uint8_t slaveAddress) {

	int adapter_nr = 0;
	if (i2CBus == "1")
		adapter_nr = 1; /* probably dynamically determined */ //TODO: Adapter Nr. aus string generieren
	else
		adapter_nr = 1;
	char filename[20];
	m_file = -1;
	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);	//Generierung Filename
	m_file = open(filename, O_RDWR);
	if (m_file < 0) {	//eigentlich witzlos, m_file ist unsigned
		printf("Failed to open the i2c bus.\n");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}

	m_slave_adress = slaveAddress; /* The I2C address mit i2c detect */
	//0x4b für den adc nach meiner Mat-Konfiguration

	if (ioctl(m_file, I2C_SLAVE, m_slave_adress) < 0) {	//MAN KANN DEN SLAVE auswählen, bei Kommunication mit ander schnittstelle muss ich die alte schließen und eine neue öffnen
		printf("Failed to acquire bus access and/or talk to slave.\n");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}

}

/**
 * Destuktor muss close aufrufen
 * Schließt die Verbindung, sodass der Adaper eine neue Verbindung aufbauen kann
 *
 *
 * @return kein return
 */
CI2cDevice::~CI2cDevice() {
	close(m_file);
}

/**
 * Sendet das in data übergeben data-Byte an ein I2c Device
 *
 *
 * @param <data>8Bit die zu senden sind
 * @return kein return
 */
void CI2cDevice::sendByte(uint8_t data) {
	uint8_t buf[1];

	buf[0] = data;

	if (write(m_file, buf, 1) != 1) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
		//	    buffer = g_strerror(errno);
		//	    printf(buffer);
		printf("\n\n");
	}

}
/**
 * Empfängt ein Byte über die I2C-Schnittstelle
 *
 *
 * @return Gibt die empfangenen 8Bit zurück
 */
uint8_t CI2cDevice::receiveByte() {

	uint8_t bufdata[1];		//daten werden hier abgelegt
	// Using I2C Read
	if (read(m_file, bufdata, 1) != 1) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to read from the i2c bus.\n");
//	        buffer = g_strerror(errno);
//	        printf(buffer);
		printf("\n\n");
	} else {
		printf("Data %02d ", (int) bufdata[0]);
	}

	return bufdata[0];

}
/**
 * schreibt ein Byte auf den I2C Bus nach SMBus Protokoll
 *
 *
 * @param <commandCode>	Geräte spezifischer Commandocode
 * für adc 0x47 siehe Control-Byte S.6 Datenblatt des CPcf8591
 * @param <data> die zu schreibenden Daten

 * @return kein return
 */
void CI2cDevice::writeByte(uint8_t commandCode, uint8_t data) {

	uint8_t buf[2];

	//buf[0] = m_slave_adress;	//wird schon automatisch mit dem wirte Befehl geschickt
	buf[0] = commandCode;
	buf[1] = data;

	if (write(m_file, buf, 2) != 2) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
//	    buffer = g_strerror(errno);
//	    printf(buffer);
		printf("\n\n");
	}

}
/**
 * Liest ein Byte vom I2C Bus nach Smbus Protokoll
 *
 *
 * @param <commandCode>Geräte spezifischer Commandocode
 * für adc 0x47 siehe Control-Byte S.6 Datenblatt des CPcf8591
 * @return gibt das gelesene Byte zurück
 */
uint8_t CI2cDevice::readByte(uint8_t commandCode) {

	uint8_t buf[1];	//buffer um commandcode an den baustein zu übergeben und richtiges leseregister auszuwählen
	uint8_t bufdata[1];		//daten werden hier abgelegt

	buf[0] = commandCode;

	if (write(m_file, buf, 1) != 1) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
//	    buffer = g_strerror(errno);
//	    printf(buffer);
		printf("\n\n");
	}

	// Using I2C Read
	if (read(m_file, bufdata, 1) != 1) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to read from the i2c bus.\n");
//	        buffer = g_strerror(errno);
//	        printf(buffer);
		printf("\n\n");
	} else {
		printf("Data %02d ", (int) bufdata[0]);
	}

	return bufdata[0];
}

/**
 * schreibt ein Wort auf den I2C Bus nach SMBUS Protokoll
 *
 *
 * @param <commandCode>Geräte spezifischer Commandocode
 * für adc 0x47 siehe Control-Byte S.6 Datenblatt des CPcf8591
 * @param <data> das zu Schreibende Wort

 * @return kein return
 */
void CI2cDevice::writeWord(uint8_t commandCode, uint16_t data) {
	uint8_t buf[3];

	buf[0] = commandCode;
	buf[1] = (uint8_t) (data & 0xff);
	buf[2] = (uint8_t) (data >> 8);

	if (write(m_file, buf, 3) != 3) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
//	    buffer = g_strerror(errno);
//	    printf(buffer);
		printf("\n\n");
	}
}
/**
 * Liest ein Wort vom I2C Bus nach SMBus Protokoll
 *
 *
 * @param <commandCode>Geräte spezifischer Commandocode
 * für adc 0x47 siehe Control-Byte S.6 Datenblatt des CPcf8591
 * @return Gibt das gelesene Wort zurück HighByte->LowByte
 */
uint16_t CI2cDevice::readWord(uint8_t commandCode) {

	uint8_t buf[1];	//buffer um commandcode an den baustein zu übergeben und richtiges leseregister auszuwählen
	uint8_t bufdata[2];		//daten werden hier abgelegt
	uint16_t retData = 0;
	buf[0] = commandCode;

	if (write(m_file, buf, 1) != 1) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
//	    buffer = g_strerror(errno);
//	    printf(buffer);
		printf("\n\n");
	}

	// Using I2C Read
	if (read(m_file, bufdata, 2) != 2) {
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to read from the i2c bus.\n");
//	        buffer = g_strerror(errno);
//	        printf(buffer);
		printf("\n\n");
	} else {
		retData = bufdata[1];
		retData = retData << 8;
		retData = retData | bufdata[0];
		printf("Data %02d ", (int) retData);
	}

	return retData;
}

