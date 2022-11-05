/**
 * @defgroup mcu_crc CRC
 * @ingroup mcu
 * 
 * @file mcu_crc.h
 * @ingroup mcu mcu_crc
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "../mcu_def.h"
#include "../system/mcu_system.h"


namespace mcu {


namespace crc {


/// @brief 
enum InputDataFormat
{
	BYTES = CRC_INPUTDATA_FORMAT_BYTES,
	HALFWORDS = CRC_INPUTDATA_FORMAT_HALFWORDS,
	WORDS = CRC_INPUTDATA_FORMAT_WORDS
};


/**
 * @brief 
 * 
 */
struct Config
{
	CRC_InitTypeDef init;
	InputDataFormat inputDataFormat;
};


/**
 * @brief 
 * 
 */
class Module
{
private:
	Module() = default;
	CRC_HandleTypeDef m_handle;
public:
	Module(const Module& other) = delete;
	Module& operator=(const Module& other) = delete;

	/**
	 * @brief 
	 * 
	 * @return Module& 
	 */
	static Module& instance()
	{
		static Module instance_;
		return instance_;
	}

	/**
	 * @brief 
	 * 
	 * @param cfg 
	 */
	void init(const Config& cfg)
	{
		m_handle.Instance = CRC;
		m_handle.Init = cfg.init;
		m_handle.InputDataFormat = static_cast<uint32_t>(cfg.inputDataFormat);
		if (HAL_CRC_Init(&m_handle) != HAL_OK)
		{
			fatal_error("CRC module initialization failed");
		}
	}

	/**
	 * @brief 
	 * 
	 * @return HalStatus 
	 */
	HalStatus deinit()
	{
		return HAL_CRC_DeInit(&m_handle);
	}

	/**
	 * @brief 
	 * 
	 * @param buf 
	 * @param len 
	 * @return uint32_t 
	 */
	uint32_t calculate(uint32_t* buf, uint32_t len)
	{
		return HAL_CRC_Accumulate(&m_handle, buf, len);
	}

	/**
	 * @brief 
	 * 
	 * @param buf 
	 * @param len 
	 * @return uint32_t 
	 */
	uint32_t accumulate(uint32_t* buf, uint32_t len)
	{
		return HAL_CRC_Calculate(&m_handle, buf, len);
	}
};


}


}


