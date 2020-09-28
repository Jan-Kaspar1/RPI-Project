/*
 * CThermostat.h
 *
 *  Created on: 31.05.2020
 *      Author: esy
 */

#ifndef SRC_CTHERMOSTAT_H_
#define SRC_CTHERMOSTAT_H_
#include<gpiod.hpp>
#include "CPcf8591.h"
#include <string>
#include <signal.h>

class CThermostat {
private:
	CPcf8591 m_dac; /**< CPcf8591 Adc mit eingebundener Kommunikation*/
	uint8_t m_channel; /**< uint8_t Auswahl des gewünschten ADC-Channels*/
	float m_thresholdLow; /**< float Unterer Schwellwert*/
	float m_thresholdHigh; /**< float oberer Schwellwert*/
	bool m_outputState; /**< Ausgang ein oder ausgeschaltet?*/
	gpiod::line *m_tOut; /**< gpiod::line * Auf welchem Pin soll das Ausgangsignal geschaltet werden*/

public:
	CThermostat(const std::string &i2cBus, uint8_t slaveAddress, uint8_t channel,
			gpiod::line *tOut);
	virtual ~CThermostat();
	float temperature();
	void setTL(float threshold);
	void setTH(float threshold);
	bool isOn();
	static void timerSignal(int signum);
};

#endif /* SRC_CTHERMOSTAT_H_ */
