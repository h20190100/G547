#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<fcntl.h>

#define MAJOR_NUM 220

#define IOCTL_SET_CHANNEL _IOW(MAJOR_NUM, 0, unsigned int)
#define IOCTL_SET_ALIGNMENT _IOW(MAJOR_NUM, 1, unsigned int)

int main()
{
while(1){
int fd,channel_no,alignment,p;
unsigned short int data;

fd = open("/dev/adc8",O_RDONLY);
if(fd<0){
	perror("open");
	//printf("unable to open adc device file \n");
	return -1;
	}

printf("Select the channel (0-7) \n");
scanf("%d", &channel_no);

if(channel_no >7)
{printf("Invalid channel \n"); 
 break;}

printf("the selected channel is %d \n",channel_no); 
ioctl(fd, IOCTL_SET_CHANNEL, &channel_no);

printf("Select the alignment (0-left alignment,1-right alignment) \n");
scanf("%d", &alignment);
/*if(alignment!=0,1)
{
printf("Invalid alignment selection \n");
break;}*/
if(alignment ==0){printf("left alignment data is ");}
else if(alignment ==1){printf("right alignment data is ");}
else {
printf("Invalid alignment selection \n");
break;}
ioctl(fd, IOCTL_SET_ALIGNMENT, &alignment);

if ( read(fd,&data,2) < 0) {
	perror("read");
	exit(EXIT_FAILURE);
}

printf(" %u \n",data);
printf("Enter 1 to exit and 2 to continue ");
scanf("%d",&p);
if(p==1)
break;
else if(p==2)
continue;
else
break;
//printf("closing adc \n");

close(fd);
}
}


