#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> //file open
#include <unistd.h> 
#include<sys/ioctl.h> //ioctl
 
#define WR_VALUE _IOW('a',0,int*)
#define RD_VALUE _IOR('a',1,int*)
 
int main()
{
        int fd;
        unsigned int value, number;
 
        printf("\nOpening Driver\n");
        fd = open("/dev/adc0",O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file\n");
                return 0;
        }
 
        printf("Enter the channel number between 0-7 \n");
        scanf("%d",&number);
        printf("Writing Value to Driver\n");
        ioctl(fd, WR_VALUE, (unsigned int*) &number); 
 
        printf("Reading Value from channel\n");
        ioctl(fd, RD_VALUE, (unsigned int*) &value);
        printf("adc value is %d\n", value);
 
        printf("Closing Driver\n");
        close(fd);
}
