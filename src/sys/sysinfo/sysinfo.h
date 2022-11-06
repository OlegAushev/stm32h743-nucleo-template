/**
 * @file sysinfo.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


class Sysinfo
{
private:
	static inline float m_mcuTemp{0.f};

public:
	void static saveMcuTemperature(float mcuTemp)
	{
		m_mcuTemp = mcuTemp;
	}

	static float mcuTemperature()
	{
		return m_mcuTemp;
	}
};


