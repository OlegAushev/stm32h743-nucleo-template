/**
 * @file emb_uart.h
 * @author Oleg Aushev (aushevom@protonmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once


#include "../emb_def.h"


namespace emb {


/**
 * @brief UART interface class.
 */
class IUart
{
public:
	IUart() {}
	virtual ~IUart() {}
	
	IUart(const IUart& other) = delete;
	IUart& operator=(const IUart& other) = delete;

	//virtual void reset() = 0;
	//virtual bool hasRxError() const = 0;

	virtual int recv(char& ch) = 0;
	virtual int recv(char* buf, size_t len) = 0;

	virtual int send(char ch) = 0;
	virtual int send(const char* buf, size_t len) = 0;

	//virtual void registerRxInterruptHandler(void (*handler)(void)) = 0;
	//virtual void enableRxInterrupts() = 0;
	//virtual void disableRxInterrupts() = 0;
	//virtual void acknowledgeRxInterrupt() = 0;
};


} // namespace emb


