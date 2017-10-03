#include "Ping.h"
#include "Arduino.h"

Ping::Ping(int trigger, int echo, int max_dist) : m_trigger(trigger), m_echo(echo), m_max_dist(max_dist)
{
	pinMode(m_trigger, OUTPUT);
	pinMode(m_echo, INPUT);
}

unsigned long Ping::ping()
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

unsigned long Ping::ping_mm(int temp, int it)
{
	float c = (331.5 + (temp * 0.6)) / 1000.0; //mm/us
	
	unsigned long uS[it], last;
	uint8_t j, i = 0;
	unsigned long t;
	uS[0] = 0;

	while (i < it) {
		last = ping();  

		if (last != 0) {         // Ping in range, include as part of median.
			if (i > 0) {               // Don't start sort till second ping.
				for (j = i; j > 0 && uS[j - 1] < last; j--) // Insertion sort loop.
					uS[j] = uS[j - 1];                      // Shift ping array to correct position for sort insertion.
			} else j = 0;              // First ping is sort starting point.
			uS[j] = last;              // Add last ping to array in sorted position.
			i++;                       // Move to next ping.
		} else it--;                   // Ping out of range, skip and don't include as part of median.
		delay(10);
	}
	return (uS[it >> 1] / 2.0 * c); // Return the ping distance median.
}