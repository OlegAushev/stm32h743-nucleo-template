/**
 * @file sysconfig_adc.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "sysconfig.h"


#pragma GCC diagnostic ignored "-Wmissing-field-initializers"


/* ADC3 */
const mcu::adc::Config sysconfig::adc3::config = {
	.init = {
		.ClockPrescaler =		ADC_CLOCK_ASYNC_DIV16,	
		.Resolution =			ADC_RESOLUTION_16B,
		.ScanConvMode =			ADC_SCAN_ENABLE,
		.EOCSelection =			ADC_EOC_SEQ_CONV,
		.LowPowerAutoWait =		DISABLE,
		.ContinuousConvMode =		ENABLE,				
		.NbrOfConversion =		2,			
		.DiscontinuousConvMode =	DISABLE,
		.NbrOfDiscConversion =		1,				// discarded
		.ExternalTrigConv =		ADC_SOFTWARE_START,
		.ExternalTrigConvEdge =		ADC_EXTERNALTRIGCONVEDGE_NONE,	// discarded
		.ConversionDataManagement =	ADC_CONVERSIONDATA_DMA_CIRCULAR,
		.Overrun =			ADC_OVR_DATA_OVERWRITTEN,
		.LeftBitShift =			ADC_LEFTBITSHIFT_NONE,
		.OversamplingMode =		DISABLE
	}
};

const mcu::dma::Config sysconfig::adc3::dma::config = {
	.init = {
		.Request =			DMA_REQUEST_ADC3,
		.Direction =			DMA_PERIPH_TO_MEMORY,
		.PeriphInc =			DMA_PINC_DISABLE,
		.MemInc=			DMA_MINC_ENABLE,
		.PeriphDataAlignment =		DMA_PDATAALIGN_HALFWORD,
		.MemDataAlignment =		DMA_MDATAALIGN_HALFWORD,
		.Mode =				DMA_CIRCULAR,
		.Priority =			DMA_PRIORITY_HIGH,
	}
};

const ADC_ChannelConfTypeDef sysconfig::adc3::channels::internalTemp = {
	.Channel =			ADC_CHANNEL_TEMPSENSOR,		/* Sampled channel number */
	.Rank =				ADC_REGULAR_RANK_1,		/* Rank of sampled channel number ADCx_CHANNEL */
	.SamplingTime =			ADC_SAMPLETIME_810CYCLES_5,	/* Sampling time (number of clock cycles unit) */
	.SingleDiff =			ADC_SINGLE_ENDED,		/* Single input channel */
	.OffsetNumber =			ADC_OFFSET_NONE,		/* No offset subtraction */ 
	.Offset =			0,				/* Parameter discarded because offset correction is disabled */
	.OffsetRightShift =		DISABLE,			/* No Right Offset Shift */
	.OffsetSignedSaturation =	DISABLE,			/* No Signed Saturation */
};

const ADC_ChannelConfTypeDef sysconfig::adc3::channels::internalVref = {
	.Channel =			ADC_CHANNEL_VREFINT,		/* Sampled channel number */
	.Rank =				ADC_REGULAR_RANK_2,		/* Rank of sampled channel number ADCx_CHANNEL */
	.SamplingTime =			ADC_SAMPLETIME_810CYCLES_5,	/* Sampling time (number of clock cycles unit) */
	.SingleDiff =			ADC_SINGLE_ENDED,		/* Single input channel */
	.OffsetNumber =			ADC_OFFSET_NONE,		/* No offset subtraction */ 
	.Offset =			0,				/* Parameter discarded because offset correction is disabled */
	.OffsetRightShift =		DISABLE,			/* No Right Offset Shift */
	.OffsetSignedSaturation =	DISABLE,			/* No Signed Saturation */
};


