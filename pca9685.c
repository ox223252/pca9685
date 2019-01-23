////////////////////////////////////////////////////////////////////////////////
/// \copiright ox223252, 2018
///
/// This program is free software: you can redistribute it and/or modify it
///     under the terms of the GNU General Public License published by the Free
///     Software Foundation, either version 2 of the License, or (at your
///     option) any later version.
///
/// This program is distributed in the hope that it will be useful, but WITHOUT
///     ANY WARRANTY; without even the implied of MERCHANTABILITY or FITNESS FOR
///     A PARTICULAR PURPOSE. See the GNU General Public License for more
///     details.
///
/// You should have received a copy of the GNU General Public License along with
///     this program. If not, see <http://www.gnu.org/licenses/>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// \file pca9685.c
/// \brief library used to manage pca9685 micro
/// \author ox223252
/// \date 2018-03
/// \copyright GPLv2
/// \version 0.1
/// \warning NONE
/// \bug NONE
////////////////////////////////////////////////////////////////////////////////

// ressources:
// https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf

#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include "pca9685.h"

const float clock = 25000000.0;
const uint16_t periode = 4096;

typedef enum
{
	MODE1 = 0,
	MODE2 = 1,
	SUBADR1 = 2,
	SUBADR2 = 3,
	SUBADR3 = 4,
	ALLCALLADR = 5,
	LED0_ON_L = 6,
	LED0_ON_H = 7,
	LED0_OFF_L = 8,
	LED0_OFF_H = 9,
	LED1_ON_L = 10,
	LED1_ON_H = 11,
	LED1_OFF_L = 12,
	LED1_OFF_H = 13,
	LED2_ON_L = 14,
	LED2_ON_H = 15,
	LED2_OFF_L = 16,
	LED2_OFF_H = 17,
	LED3_ON_L = 18,
	LED3_ON_H = 19,
	LED3_OFF_L = 20,
	LED3_OFF_H = 21,
	LED4_ON_L = 22,
	LED4_ON_H = 23,
	LED4_OFF_L = 24,
	LED4_OFF_H = 25,
	LED5_ON_L = 26,
	LED5_ON_H = 27,
	LED5_OFF_L = 28,
	LED5_OFF_H = 29,
	LED6_ON_L = 30,
	LED6_ON_H = 31,
	LED6_OFF_L = 32,
	LED6_OFF_H = 33,
	LED7_ON_L = 34,
	LED7_ON_H = 35,
	LED7_OFF_L = 36,
	LED7_OFF_H = 37,
	LED8_ON_L = 38,
	LED8_ON_H = 39,
	LED8_OFF_L = 40,
	LED8_OFF_H = 41,
	LED9_ON_L = 42,
	LED9_ON_H = 43,
	LED9_OFF_L = 44,
	LED9_OFF_H = 45,
	LED10_ON_L = 46,
	LED10_ON_H = 47,
	LED10_OFF_L = 48,
	LED10_OFF_H = 49,
	LED11_ON_L = 50,
	LED11_ON_H = 51,
	LED11_OFF_L = 52,
	LED11_OFF_H = 53,
	LED12_ON_L = 54,
	LED12_ON_H = 55,
	LED12_OFF_L = 56,
	LED12_OFF_H = 57,
	LED13_ON_L = 58,
	LED13_ON_H = 59,
	LED13_OFF_L = 60,
	LED13_OFF_H = 61,
	LED14_ON_L = 62,
	LED14_ON_H = 63,
	LED14_OFF_L = 64,
	LED14_OFF_H = 65,
	LED15_ON_L = 66,
	LED15_ON_H = 67,
	LED15_OFF_L = 68,
	LED15_OFF_H = 69,
	ALL_LED_ON_L = 250,
	ALL_LED_ON_H = 251,
	ALL_LED_OFF_L = 252,
	ALL_LED_OFF_H = 253,
	PRE_SCALE = 254,
	TestMode = 255
}
registers;

