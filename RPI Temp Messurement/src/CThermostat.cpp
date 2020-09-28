/*
 * CThermostat.cpp
 *
 *  Created on: 31.05.2020
 *      Author: esy
 */

#include "CThermostat.h"
#include <cmath>
#include <sys/time.h>
#include <iostream>
#include <list>

using namespace std;

float tempC = 0; /**< aktuelle Temp in C° gemessen mit der ISR*/

static list<CThermostat*> Thermostat; /**< Liste mit den erstellten Thermostaten*/
/**
 * Konsturktor des Thermostats initalisiert die Attribute
 *
 *
 * @param <i2cBus> I2c Schnittstelle des RPI's
 * @param <slaveAddress> Adresse des Thermostats
 * @param <channel> welcher Channel des Thermostats muss ausgelesen werden
 * @param <tOut> Geschalteter Ausgang ThermostatFkt
 *
 */
CThermostat::CThermostat(const std::string &i2cBus, uint8_t slaveAddress,
		uint8_t channel, gpiod::line *tOut) :
		m_dac(slaveAddress) {

	struct itimerval NewValue;
	NewValue.it_interval.tv_sec = 1;	//eine Sekunde beim Timer einstellen
	NewValue.it_value.tv_sec = 1;
	NewValue.it_interval.tv_usec = 0;	//eine Sekunde beim Timer einstellen
	NewValue.it_value.tv_usec = 0;

	if (setitimer(ITIMER_REAL, &NewValue, NULL)) {
		perror("settimer");
	}
	signal(SIGALRM, timerSignal);

	m_channel = channel;
	m_thresholdLow = 27;
	m_thresholdHigh = 28;
	m_outputState = 0;
	m_tOut = tOut;
	Thermostat.emplace_back(this);

}
/**
 * Destuktor Thermostat
 * löscht das erste Element der Static Liste wieder
 */
CThermostat::~CThermostat() {
	Thermostat.erase(Thermostat.begin());
}
/**
 * Gibt die akutlle Temperatur in °C zuruck
 * @return float aktuelle Temp in °C
 */
float CThermostat::temperature() {

	return tempC;
}
/**
 * Man setzt untere Schaltschwelle des Thermostats
 *
 * @param <threshold> untere Schaltschwelle
 */
void CThermostat::setTL(float threshold) {
	m_thresholdHigh = threshold;
}
/**
 * Man setzt obere Schaltschwelle des Thermostats
 *
 * @param <threshold> obere Schaltschwelle
 */
void CThermostat::setTH(float threshold) {
	m_thresholdLow = threshold;
}

/**
 * sagt dem Programm ob der Ausgang des Thermostats geschaltet hat
 *
 * @return bool wenn True dann ist der Ausgang eingeschaltet
 * bei False ist der Ausgang ausgeschaltet
 */
bool CThermostat::isOn() {
	if (m_tOut->get_value()) {
		return true;
	} else {
		return false;
	}
}
/**
 * Interrupt Routine
 * Rechnet den ADC-Wert in die Aktuelle Temperatur um.
 * Speichert diesen in die globale Variable
 * und setzt die LED je nach Schaltschwelle
 *
 * @param <signum> Welcher interrupt ausgelöst hat
 */
void CThermostat::timerSignal(int signum) { //signum=SIGALRM

	//Temperatur auslesen
	cout << "Interrupt signal (" << signum << ") received.\n";
	uint8_t adcValue = Thermostat.front()->m_dac.acquireValue(
			Thermostat.front()->m_channel);
	float voltage = (float) adcValue / 255.0 * 3.3;  // calculate voltage
	float Rt = 10 * voltage / (3.3 - voltage); //calculate resistance value of thermistor
	float tempK = 1 / (1 / (273.15 + 25) + log(Rt / 10) / 3950.0); //calculate temperature (Kelvin)
	tempC = tempK - 273.15;        //calculate temperature (Celsius)
	printf("ADC value : %d  ,\tVoltage : %.2fV, \tTemperature : %.2fC\n",
			adcValue, voltage, tempC);

	if (Thermostat.front()->m_thresholdHigh < tempC)
		Thermostat.front()->m_tOut->set_value(1);
	else if (Thermostat.front()->m_thresholdLow > tempC)
		Thermostat.front()->m_tOut->set_value(0);

}

