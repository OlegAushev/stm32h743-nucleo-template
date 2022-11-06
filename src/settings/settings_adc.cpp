/**
 * @file settings_adc.cpp
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "settings.h"


#pragma GCC diagnostic ignored "-Wmissing-field-initializers"


/* ADC3 */
const mcu::adc::Config Settings::Mcu::adc3Config = {
	.init = {
		.ClockPrescaler =		ADC_CLOCK_ASYNC_DIV16,	
		.Resolution =			ADC_RESOLUTION_16B,		/* 16-bit resolution for converted data */
		.ScanConvMode =			DISABLE,			/* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
		.EOCSelection =			ADC_EOC_SINGLE_CONV,		/* EOC flag picked-up to indicate conversion end */
		.LowPowerAutoWait =		DISABLE,			/* Auto-delayed conversion feature disabled */
		.ContinuousConvMode =		ENABLE,				/* Continuous mode disabled (automatic conversion restart after each conversion) */
		.NbrOfConversion =		1,				/* Parameter discarded because sequencer is disabled */
		.DiscontinuousConvMode =	DISABLE,			/* Parameter discarded because sequencer is disabled */
		.NbrOfDiscConversion =		1,				/* Parameter discarded because sequencer is disabled */
		.ExternalTrigConv =		ADC_SOFTWARE_START,		/* Software start to trig the 1st conversion manually, without external event */
		.ExternalTrigConvEdge =		ADC_EXTERNALTRIGCONVEDGE_NONE,	/* Parameter discarded because software trigger chosen */
		.ConversionDataManagement =	ADC_CONVERSIONDATA_DMA_CIRCULAR,/* DR register used as output (DMA mode disabled) */
		.Overrun =			ADC_OVR_DATA_OVERWRITTEN,	/* DR register is overwritten with the last conversion result in case of overrun */
		.LeftBitShift =			ADC_LEFTBITSHIFT_NONE,		/* Left shift of final results */
		.OversamplingMode =		DISABLE				/* Oversampling disable */
	}
};

const ADC_ChannelConfTypeDef Settings::AdcChannels::internalTempChannelConfig = {
	.Channel =			ADC_CHANNEL_TEMPSENSOR,		/* Sampled channel number */
	.Rank =				ADC_REGULAR_RANK_1,		/* Rank of sampled channel number ADCx_CHANNEL */
	.SamplingTime =			ADC_SAMPLETIME_810CYCLES_5,	/* Sampling time (number of clock cycles unit) */
	.SingleDiff =			ADC_SINGLE_ENDED,		/* Single input channel */
	.OffsetNumber =			ADC_OFFSET_NONE,		/* No offset subtraction */ 
	.Offset =			0,				/* Parameter discarded because offset correction is disabled */
	.OffsetRightShift =		DISABLE,			/* No Right Offset Shift */
	.OffsetSignedSaturation =	DISABLE,			/* No Signed Saturation */
};

const ADC_ChannelConfTypeDef Settings::AdcChannels::internalVrefChannelConfig = {
	.Channel =			ADC_CHANNEL_VREFINT,		/* Sampled channel number */
	.Rank =				ADC_REGULAR_RANK_2,		/* Rank of sampled channel number ADCx_CHANNEL */
	.SamplingTime =			ADC_SAMPLETIME_810CYCLES_5,	/* Sampling time (number of clock cycles unit) */
	.SingleDiff =			ADC_SINGLE_ENDED,		/* Single input channel */
	.OffsetNumber =			ADC_OFFSET_NONE,		/* No offset subtraction */ 
	.Offset =			0,				/* Parameter discarded because offset correction is disabled */
	.OffsetRightShift =		DISABLE,			/* No Right Offset Shift */
	.OffsetSignedSaturation =	DISABLE,			/* No Signed Saturation */
};
