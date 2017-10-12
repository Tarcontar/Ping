//mostly a copy from NewPing library
//but get ping in mm and take temperature into account
#pragma once
#include "Arduino.h"

#define PING_OVERHEAD 5 
#define MAX_SENSOR_DISTANCE 500
#define MAX_SENSOR_DELAY 5800
#define US_ROUNDTRIP_CM 57

class Ping
{
public: 
	Ping(int trigger, int echo, int max_dist = MAX_SENSOR_DISTANCE);
	
	unsigned int ping_mm(int temp = 22,  int it = 15, int max_dist = 0);
	unsigned long ping_us(int max_dist = 0);
	
private:
	bool trigger();
	void setMaxDistance(unsigned int max_cm_distance);
	
private:	
	unsigned int m_triggerBit;
	unsigned int m_echoBit;
	volatile uint8_t *m_triggerOutput;
	volatile uint8_t *m_echoInput;
	volatile uint8_t *m_triggerMode;
	
	unsigned int m_maxEchoTime;
	unsigned long m_max_time;
};