/**
 * @file callbacks_adc.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "mcu_stm32h7/dma/mcu_dma.h"
#include "mcu_stm32h7/adc/mcu_adc.h"
#include "sys/sysinfo/sysinfo.h"


extern mcu::dma::Buffer<uint16_t, 2> adc3DmaBuffer;
void onAdc3DmaHalfCompleted();
void onAdc3DmaCompleted();
void onAdc3DmaError();

