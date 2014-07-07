#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

int write_ret;
int write_on = 7;
int write_off = 15;

int main(int argc,char *argv[])
{
	int fd;
	fd = open("/dev/io_control",O_RDWR);
	if(fd < 0)
	{
		perror("error to open /dev/io_control\n");
		exit(1);
	}

	while(1)
	{
		for(write_on=7;write_on<=14;write_on++)
		{
			write_ret = write(fd, &write_on, 4);
			if(write_ret < 0)
			{
				perror("error to write\n");
				exit(1);
			}
			printf("led(%d) open ok!\n",(write_on-6));
			sleep(1);
			
			write_ret = write(fd, &write_off, 4);
			if(write_ret < 0)
			{
				perror("error to write\n");
				exit(1);
			}
		}
	}
	close(fd);
	return 0;
}

