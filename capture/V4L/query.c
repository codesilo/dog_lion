/*
 * =====================================================================================
 *       Copyright (c), 2013-2020, Goke C&S.
 *       Filename:  query.c
 *
 *    Description:  
 *         Others:
 *   
 *        Version:  1.0
 *        Date:  Saturday, August 16, 2014 11:04:32 HKT
 *       Revision:  none
 *       Compiler:  arm-gcc
 *
 *         Author:  Sean , houwentaoff@gmail.com
 *   Organization:  Goke
 *        History:  Saturday, August 16, 2014 Created by SeanHou
 *
 * =====================================================================================
 */

/*
 * Author: Andrew Huang <bluedrum@163.com>
 * query v4l2 device capabilities
 * date: 2010/06/16
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/videodev2.h>

#define PRINT_V4L printf

#define PRINT_IOCTL_CMD(cmd) printf(" %15s = (0x%x)\n",#cmd,cmd)
#define PRINT_IOCTL_CMD2(cmd,n) printf(" %-20s(%d) = (0x%x)\n",#cmd,n,cmd)

#define PRINT_V4L_MEMBER(m) printf(" %-20s:",#m);
#define PRINT_V4L_STRING(m) printf(" %-20s:\"%s\"\n",#m,m)
#define PRINT_V4L_INT(m) printf(" %-20s:%d\n",#m,m)
#define PRINT_V4L_INTX(m) printf(" %-20s:0x%x\n",#m,m)

/* 显示enum 值的之一*/
#define TEST_V4L_FLAG0(m,f) printf("%s",((m & f)==f)?#f:"")

/* 显示一个组合值的所有内容 */
#define PRINT_V4L_FLAGS(m) printf(" %-20s:0x%x [",#m,m) /* 组合值开始*/
#define TEST_V4L_FLAG(m,f) printf("%s",((m & f)==f)?#f",":"") /* 组合值中间值,可以有多个*/
#define PRINT_V4L_FLAGS2(m) printf("]\n") /* 组合值结束*/


#define PRINT_INT(e) printf("%s=%d\n",#e,e)




int v4l2_query_video_cap(int fd)
{
 int ret;
struct v4l2_capability cap;
ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
if(ret < 0)
    {
     printf("get vidieo capability error,error code: %d \n", errno);
     return ret;
    } 
    PRINT_V4L("general info\n");
    PRINT_IOCTL_CMD(VIDIOC_QUERYCAP);

    PRINT_V4L_STRING(cap.driver);
    PRINT_V4L_STRING(cap.card);
    PRINT_V4L_STRING(cap.bus_info);
    PRINT_V4L_MEMBER(cap.version);printf("%u.%u.%u\n",(cap.version>>16)&0XFF, (cap.version>>8)&0XFF,cap.version&0xFF);
    
    PRINT_V4L_FLAGS(cap.capabilities);    
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_VIDEO_CAPTURE);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_VIDEO_OUTPUT);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_VIDEO_OVERLAY);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_VBI_CAPTURE);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_VBI_OUTPUT);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_SLICED_VBI_CAPTURE);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_SLICED_VBI_OUTPUT);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_RDS_CAPTURE); 
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_VIDEO_OUTPUT_OVERLAY);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_HW_FREQ_SEEK);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_TUNER);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_AUDIO);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_RADIO);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_READWRITE);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_ASYNCIO);
    TEST_V4L_FLAG(cap.capabilities,V4L2_CAP_STREAMING);

    PRINT_V4L_FLAGS2(cap.capabilities);
   return 0;

}

static void _show_capture_type(enum v4l2_buf_type type)
{
       TEST_V4L_FLAG(type,V4L2_BUF_TYPE_VIDEO_CAPTURE);
        TEST_V4L_FLAG(type,V4L2_BUF_TYPE_VIDEO_OUTPUT);
        TEST_V4L_FLAG(type,V4L2_BUF_TYPE_VIDEO_OVERLAY);
        TEST_V4L_FLAG(type,V4L2_BUF_TYPE_VBI_CAPTURE);
        TEST_V4L_FLAG(type,V4L2_BUF_TYPE_VBI_OUTPUT);
        TEST_V4L_FLAG(type,V4L2_BUF_TYPE_SLICED_VBI_CAPTURE);
        TEST_V4L_FLAG(type,V4L2_BUF_TYPE_SLICED_VBI_OUTPUT);
        TEST_V4L_FLAG(type,V4L2_BUF_TYPE_VIDEO_OUTPUT_OVERLAY);
        TEST_V4L_FLAG(type,V4L2_BUF_TYPE_PRIVATE);    
}


