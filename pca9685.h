#ifndef __PCA9685_H__
#define __PCA9685_H__
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

#include <stdint.h>

#ifdef PCA_WITH_THREAD
#include <pthread.h>
#endif

////////////////////////////////////////////////////////////////////////////////
/// \file pca9685.h
/// \brief library used to manage pca9685 micro
/// \author ox223252
/// \date 2018-03
/// \copyright GPLv2
/// \version 0.2
/// \warning NONE
/// \bug NONE
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// \fn int openPCA9685 ( const char busName[], const uint8_t address, 
///     int * const pca9685Fd );
/// \param[ in ] busName: i2c bus name "/dev/i2c-0"
/// \param[ in ] address: chip address on i2c bus
/// \param[ out ] pca9685Fd: file descriptor pointer to access to device
/// \breif create access to pca9685 chip
/// \return 0: OK
///         else see errno
////////////////////////////////////////////////////////////////////////////////
int openPCA9685 ( const char fileName[], const uint8_t address, 
	int * const pca9685Fd );

////////////////////////////////////////////////////////////////////////////////
/// \fn int openPCA9685 ( int const pca9685Fd );
/// \param[ in ] pca9685Fd: file descriptor to access to device
/// \breif close access to pca9685 chip
/// \return 0: OK
///         else see errno
////////////////////////////////////////////////////////////////////////////////
int closePCA9685 ( const int pca9685Fd );

////////////////////////////////////////////////////////////////////////////////
/// \fn int setPCA9685PWMFreq ( const int freq, const int pca9685Fd );
/// \param[ in ] freq: new frequency of the chip
/// \param[ in ] pca9685Fd: file descriptor to access to device
/// \breif change the pca9685 chip
/// \return 0: OK
///         else see errno
////////////////////////////////////////////////////////////////////////////////
int setPCA9685PWMFreq ( const int freq, const int pca9685Fd );

////////////////////////////////////////////////////////////////////////////////
/// \fn int setPCA9685PWM ( const uint8_t id, const uint16_t on_value, 
///     const uint16_t off_value, const int pca9685Fd );
/// \param[ in ] id: id of the device connected to the chip [ 0 ; 15 ]
/// \param[ in ] on_value: time of the rising edge of the pulse
/// \param[ in ] off_value: time of the falling edge of the pulse
/// \param[ in ] pca9685Fd: file descriptor to access to device
/// \breif change the PWM scale of device[ id ]
/// \return 0: OK
///         else see errno
////////////////////////////////////////////////////////////////////////////////
int setPCA9685PWM ( const uint8_t id, const uint16_t on_value, 
	const uint16_t off_value, const int pca9685Fd );

#ifdef PCA_WITH_THREAD
////////////////////////////////////////////////////////////////////////////////
/// \fn void setPCA9685BusMutex ( pthread_mutex_t * const mutex )
/// \param[ in ] mutex: pointer on mutex
/// \biref set a mutex for the PCA busif multiple bus used for multiple PCA, you
///     should set mutex each time is needed or manage mutex manualy
////////////////////////////////////////////////////////////////////////////////
void setPCA9685BusMutex ( pthread_mutex_t * const mutex );

////////////////////////////////////////////////////////////////////////////////
/// \fn void clearPCA9685BusMutex ( void );
/// \brief clear mutex set for PCA bus
////////////////////////////////////////////////////////////////////////////////
void clearPCA9685BusMutex ( void );
#endif

#endif