typedef enum
{
	ALLCALL = 0x01,
	SUB3 = 0x02,
	SUB2 = 0x04,
	SUB1 = 0x08,
	SLEEP = 0x10,
	AI = 0x20,
	EXTCLK = 0x04,
	RESTART = 0x80
}
mode1_values;

typedef enum
{
	ALLCALL_ON = 0x01,
	ALLCALL_OFF = 0x02,
	SUB3_ON = 0x04,
	SUB3_OFF = 0x08,
	SUB2_ON = 0x10,
	SUB2_OFF = 0x20,
	SUB1_ON = 0x40,
	SUB1_OFF = 0x80,
	SLEEP_ON = 0x0100,
	SLEEP_OFF = 0x0200,
	AI_ON = 0x0400,
	AI_OFF = 0x0800,
	EXTCLK_ON = 0x1000,
	EXTCLK_OFF = 0x2000,
	RESTART_ON = 0x4000,
	RESTART_OFF = 0x8000,
	MODE1_LAST
}
mode1_status;

typedef enum
{
	OUTNE_0 = 0x00,
	OUTNE_DRV = 0x01,
	OUTNE_HZ = 0x02,
	OUTNE = 0x03,
	OUT_HZ = 0x04,
	OCH = 0x08,
	INVRT = 0x10,
	OUT_TP = 0x20
}
mode2;

static int configPCA9685 ( const registers mode, const uint32_t configMask, const int pca9685Fd )
{
	uint8_t buf[ 2 ] = { 0 };
	uint32_t i = 0; // loop counter

	if ( configMask == 0 )
	{
		buf[ 0 ] = mode;
		buf[ 1 ] = 0;
		write ( pca9685Fd, buf, 2 ); // request  MODE1 register Addr
	}

	switch ( mode )
	{
		case MODE1:
		{
			buf[ 0 ] = MODE1;
			write ( pca9685Fd, buf, 1 ); // request  MODE1 register Addr
			read ( pca9685Fd, &buf[ 1 ], 1 ); // read data

			for ( i = 1; i < MODE1_LAST; i *= 2 )
			{
				switch ( configMask & i )
				{
					case ALLCALL_ON:
					{
						buf[ 1 ] |= ALLCALL;
						break;
					}
					case ALLCALL_OFF:
					{
						buf[ 1 ] &= ~ALLCALL;
						break;
					}
					case SUB3_ON:
					{
						buf[ 1 ] |= SUB3;
						break;
					}
					case SUB3_OFF:
					{
						buf[ 1 ] &= ~SUB3;
						break;
					}
					case SUB2_ON:
					{
						buf[ 1 ] |= SUB2;
						break;
					}
					case SUB2_OFF:
					{
						buf[ 1 ] &= ~SUB2;
						break;
					}
					case SUB1_ON:
					{
						buf[ 1 ] |= SUB1;
						break;
					}
					case SUB1_OFF:
					{
						buf[ 1 ] &= ~SUB1;
						break;
					}
					case SLEEP_ON:
					{
						buf[ 1 ] |= SLEEP;
						break;
					}
					case SLEEP_OFF:
					{
						buf[ 1 ] &= ~SLEEP;
						break;
					}
					case AI_ON:
					{
						buf[ 1 ] |= AI;
						break;
					}
					case AI_OFF:
					{
						buf[ 1 ] &= ~AI;
						break;
					}
					case EXTCLK_ON:
					{
						buf[ 1 ] |= EXTCLK;
						break;
					}
					case EXTCLK_OFF:
					{
						buf[ 1 ] &= ~EXTCLK;
						break;
					}
					case RESTART_ON:
					case RESTART_OFF:
					{
						buf[ 1 ] |= RESTART;
						break;
					}
				}
			}
			return ( write ( pca9685Fd, buf, 2 ) != 2 );
		}
		case MODE2:
		{
			buf[ 0 ] = MODE2;
			write ( pca9685Fd, buf, 1 ); // request  MODE1 register Addr
			read ( pca9685Fd, &buf[ 1 ], 1 ); // read data

			if ( ( configMask & OUTNE ) == OUTNE_0 )
			{ // totem pole
				buf[ 1 ] &= ~OUTNE;
			}

			if ( ( configMask & OUTNE ) == OUTNE_DRV )
			{ // Drv
				buf[ 1 ] &= ~OUTNE;
				buf[ 1 ] |= OUTNE_DRV;
			}

			if ( ( configMask & OUTNE ) == OUTNE_HZ )
			{ // High Impedance
				buf[ 1 ] &= ~OUTNE;
				buf[ 1 ] |= OUTNE_HZ;
			}

			if ( configMask & OUT_HZ )
			{ // request for a high imp
				buf[ 1 ] &= ~OUT_HZ;
			}

			if ( configMask & OUT_TP )
			{ // request for a totem pole
				buf[ 1 ] |= OUT_HZ;
			}

			if ( configMask & OCH )
			{ // heu...
				// OCH
			}

			if ( configMask & INVRT )
			{ // invert output
				buf[ 1 ] |= INVRT;
			}
			return ( write ( pca9685Fd, buf, 2 ) != 2 );
		}
		default:
		{
			break;
		}
	}
	return ( 1 );
}

