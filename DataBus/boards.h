/*
 * boards.h
 *
 *  Created on: Mar 25, 2014
 *      Author: mes
 */

#ifndef BOARDS_H_
#define BOARDS_H_

#include "mbed.h"

/** RoverBaseboard R0.5 */
#define I2CSDA 		p9
#define I2CSCL 		p10
#define UART0TX 	USBTX /** mbed */
#define UART0RX 	USBRX /** mbed */
#define UART1TX		p13
#define UART1RX		p14
#define UART2TX 	p28
#define UART2RX 	p27
#define UART3TX 	p17
#define UART3RX 	p18
#define THROTTLE	p22
#define STEERING	p21
#define ENCALEFT	p30
#define ALEFT		ENCALEFT
#define ENCARIGHT	p29
#define ARIGHT		ENCARIGHT
#define ENCBLEFT	/** unavailable for mbed */
#define BLEFT		ENCBLEFT
#define ENCBRIGHT	/** unavailable for mbed */
#define BRIGHT		ENCBRIGHT
// REV 0.5
#define _D0			p11
#define _D1			p12
#define _D2			/** unavailable for mbed */
#define _D7			/** unavailable for mbed */
#define _D8			/** unavailable for mbed */
#define _D9			/** unavailable for mbed */
#define _P1 		p26
#define _P2 		p25
#define _A0 		p15
#define _A1 		p16
#define _A2 		p19
#define _A3 		p20
// REV 0.6
#define IO0			p11
#define IO1			p12
#define PW0			p25
#define PW1			p26
#define AN0 		p15
#define AN1 		p16
#define AN2 		p19
#define AN3 		p20

#endif /* BOARDS_H_ */
