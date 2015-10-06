/*
 * =====================================================================================
 *       Copyright (c), 2013-2020, Goke C&S.
 *       Filename:  is_support_v4l.c
 *
 *    Description:  
 *         Others:
 *   
 *        Version:  1.0
 *        Date:  Saturday, August 16, 2014 10:18:41 HKT
 *       Revision:  none
 *       Compiler:  arm-gcc
 *
 *         Author:  Sean , houwentaoff@gmail.com
 *   Organization:  Goke
 *        History:  Saturday, August 16, 2014 Created by SeanHou
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/videodev2.h>
//#include <linux/videodev.h>

/*
  0 -- 不是v4l设备
  1 -- v4l 设备
  2 -- v4l2 设备
*/
int test_v4l_version(int fd)
{
   int ret = 0;
   char dummy[256]; 

    if (-1 != ioctl(fd,VIDIOC_QUERYCAP,dummy)) { 
        ret = 2;
    } 
  
//     else if (-1 != ioctl(fd,VIDIOCGCAP,dummy)) { 
//        ret = 1;
//    } 

    return ret;
}


int main(int argc,char * argv[])
{
   char dev_name[64] = "/dev/video0";
   int cam_fd =-1;

   if(argc>1)
       {
        strncpy(dev_name,argv[1],sizeof(dev_name)-1);
       }

   printf("open device %s\n",dev_name);
    cam_fd = open(dev_name,O_RDWR|O_NONBLOCK);

    if(cam_fd == -1)
        {
         printf("open failure \n");
         return -1;
        }

   switch(test_v4l_version(cam_fd))
       {
       case 0:
        printf("%s:fd %d isn't v4l deivce\n",dev_name,cam_fd);
        return -1;
        break;
    case 1:
        printf("\n### video4linux 1 device info [%s] ###\n",dev_name); 
        return -2;
        break;
    case 2:
        printf("\n### v4l2 device info [%s] ###\n",dev_name); 
        break;
       }
    

    
   
    close(cam_fd);

   return 0;
    
}

