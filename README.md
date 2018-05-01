# pca9685


## example 
How to use this code with a servo motor plug to the 15th connector of a pca9685 shield.

```C
#include <stdlib.h>
#include <unistd.h>

#include "pca9685/pca9685.h"

int main ( int argc, char * argv[] )
{
	int i = 0;
	int pca9685 = 0;

	if ( openPCA9685 ( "/dev/i2c-1", 0x40, &pca9685 ) )
	{
		return ( 1 );
	}

	for ( i = 200; i < 400; i += 2 )
	{
		setPCA9685PWM ( 15, 0, i, pca9685 );
		usleep ( 100000 );
	}

	closePCA9685 ( pca9685 );

	return ( 0 );
}
```
