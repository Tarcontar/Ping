#include "Ping.hpp"

Ping::Ping()
{
	
}

Ping::Ping(int trigger, int echo, int max_dist)
{
	m_triggerBit = digitalPinToBitMask(trigger); // Get the port register bitmask for the trigger pin.
	m_echoBit = digitalPinToBitMask(echo);       // Get the port register bitmask for the echo pin.

	m_triggerOutput = portOutputRegister(digitalPinToPort(trigger)); // Get the output port register for the trigger pin.
	m_echoInput = portInputRegister(digitalPinToPort(echo));         // Get the input port register for the echo pin.

	m_triggerMode = (uint8_t *) portModeRegister(digitalPinToPort(trigger)); // Get the port mode register for the trigger pin.
	*m_triggerMode |= m_triggerBit; //set to output
	setMaxDistance(max_dist);
}

unsigned int Ping::ping_mm(int temp, int it, int max_dist)
{
	unsigned long uS[it], last;
	int j, i = 0;
	unsigned long t;
	float c = (331.5 + (temp * 0.6)) / 1000.0; //mm/us
	if (max_dist > 0) setMaxDistance(max_dist);
	uS[0] = 0;

	while (i < it) 
	{
		last = ping_us();  

		if (last != 0) 
		{         
			if (i > 0) 
			{             
				//simple insertion sort
				for (j = i; j > 0 && uS[j - 1] < last; j--) 
				{
					uS[j] = uS[j - 1];                      
				}
			} 
			else 
			{
				j = 0;   
			}
			uS[j] = last;              
			i++;                      
		} 
		else 
		{
			it--;        
		}
		delay(10);
	}
	return (uS[it >> 1]  / 2.0) * c; 
}

unsigned long Ping::ping_us(int max_dist)
{	
	if (max_dist > 0) setMaxDistance(max_dist);
	if (!trigger()) return m_maxEchoTime;
	
	while (*m_echoInput & m_echoBit)   //wait for ping echo
		if (micros() > m_max_time) return m_maxEchoTime;
	
	return (micros() - (m_max_time - m_maxEchoTime) - PING_OVERHEAD);
}

bool Ping::trigger()
{
	*m_triggerOutput &= ~m_triggerBit;   //low
	delayMicroseconds(4);              
	*m_triggerOutput |= m_triggerBit;    //high
	delayMicroseconds(10);            
	*m_triggerOutput &= ~m_triggerBit;   //low
	
	if (*m_echoInput & m_echoBit) return false; // previous ping has not finished yet
	m_max_time = micros() + m_maxEchoTime + MAX_SENSOR_DELAY; 
	
	while(!(*m_echoInput & m_echoBit));	//wait for ping to start
		if (micros() > m_max_time) return false;
	m_max_time = micros() + m_maxEchoTime;
	return true;
}

void Ping::setMaxDistance(unsigned int max_cm_distance)
{
	m_maxEchoTime = min(max_cm_distance, (unsigned int) MAX_SENSOR_DISTANCE) * US_ROUNDTRIP_CM + (US_ROUNDTRIP_CM / 2);
}