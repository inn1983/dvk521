/*
 * key testing utility 
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
int read_pin;

int main(int argc, char *argv[])
{
	unsigned int i;
	char read_pin_buf;

	fd = open("/dev/io_control",O_RDWR);
	if(fd < 0)
	{
		perror("error to open /dev/io_control_dev");
		exit(1);
	}
	
	while(1)
	{
		ret = read(fd, &read_pin,4);
        if(ret < 0)
        {
			perror("error to read\n");
            exit(1);
        } 
		read_pin_buf=read_pin;
		
		for(i=4;i<10;i++)
  		{
  			if(read_pin_buf==i && (i>6))
				printf("read the PIN: PI%d-(key%d)\n",i,i-7);
			if(read_pin_buf==i && (i<=6))
				printf("read the PIN: PI%d-(key%d)\n",i,i+1);
		}
		if(read_pin_buf==10)
			printf("read the PIN: PG9-(key3)\n");
		if(read_pin_buf==11)
			printf("read the PIN: PG11-(key4)\n");
	}
	
	close(fd);
	return 0;
}