int openPCA9685 ( const char busName[], const uint8_t address, int * const pca9685Fd )
{
	*pca9685Fd = open ( busName, O_RDWR );
	if ( *pca9685Fd < 0 )
	{
		return ( __LINE__ );
	}

	if ( ioctl ( *pca9685Fd, I2C_SLAVE, address ) < 0 )
	{
		return ( __LINE__ );
	}

	if ( configPCA9685 ( MODE1, 0, *pca9685Fd ) )
	{
		return ( __LINE__ );
	}

	if ( configPCA9685 ( MODE2, 0, *pca9685Fd ) )
	{
		return ( __LINE__ );
	}

	if ( configPCA9685 ( MODE2, OUT_TP, *pca9685Fd ) )
	{
		return ( __LINE__ );
	}

	return ( setPCA9685PWMFreq ( 50, *pca9685Fd ) );
}

int closePCA9685 ( const int pca9685Fd )
{
	return ( close ( pca9685Fd ) );
}

int setPCA9685PWMFreq ( const int freq, const int pca9685Fd )
{
	uint8_t buf[ 2 ] = { 0 };
	uint8_t prescale_val = ( uint32_t )( clock / periode / freq )  - 1;

	if ( configPCA9685 ( MODE1, SLEEP_ON | ALLCALL_OFF | AI_ON, pca9685Fd ) )
	{
		return ( __LINE__ );
	}

	buf[ 0 ] = PRE_SCALE; // prescaler register
	buf[ 1 ] = prescale_val; // multiplyer for PWM frequency
	if ( write ( pca9685Fd, buf, 2 ) != 2 )
	{
		return ( __LINE__ );
	}

	if ( configPCA9685 ( MODE1, SLEEP_OFF, pca9685Fd ) )
	{
		return ( __LINE__ );
	}

	if ( configPCA9685 ( MODE2, OUT_TP | OUTNE_HZ, pca9685Fd ) )
	{
		return ( __LINE__ );
	}

	return ( 0 );
}

int setPCA9685PWM ( const uint8_t id, const uint16_t on_value, const uint16_t off_value, const int pca9685Fd )
{
	uint8_t buf[ 5 ] = { 0 };

	buf[ 0 ] = LED0_ON_L + ( 4 * id ); // led register for LSB ON
	buf[ 1 ] = on_value & 0xFF; // LSB ON value
	buf[ 2 ] = on_value >> 8; // MSB ON value
	buf[ 3 ] = off_value & 0xFF; // LSB OFF value
	buf[ 4 ] = off_value >> 8; // MSB OFF value
	return ( write ( pca9685Fd, buf, 5 ) != 5 );
}
