#include "Ping.h"
#include "Arduino.h"

Ping::Ping(int trigger, int echo, int max_dist) : m_trigger(trigger), m_echo(echo), m_max_dist(max_dist)
{
	pinMode(m_trigger, OUTPUT);
	pinMode(m_echo, INPUT);
}

unsigned long Ping::ping_us()
{
	digitalWrite(m_trigger, LOW);
	delayMicroseconds(2);
	digitalWrite(m_trigger, HIGH);
	delayMicroseconds(10);
	digitalWrite(m_trigger, LOW);

	noInterrupts();
	unsigned long result = pulseIn(m_echo, HIGH, 8000); //use specific value for that later
	interrupts();
	return result;
}

unsigned int Ping::ping_mm(int temp, int it)
{
	unsigned long uS[it], last;
	int j, i = 0;
	unsigned long t;
	float c = (331.5 + (temp * 0.6)) / 500.0; //mm/us
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
					uS[j] = uS[j - 1];                      
			} 
			else 
				j = 0;             
			uS[j] = last;              
			i++;                      
		} 
		else 
			it--;               
		delay(10);
	}
	return uS[it >> 1] / c; 
}