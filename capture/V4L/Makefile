
FFMPEG_DIR=ffmpeg-2.2.2

#INC=-I/usr/local/ffmpeg/include/
INC=-I/usr/local/ffmpeg/include/
SYS_LDIR:=-lpthread -lz  -lm -lbz2
#FFPEG_LIB:=-lavformat -lavcodec -lswscale -lavutil -lavfilter -lavdevice -lswresample 
LOCAL_LDIR+= \
	-L$(FFMPEG_DIR)/libavformat -lavformat\
	-L$(FFMPEG_DIR)/libavcodec -lavcodec \
	-L$(FFMPEG_DIR)/libswscale -lswscale \
	-L$(FFMPEG_DIR)/libavutil -lavutil \
	-L$(FFMPEG_DIR)/libavfilter -lavfilter \
	-L$(FFMPEG_DIR)/libavdevice -lavdevice \
	-L$(FFMPEG_DIR)/libswresample -lswresample 
LDIR=$(LOCAL_LDIR) $(SYS_LDIR)
#LDIR:=$(FFPEG_LIB) $(SYS_LDIR)


.PHONY:yuv2264 clean
v4l2:v4l2.o
	-@echo compiling v4l2
	-@$(CC) v4l2.c -ljpeg -o v4l2

yuv2264: 
#	-@$(CC) yuv2264.c $(INC) $(LDIR) -Wl,-rpath=/usr/local/lib:ffmpeg-2.2.2/lib -o yuv2264
	-@$(CC) yuv2264.c $(INC) $(LDIR)  -o yuv2264

test_ffmpeg:
	-@$(CC) test_ffmpeg.c $(INC) $(LDIR)  -o test_ffmpeg
clean:
	-@$(RM) *.o test_ffmpeg yuv2264 v4l2 -f
