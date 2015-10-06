/*
 * =====================================================================================
 *       Copyright (c), 2013-2020, Goke C&S.
 *       Filename:  v4l2.c
 *
 *    Description:  
 *         Others:
 *   
 *        Version:  1.0
 *        Date:  Sunday, May 25, 2014 10:25:27 HKT
 *       Revision:  none
 *       Compiler:  arm-gcc
 *
 *         Author:  Sean , houwentaoff@gmail.com
 *   Organization:  Goke
 *        History:  Created by SeanHou
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <linux/videodev2.h>
#include <jpeglib.h>

typedef struct
{
    void *start;
    int length;
}BUFTYPE;

BUFTYPE *user_buf;
int n_buffer = 0;
//打开摄像头设备
/**
 * @brief 
 *
 * @return 
 */
int open_camer_device()
{
    printf("open_camer_device\n");
    int fd;

    if((fd = open("/dev/video0",O_RDWR | O_NONBLOCK)) < 0)
    {
        perror("Fail to open");
        exit(EXIT_FAILURE);
    } 

    return fd;
}

int init_mmap(int fd)
{
    printf("init_mmap\n");
    int i = 0;
    struct v4l2_requestbuffers reqbuf;/*Sean Hou:  */

    bzero(&reqbuf,sizeof(reqbuf));
    reqbuf.count = 4;/*Sean Hou: buf数量 */
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    //申请视频缓冲区(这个缓冲区位于内核空间，需要通过mmap映射)
    //这一步操作可能会修改reqbuf.count的值，修改为实际成功申请缓冲区个数
    /*Sean Hou: 请求分配视频缓冲区 */
    if(-1 == ioctl(fd,VIDIOC_REQBUFS,&reqbuf))/*Sean Hou:reqbuf为实际分配的情况 */
    {
        perror("Fail to ioctl 'VIDIOC_REQBUFS'");
        exit(EXIT_FAILURE);
    }

    n_buffer = reqbuf.count;/*Sean Hou: 实际分配的count(可能会被ioctl改变) */

    printf("n_buffer = %d\n",n_buffer);/*Sean Hou: 这里是4? */

    user_buf = calloc(reqbuf.count+1,sizeof(*user_buf));
    if(user_buf == NULL){
        fprintf(stderr,"Out of memory\n");
        exit(EXIT_FAILURE);
    }
    //将内核缓冲区映射到用户进程空间
    for(i = 0; i < reqbuf.count; i ++)
    {
        struct v4l2_buffer buf;

        bzero(&buf,sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        //查询申请到内核缓冲区的信息
        if(-1 == ioctl(fd,VIDIOC_QUERYBUF,&buf))
        {
            perror("Fail to ioctl : VIDIOC_QUERYBUF");
            exit(EXIT_FAILURE);
        }

        printf("Sean: buf.length == [%d\n]", buf.length);

        user_buf[i].length = buf.length;
        user_buf[i].start = 
            mmap(
                NULL,/*start anywhere*/
                buf.length,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                fd,buf.m.offset
                );
        if(MAP_FAILED == user_buf[i].start)
        {
            perror("Fail to mmap");
            exit(EXIT_FAILURE);
        }
    }   
    printf("===>%s()line[%d]i[%d]\n", __func__, __LINE__, i);
    user_buf[i].start = malloc(409600);
    memset(user_buf[i].start, 0, 4096);

    return 0;
}
//初始化视频设备
/**
 * @brief 
 *
 * @param fd
 *
 * @return 
 */
int init_camer_device(int fd)
{
    printf("init_camer_device\n");
    printf("Sean ===> %s():\n", __func__);
    struct v4l2_fmtdesc fmt;
    struct v4l2_capability cap;
    struct v4l2_format stream_fmt;
    int ret;
    //当前视频设备支持的视频格式
    memset(&fmt,0,sizeof(fmt));
    fmt.index = 0;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

/*Sean Hou: 获取当前视频设备支持的视频格式 */
    while((ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmt)) == 0)
    {
        fmt.index ++ ;
/*Sean Hou: 4个字节 */
        printf("{pixelformat = %c%c%c%c},{description = %s}\n",
            fmt.pixelformat & 0xff,(fmt.pixelformat >> 8)&0xff,
            (fmt.pixelformat >> 16) & 0xff,(fmt.pixelformat >> 24)&0xff,
            fmt.description);
    }
    printf("Sean ===> %s(): 11111\n", __func__);
    //查询视频设备驱动的功能 capability
    ret = ioctl(fd,VIDIOC_QUERYCAP,&cap);
    /*Sean Hou:  */
    printf("driver:%s\n", cap.driver);
    printf("card :%s\n", cap.card);
    printf("bus_info:%s\n", cap.bus_info);
    printf("version:%08x\n", cap.version);
    printf("capabilities:%08X\n", cap.capabilities);
    if(ret < 0){
        perror("FAIL to ioctl VIDIOC_QUERYCAP");
        exit(EXIT_FAILURE);
    }
    //我的摄像头 只支持下面这2个,将capabilities 和 下面所在的与然后将其打印出来
    //判断是否是一个视频捕捉设备
    if(!(cap.capabilities & V4L2_BUF_TYPE_VIDEO_CAPTURE))
    {
        printf("The Current device is not a video capture device\n");
        exit(EXIT_FAILURE);

    }


    printf("Sean ===> %s(): LINE[%d]\n", __func__, __LINE__);
    //判断是否支持视频流形式
    if(!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        printf("The Current device does not support streaming i/o\n");
        exit(EXIT_FAILURE);
    }

    //设置摄像头采集数据格式，如设置采集数据的
    //长,宽，图像格式(JPEG,YUYV,MJPEG等格式)
    stream_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    /*Sean Hou: 必须是16的倍数？？ */
    stream_fmt.fmt.pix.width = 1366;
    stream_fmt.fmt.pix.height = 768;
    stream_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;//V4L2_PIX_FMT_MJPEG; 不能为其他 如 H264 MJPEG之类的么
//    stream_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_H264;//V4L2_PIX_FMT_MJPEG;
/*fmt.fmt.pix.field = V4L2_FIELD_INTERLACED; 意思就是采集数据的时候使用的是隔行扫描的方式，也就是说我将采集到的yuv写到文件中的方式是用隔行扫描的方式保存。*/
    stream_fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    /*-----------------------------------------------------------------------------
      注意：如果该视频设备驱动不支持你所设定的图像格式，视频驱动会重新修改struct v4l2_format结构体变量的值为该视频设备所支持的图像格式，所以在程序设计中，设定完所有的视频格式后，要获取实际的视频格式，要重新读取 struct v4l2_format结构体变量。
      使用VIDIOC_G_FMT设置视频设备的视频数据格式，VIDIOC_TRY_FMT验证视频设备的视频数据格式。
     *  
     *-----------------------------------------------------------------------------*/
    printf("Sean ===> %s(): LINE[%d]\n", __func__, __LINE__);
    /*Sean Hou: 设置命令 */
    if(-1 == ioctl(fd,VIDIOC_S_FMT,&stream_fmt))
    {
        perror("Fail to ioctl");
        exit(EXIT_FAILURE);
    }

    /*Sean Hou: 获取命令 */
    if(-1 == ioctl(fd,VIDIOC_G_FMT,&stream_fmt))
    {
        perror("Fail to ioctl");
        exit(EXIT_FAILURE);
    }

    printf("type:%d\n", stream_fmt.type);
    printf("width:%d\n", stream_fmt.fmt.pix.width);
    printf("height:%d\n", stream_fmt.fmt.pix.height);
    printf("field:%d\n", stream_fmt.fmt.pix.field);
    printf("bytesperline:%d\n", stream_fmt.fmt.pix.bytesperline);
    printf("sizeimage:%d\n", stream_fmt.fmt.pix.sizeimage);
    printf("colorspace:%d\n", stream_fmt.fmt.pix.colorspace);
    printf("priv:%d\n", stream_fmt.fmt.pix.priv);
    printf("raw_data:%s\n", stream_fmt.fmt.raw_data);
    //初始化视频采集方式(mmap)
    printf("Sean ===> %s(): LINE[%d]\n", __func__, __LINE__);
    init_mmap(fd);
    printf("Sean <=== %s():\n", __func__);

    return 0;
}
int start_capturing(int fd)
{
    printf("start_capturing\n");
    printf("Sean ===> %s():\n", __func__);
    unsigned int i;
    enum v4l2_buf_type type;

    //将申请的内核缓冲区放入一个队列中
    for(i = 0;i < n_buffer;i ++)
    {
        struct v4l2_buffer buf;

        bzero(&buf,sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;/*Sean Hou: 为何设置顺序在 mmap操作之后?? */
        buf.index = i;/*Sean Hou: 只有4个缓冲区个数 */

        /*Sean Hou: 投放一个空的buf到视频缓冲区queue */
        if(-1 == ioctl(fd,VIDIOC_QBUF,&buf))
        {
            perror("Fail to ioctl 'VIDIOC_QBUF'");
            exit(EXIT_FAILURE);
        }
    }
    //开始采集数据
    /*Sean Hou: 和select配合使用，select时应用程序可以从mmap中的地址取值 */
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(-1 == ioctl(fd,VIDIOC_STREAMON,&type)) /*Sean Hou:stream ON */
    {
        printf("i = %d.\n",i);
        perror("Fail to ioctl 'VIDIOC_STREAMON'");
        exit(EXIT_FAILURE);
    }

    return 0;
}
#if 1
int write_jpeg(char *filename,unsigned char *buf,int quality,int width, int height, int gray)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *fp;
    int i;
    unsigned char *line;
    int line_length;
    
    if (NULL == (fp = fopen(filename,"w"))) 
    {
                fprintf(stderr,"grab: can't open %s: %s\n",filename,strerror(errno));
                return -1;
    }
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, fp);
    cinfo.image_width  = width;
    cinfo.image_height = height;
    cinfo.input_components = gray ? 1: 3;
    cinfo.in_color_space = gray ? JCS_GRAYSCALE: JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);


    line_length = gray ? width : width * 3;
    for (i = 0, line = buf; i < height; i++, line += line_length)
        jpeg_write_scanlines(&cinfo, &line, 1);
    
    jpeg_finish_compress(&(cinfo));
    jpeg_destroy_compress(&(cinfo));
    fclose(fp);


    return 0;
}
#endif
int YUV422To420(unsigned char yuv422[], unsigned char yuv420[], int width, int height)
{        

       int ynum=width*height;
	   int i,j,k=0;
       printf("Sean ===> %s():\n", __func__);

	//得到Y分量
	   for(i=0;i<ynum;i++){
		   yuv420[i]=yuv422[i*2];
	   }
       printf("Sean ===> %s():line[%d]\n", __func__, __LINE__);
	//得到U分量
	   for(i=0;i<height;i++){
		   if((i%2)!=0)continue;
		   for(j=0;j<(width/2);j++){
			   if((4*j+1)>(2*width))break;
			   yuv420[ynum+k*2*width/4+j]=yuv422[i*2*width+4*j+1];
			  		   }
		    k++;
	   }
       printf("Sean ===> %s():line[%d]\n", __func__, __LINE__);
	   k=0;
	//得到V分量
	   for(i=0;i<height;i++){
		   if((i%2)==0)continue;
		   for(j=0;j<(width/2);j++){
			   if((4*j+3)>(2*width))break;
			   yuv420[ynum+ynum/4+k*2*width/4+j]=yuv422[i*2*width+4*j+3];
			  
		   }
		    k++;
	   }
	   
	   
       printf("Sean <=== %s():\n", __func__);

       return 1;
}
//将采集好的数据放到文件中
int process_image(void *addr,int length)
{
    printf("process_image\n");
    FILE *fp;
    static int num = 0;
    char picture_name[20];

    sprintf(picture_name,"picture%d.jpg",num ++);
    
    if (-1 == write_jpeg(picture_name, addr, 75, 640, 420, 0))
    {
        perror("write_jpeg error!\n");
    }
    usleep(500);
#if 0
    if((fp = fopen(picture_name,"w")) == NULL)
    {
        perror("Fail to fopen");
        exit(EXIT_FAILURE);
    }

    fwrite(addr,length,1,fp);
    usleep(500);

    fclose(fp);
#endif
    return 0;
}