//void show_video_std(v4l2_std_id std)
void show_video_std(v4l2_std_id std)
{
// printf("%s:std=%x\n",__FUNCTION__,std);
  TEST_V4L_FLAG(std,V4L2_STD_PAL_B);
    TEST_V4L_FLAG(std,V4L2_STD_PAL_B1);
    TEST_V4L_FLAG(std,V4L2_STD_PAL_G);
    TEST_V4L_FLAG(std,V4L2_STD_PAL_H);
    TEST_V4L_FLAG(std,V4L2_STD_PAL_I);
    TEST_V4L_FLAG(std,V4L2_STD_PAL_D);
    TEST_V4L_FLAG(std,V4L2_STD_PAL_D1);
    TEST_V4L_FLAG(std,V4L2_STD_PAL_M);
    TEST_V4L_FLAG(std,V4L2_STD_PAL_N);
    TEST_V4L_FLAG(std,V4L2_STD_PAL_Nc);
    TEST_V4L_FLAG(std,V4L2_STD_PAL_60);
    TEST_V4L_FLAG(std,V4L2_STD_NTSC_M);
    TEST_V4L_FLAG(std,V4L2_STD_NTSC_M_JP);
    TEST_V4L_FLAG(std,V4L2_STD_NTSC_443);
    TEST_V4L_FLAG(std,V4L2_STD_NTSC_M_KR);
    TEST_V4L_FLAG(std,V4L2_STD_SECAM_B);
    TEST_V4L_FLAG(std,V4L2_STD_SECAM_D);
    TEST_V4L_FLAG(std,V4L2_STD_SECAM_G);
    TEST_V4L_FLAG(std,V4L2_STD_SECAM_H);
    TEST_V4L_FLAG(std,V4L2_STD_SECAM_K);
    TEST_V4L_FLAG(std,V4L2_STD_SECAM_K1);
    TEST_V4L_FLAG(std,V4L2_STD_SECAM_L);
    TEST_V4L_FLAG(std,V4L2_STD_SECAM_LC);
    TEST_V4L_FLAG(std,V4L2_STD_ATSC_8_VSB);
    TEST_V4L_FLAG(std,V4L2_STD_ATSC_16_VSB);
}



int v4l2_enum_video_input(int fd)
{
  struct v4l2_input input;
  int ret;
  memset(&input, 0, sizeof(input));//initialize i.index=0;

  PRINT_V4L("inputs\n");
  
  while(ret=ioctl(fd, VIDIOC_ENUMINPUT, &input)>=0)
  {
    PRINT_IOCTL_CMD2(VIDIOC_ENUMINPUT,input.index);
    PRINT_V4L_INT(input.index);
    PRINT_V4L_STRING(input.name);
    
    PRINT_V4L_MEMBER(input.type);
    TEST_V4L_FLAG0(input.type,V4L2_INPUT_TYPE_TUNER);
    TEST_V4L_FLAG0(input.type,V4L2_INPUT_TYPE_CAMERA);
    PRINT_V4L("\n");

    PRINT_V4L_INT(input.audioset);    
    PRINT_V4L_INT(input.tuner);



    PRINT_V4L_FLAGS(input.std);
    
    show_video_std(input.std);
    PRINT_V4L_FLAGS2(input.std);
    
    
   //printf("input i.name:%s; i.index :%d i.type = %s\n",i.name, i.index, i.type);
   input.index++; 
  }

  return ret;
}

static void _show_pixel_format(unsigned int pixelformat )
{
    if(pixelformat == 0)
        {
         printf("unknow pixelformat %d\n",pixelformat);
         return ;
        }
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_RGB332);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_RGB444);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_RGB555);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_RGB565);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_RGB555X);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_RGB565X);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_BGR24);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_RGB24);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_BGR32);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_RGB32);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_GREY);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_Y16);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_PAL8);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YVU410);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YVU420);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YUYV);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_UYVY);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YUV422P);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YUV411P);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_Y41P);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YUV444);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YUV555);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YUV565);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YUV32);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_NV12);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_NV21);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YUV410);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YUV420);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YYUV);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_HI240);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_HM12);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_SBGGR8);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_SGBRG8);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_SGRBG10);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_SGRBG10DPCM8);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_SBGGR16);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_MJPEG);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_JPEG);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_DV);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_MPEG);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_WNVA);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_SN9C10X);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_PWC1);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_PWC2);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_ET61X251);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_SPCA501);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_SPCA505);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_SPCA508);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_SPCA561);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_PAC207);
        TEST_V4L_FLAG(pixelformat,V4L2_PIX_FMT_YVYU);    
}

