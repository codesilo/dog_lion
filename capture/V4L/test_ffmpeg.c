/*
 * =====================================================================================
 *       Copyright (c), 2013-2020, Goke C&S.
 *       Filename:  test_ffmpeg.c
 *
 *    Description:  测试ffmpge库函数的调用
 *         Others:
 *   
 *        Version:  1.0
 *        Date:  Tuesday, May 27, 2014 12:43:47 HKT
 *       Revision:  none
 *       Compiler:  arm-gcc
 *
 *         Author:  Sean , houwentaoff@gmail.com
 *   Organization:  Goke
 *        History:  Tuesday, May 27, 2014 Created by SeanHou
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>

#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
    printf("hello world!\n");
    av_register_all();

    return EXIT_SUCCESS;
}
