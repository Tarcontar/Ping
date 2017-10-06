#pragma once

class Ping
{
public: 
	Ping(int trigger = -1, int echo = -1, int max_dist = 4000);
	
	unsigned long ping_us();
	unsigned int ping_mm(int temp = 22, int it = 15);
	
private:
	int m_trigger;
	int m_echo;
	int m_max_dist;
};