int v4l2_enum_video_format(int fd,enum v4l2_buf_type type,char * title)
{
   struct v4l2_fmtdesc fmtdesc;
   int i=0;
   memset(&fmtdesc, 0, sizeof(fmtdesc));
   


   PRINT_V4L("%s\n",title);

   for(i=0;;i++)
       {
        
     fmtdesc.index = i; 
      fmtdesc.type = type;
     
     if(-1== ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc))
        break;
   
       
              
        PRINT_IOCTL_CMD(VIDIOC_ENUM_FMT);
        PRINT_V4L_INT(fmtdesc.index);
        
        PRINT_V4L_FLAGS(fmtdesc.type);
        _show_capture_type(fmtdesc.type); 
        PRINT_V4L_FLAGS2(fmtdesc.type);

        PRINT_V4L_INT(fmtdesc.flags);
        PRINT_V4L_STRING(fmtdesc.description);


        PRINT_V4L_FLAGS(fmtdesc.pixelformat);
        _show_pixel_format(fmtdesc.pixelformat);    
        PRINT_V4L_FLAGS2(fmtdesc.pixelformat);

        

        
     
        
       }
  return i;
  
}

#define v4l2_enum_foramt(fd,type) v4l2_enum_video_format(fd,type,#type)
#define v4l2_enum_video_capture(fd) v4l2_enum_foramt(fd,V4L2_BUF_TYPE_VIDEO_CAPTURE)
#define v4l2_enum_video_overlay(fd) v4l2_enum_foramt(fd,V4L2_BUF_TYPE_VIDEO_OVERLAY)
#define v4l2_enum_video_vbi_capture(fd) v4l2_enum_foramt(fd,V4L2_CAP_VBI_CAPTURE)



void _show_v4l2_field(unsigned v4l2_filed)
{
      PRINT_V4L_MEMBER(v4l2_filed);
      TEST_V4L_FLAG0(v4l2_filed,V4L2_FIELD_ANY);
     TEST_V4L_FLAG0(v4l2_filed,V4L2_FIELD_NONE);
     TEST_V4L_FLAG0(v4l2_filed,V4L2_FIELD_TOP);
     TEST_V4L_FLAG0(v4l2_filed,V4L2_FIELD_BOTTOM);
     TEST_V4L_FLAG0(v4l2_filed,V4L2_FIELD_INTERLACED);
     TEST_V4L_FLAG0(v4l2_filed,V4L2_FIELD_SEQ_TB);
     TEST_V4L_FLAG0(v4l2_filed,V4L2_FIELD_SEQ_BT);
     TEST_V4L_FLAG0(v4l2_filed,V4L2_FIELD_ALTERNATE);
     TEST_V4L_FLAG0(v4l2_filed,V4L2_FIELD_INTERLACED_TB);
     TEST_V4L_FLAG0(v4l2_filed,V4L2_FIELD_INTERLACED_BT);
     PRINT_V4L("\n");
}

int v4l2_get_capture_format(int fd)
{
    struct v4l2_format format; 
    memset(&format,0,sizeof(format)); 
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
         if (-1 == ioctl(fd,VIDIOC_G_FMT,&format))
         {
             perror("VIDIOC_G_FMT(VIDEO_CAPTURE)"); 
             return -1;
         }

     PRINT_IOCTL_CMD(VIDIOC_G_FMT);
     PRINT_V4L_FLAGS(format.type);
     _show_capture_type(format.type); 
     PRINT_V4L_FLAGS2(format.type);


     
     PRINT_V4L_INT(format.fmt.pix.width);
     PRINT_V4L_INT(format.fmt.pix.height);
     _show_pixel_format(format.fmt.pix.pixelformat);
     
     PRINT_V4L_INT(format.fmt.pix.bytesperline);
     PRINT_V4L_INT(format.fmt.pix.sizeimage);

     

     _show_v4l2_field(format.fmt.pix.field);

     

    return 0;         
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


    printf("### v4l2 device info [%s] ###\n",dev_name);

    
   v4l2_query_video_cap(cam_fd);
   v4l2_enum_video_input(cam_fd);

  v4l2_enum_video_capture(cam_fd);
   

   v4l2_get_capture_format(cam_fd);
    close(cam_fd);

   return 0;
    
}
