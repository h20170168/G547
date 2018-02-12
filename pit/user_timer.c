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
        int fd,input;
        unsigned int value, number;
 
        printf("\nOpening Driver\n");
        fd = open("/dev/timer0",O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file\n");
                return 0;
        }
  	 printf( "press the following key\n" );
         printf( "(1) To set delay in kernal space\n" );
         printf( "(2) read back current counter value\n" );
        
	 scanf( "%d", &input );
       
        switch ( input ) 
      {
        case 1: printf("Enter the amount of delay you want in ms \n");
        scanf("%d",&number);
        ioctl(fd, WR_VALUE, (unsigned int*) &number); 
        break;
        case 2:
        printf("Reading current counter value \n");
        ioctl(fd, RD_VALUE, (unsigned int*) &value);
        printf("counter current value is %d\n", value);
        break;
	default :  printf( "bad input \n" );
	break;
	}
        printf("Closing Driver\n");
        close(fd);
}