/**
 * @brief 
 *
 * @param fd /dev/video设备文件
 * @param fp 264文件fp
 *
 * @return 
 */
int read_frame(int fd, FILE *fp)
{
    printf("read_frame\n");
    struct v4l2_buffer buf;
    unsigned int i;

    bzero(&buf,sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    //从队列中取缓冲区
    if(-1 == ioctl(fd,VIDIOC_DQBUF,&buf))
    {
        perror("Fail to ioctl 'VIDIOC_DQBUF'");
        exit(EXIT_FAILURE);
    }

    assert(buf.index < n_buffer);
//    memset(user_buf[4].start, 0, 1024);

    YUV422To420(user_buf[buf.index].start, user_buf[4].start, 640, 480);

//    fwrite(user_buf[buf.index].start, user_buf[buf.index].length, 1, fp);
    fwrite(user_buf[4].start, user_buf[buf.index].length, 1, fp);
    //usleep(500);


    //读取进程空间的数据到一个文件中
#if 0
    process_image(user_buf[buf.index].start,user_buf[buf.index].length);
#endif

    if(-1 == ioctl(fd,VIDIOC_QBUF,&buf)) /*Sean Hou: 将采集的视频写入到文件中后又将用户视频buf地址放入缓冲队列,供视频采集的填充 */
    {
        perror("Fail to ioctl 'VIDIOC_QBUF'");
        exit(EXIT_FAILURE);
    }

    return 1;
}
int mainloop(int fd)
{ 
    printf("mainloop\n");
    int count = 100;

    FILE *fp=NULL;

    if (NULL == (fp = fopen("test.yuv", "aw")))
    {
        perror("fopen error!!\n");
        return -2;
    }

    while(count -- > 0)
    {
        for(;;)
        {
            fd_set fds;
            struct timeval tv;
            int r;

            FD_ZERO(&fds);
            FD_SET(fd,&fds);

            /*Timeout*/
            tv.tv_sec = 2;
            tv.tv_usec = 0;

            r = select(fd + 1,&fds,NULL,NULL,&tv);

            if(-1 == r)
            {
                if(EINTR == errno)
                    continue;

                perror("Fail to select");
                exit(EXIT_FAILURE);
            }

            if(0 == r)
            {
                fprintf(stderr,"select Timeout\n");
                exit(EXIT_FAILURE);
            }

            if(read_frame(fd, fp))
                break;
        }
    }

    fclose(fp);

    return 0;
}
void stop_capturing(int fd)
{
    printf("stop_capturing\n");
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(-1 == ioctl(fd,VIDIOC_STREAMOFF,&type))
    {
        perror("Fail to ioctl 'VIDIOC_STREAMOFF'");
        exit(EXIT_FAILURE);
    }

    return;
}
void uninit_camer_device()
{
    printf("uninit_camer_device\n");
    unsigned int i;

    for(i = 0;i < n_buffer;i ++)
    {
        if(-1 == munmap(user_buf[i].start,user_buf[i].length))
        {
            exit(EXIT_FAILURE);
        }
    }

    free(user_buf);

    return;
}

void close_camer_device(int fd)
{
    printf("close_camer_device\n");
    if(-1 == close(fd))
    {
        perror("Fail to close fd");
        exit(EXIT_FAILURE);
    }

    return;
}

int main()
{
    int fd;       

    fd = open_camer_device();

    init_camer_device(fd);

    start_capturing(fd);

    mainloop(fd);

    stop_capturing(fd);

    uninit_camer_device(fd);

    close_camer_device(fd);

    return 0;
}
