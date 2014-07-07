
/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
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


#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static const char *device = "/dev/spidev0.0";
static uint8_t mode=3;
static uint8_t bits = 8;
static uint32_t speed = 50000;
static uint16_t delay;

int fd,fd_cs;
int ret = 0;
int write_on = 6;
int write_off = 5;


void _delay_ms( unsigned int x)
{
    unsigned int  i,j;
    for(i=0;i<x;i++)
       for(j=0;j<100;j++);
}

void ee_write(unsigned int BufferOffset,unsigned char data)
{
     
//     c_s=0;
	ret = write(fd_cs, &write_off, 4);
		if(ret < 0)
		{
			perror("error to write\n");
			exit(1);
		}

 //spi_transmit_byte(0x84);	
	uint8_t txb[1] = {
		0x84,
	};
	ret = write(fd,txb,ARRAY_SIZE(txb));
	if (ret <1)
		pabort("can't write");
	
	//spi_transmit_byte(0xff);		
	txb[0] = 0xff;
	ret = write(fd,txb,ARRAY_SIZE(txb));
	if (ret <1)
		pabort("can't write");
   
  //spi_transmit_byte((unsigned char)(BufferOffset>>8));  
	txb[0] = (unsigned char)(BufferOffset>>8);
	ret = write(fd,txb,ARRAY_SIZE(txb));
	if (ret <1)
		pabort("can't write");
		
	//spi_transmit_byte((unsigned char)BufferOffset);		
	txb[0] = (unsigned char)BufferOffset;
	ret = write(fd,txb,ARRAY_SIZE(txb));
	if (ret <1)
		pabort("can't write");
		
	//spi_transmit_byte(data);		        	
	txb[0] = data;
	ret = write(fd,txb,ARRAY_SIZE(txb));
	if (ret <1)
		pabort("can't write");	  
	  													
//    c_s=1;
	ret = write(fd_cs, &write_on, 4);
	if(ret < 0)
	{
		perror("error to write\n");
		exit(1);
	}

}

unsigned char ee_read(unsigned int BufferOffset)
{
	uint8_t txb[1] = {
		0,
	};
	uint8_t rxb[ARRAY_SIZE(txb)] = {0, 
	};   


	//	   c_s=0;
	ret = write(fd_cs, &write_off, 4);
		if(ret < 0)
		{
			perror("error to write\n");
			exit(1);
		}
	
	//spi_transmit_byte(0xD4);	
	txb[0] = 0xD4;
	ret = write(fd,txb,ARRAY_SIZE(txb));
	if (ret <1)
		pabort("can't write");
	
	//spi_transmit_byte(0xff);
	txb[0] = 0xff;
	ret = write(fd,txb,ARRAY_SIZE(txb));
	if (ret <1)
		pabort("can't write");
     
  //spi_transmit_byte((unsigned char)(BufferOffset>>8));   
	txb[0] = (unsigned char)(BufferOffset>>8);
	ret = write(fd,txb,ARRAY_SIZE(txb));
	if (ret <1)
		pabort("can't write"); 
		
	//spi_transmit_byte((unsigned char)BufferOffset);	
	txb[0] = (unsigned char)BufferOffset;
	ret = write(fd,txb,ARRAY_SIZE(txb));
	if (ret <1)
		pabort("can't write");
	
	//spi_transmit_byte(0xff);	
	txb[0] = 0xff;
	ret = write(fd,txb,ARRAY_SIZE(txb));
	if (ret <1)
		pabort("can't write");

	//spi_transmit_byte(0xff);
	ret = read(fd,rxb,ARRAY_SIZE(txb));
	if (ret <1)
		pabort("can't read");
		

	//	  c_s=1;
	ret = write(fd_cs, &write_on, 4);
		if(ret < 0)
		{
		perror("error to write\n");
		exit(1);
		} 		

  //temp=SSPBUF;	
  return rxb[0];	
}


int main(int argc, char *argv[])
{
	unsigned int i, num;

	fd_cs = open("/dev/io_control",O_RDWR);
	if(fd_cs < 0)
	{
		perror("error to open /dev/io_control\n");
		exit(1);
	}

	
	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");
	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);



 /* CS = High */
 ret = write(fd_cs, &write_on, 4);
	   if(ret < 0)
	   {
		   perror("error to write\n");
		   exit(1);
	   }
  sleep(1);


 /* Write and  read */
 printf("Start to write 0-255\n");
  for(num=0;num<256;num++)
	{
	ee_write((unsigned int)num,num);
	for(i=0;i<40;i++);				
	}
  
  printf("Start to read 0-255\n");
  for(num=0;num<256;num++)
	{
	printf("%d  ",ee_read((unsigned int)num));
	_delay_ms(50);
	}

  printf("\n");	

	
	close(fd);
	close(fd_cs);

	return ret;
}
