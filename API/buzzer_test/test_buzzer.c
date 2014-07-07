/*
 *buzzer test
 */
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

int fd;
int ret = 0;
int buzzer_on=3;
int buzzer_off=4;

void _delay_ms( unsigned int x)
{
    unsigned int  i,j;
    for(i=0;i<x;i++)
       for(j=0;j<1;j++);
}

int main(int argc, char *argv[])
{
	unsigned int i, num;
	char read_pin_buf;

	fd = open("/dev/io_control",O_RDWR);
	if(fd < 0)
	{
		perror("error to open /dev/io_control\n");
		exit(1);
	}
	printf("start buzzer test.....\n");
	
	for(i=0;i<10000;i++)
	{
		/*open buzzer*/
		ret = write(fd, &buzzer_on,4);
		if(ret < 0)
		{
		       perror("error to write\n");
		       exit(1);
		} 
		_delay_ms(i);
		
		/*close buzzer*/
		ret = write(fd, &buzzer_off,4);
        if(ret < 0)
        {
			perror("error to write\n");
			exit(1);
        } 
	    _delay_ms(i);
	}
	
	printf("Stop\n");
	/*close buzzer*/
    ret = write(fd, &buzzer_off,4);
	if(ret < 0)
	{
		perror("error to write\n");
		exit(1);
	} 
	
	close(fd);
	return 0;
}

