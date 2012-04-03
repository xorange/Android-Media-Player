#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"

#include "SDL.h"
#include "SDL_thread.h"

#include <android/log.h>
#define INFO(msg) __android_log_write(ANDROID_LOG_INFO,"testff.c",msg);

int main(int argc, char *argv[])
{
  AVFormatContext *pFormatCtx;
  int             i, videoStream;
  AVCodecContext  *pCodecCtx;
  AVCodec         *pCodec;
  AVFrame         *pFrame; 
  AVPacket        packet;
  int             frameFinished;
  float           aspect_ratio;
  static struct SwsContext *img_convert_ctx;

INFO("main begin. argv[1] as follow");
INFO(argv[1]);

/*****************************************************/
/* FFmpeg */
/*****************************************************/

  av_register_all();

  if(av_open_input_file(&pFormatCtx, argv[1], NULL, 0, NULL)!=0)
	INFO("failed av_open_input_file ");
  
  if(av_find_stream_info(pFormatCtx)<0)
    	INFO("failed av_find_stream_info");

INFO("stream info found");

  videoStream=-1;
  for(i=0; i<pFormatCtx->nb_streams; i++)
    if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
      videoStream=i;
      break;
    }
  if(videoStream==-1)
    	INFO("failed videostream == -1");

INFO("videoStream found");
  
  pCodecCtx=pFormatCtx->streams[videoStream]->codec;
  
  pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
  if(pCodec==NULL) {
    INFO("Unsupported codec!");
  }
  
  if(avcodec_open(pCodecCtx, pCodec)<0)
    INFO("failed codec_open");
  
  pFrame=avcodec_alloc_frame();

/*****************************************************/
/* /FFmpeg */
/*****************************************************/

INFO("codec name:");
INFO(pCodec->name);
INFO("before this is the codec name. Getting into stream decode:");
/*****************************************************/
/* video stream */
/*****************************************************/

  i=0;
  while(av_read_frame(pFormatCtx, &packet)>=0) {

    if(packet.stream_index==videoStream) {
      avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, 
			   &packet);
      if(frameFinished) {
++i;
INFO("frame finished. hope could see this.");

	AVPicture pict;
/*
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
 pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
 PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize,
 0, pCodecCtx->height, pict.data, pict.linesize);
*/
      }

    }

    av_free_packet(&packet);
  }

/*****************************************************/
/* /video stream */
/*****************************************************/

  av_free(pFrame);
  
  avcodec_close(pCodecCtx);
  
  av_close_input_file(pFormatCtx);
  
  INFO("end of main");
}
