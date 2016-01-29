//This file is included, so this is required!
#ifndef _LINC_LIBVPX_CPP_
#define _LINC_LIBVPX_CPP_

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>

#include "./linc_libvpx.h"

#include <hxcpp.h>

namespace linc {

    namespace libvpx {

      Array<unsigned char> to_haxe_bytes(unsigned char* image_bytes, int length);

      struct DecInputContext load(char const *filename) {

        FILE *infile;
        const VpxInterface *interface = NULL;
        vpx_codec_dec_cfg_t cfg = {0, 0, 0};
        int dec_flags = 0;

        infile = fopen(filename, "rb");

        if (!infile) {
          fatal("Failed to open file '%s'", filename);
        }

        struct DecInputContext input;
        vpx_codec_ctx_t *decoder = (vpx_codec_ctx_t*)calloc(1, sizeof(vpx_codec_ctx_t));;
        struct VpxInputContext *vpx_ctx = (VpxInputContext*)calloc(1, sizeof(VpxInputContext));
        struct WebmInputContext *webm_ctx = (WebmInputContext*)calloc(1, sizeof(WebmInputContext));
        memset(&(input), 0, sizeof(input));
        input.vpx_ctx = vpx_ctx;
        input.webm_ctx = webm_ctx;
        input.decoder = decoder;
        input.file = infile;


        input.vpx_ctx->file = infile;
        if(file_is_webm(input.webm_ctx, input.vpx_ctx))
          input.vpx_ctx->file_type = FILE_TYPE_WEBM;
        else
          fatal("Input isn't a webm!");

        if(webm_guess_framerate(input.webm_ctx, input.vpx_ctx)) {
          fatal("Failed to guess framerate!");
        }

        interface = get_vpx_decoder_by_fourcc(input.vpx_ctx->fourcc);

        if(!interface)
          interface = get_vpx_decoder_by_index(0);

        if(vpx_codec_dec_init(input.decoder, interface->codec_interface(), &cfg, dec_flags)) {
          fatal("Failed to initialize decoder: %s\n", vpx_codec_error(input.decoder));
        }

        return input;

      }

      Dynamic read_frame(struct DecInputContext *input) {

        vpx_codec_iter_t iter = NULL;
        vpx_image_t *img;

        uint8_t *buf = NULL;
        size_t bytes_in_buffer = 0;
        size_t buffer_size = 0;
        int status = webm_read_frame(input->webm_ctx, &buf, &bytes_in_buffer, &buffer_size);
        if(status == 0) {
          if(vpx_codec_decode(input->decoder, buf, (unsigned int)bytes_in_buffer, NULL, 0)) {
            warn("Failed to decode frame: %s", vpx_codec_error(input->decoder));
          }
        } else {
          warn("EOF/Error %d", status);
        }

        if(status == 0) {
          img = vpx_codec_get_frame(input->decoder, &iter);
        } else {
          img = NULL;
        }

        /*
        {
          status:Int, (0 = success, 1 = EOF, -1 = Error)
          data:BytesData, // image bytes
          width:Int,
          height:Int,
          comp:Int, // RGBA = 4
        }
        */
        int width = 0;
        int height = 0;
        int comp = 4;
        unsigned char* rgba;


        hx::Anon result = hx::Anon_obj::Create();
        result->Add(HX_CSTRING("status"), status);

        if(img != NULL) {
          width = img->d_w;
          height = img->d_h;
          rgba = (unsigned char*)malloc(sizeof(unsigned char) * width * height * comp);
          if(img->fmt == VPX_IMG_FMT_I420) {
            ::libyuv::I420ToABGR(
              img->planes[VPX_PLANE_Y],img->stride[VPX_PLANE_Y],
              img->planes[VPX_PLANE_U],img->stride[VPX_PLANE_U],
              img->planes[VPX_PLANE_V],img->stride[VPX_PLANE_V],
              rgba, width * comp, width, height
            );

            ImgBytesData data = to_haxe_bytes(rgba, width * height * comp);
            result->Add(HX_CSTRING("data"), data);

          } else {
            warn("Img format unknown");
          }
        } else {
          warn("No image decoded!");
        }

        result->Add(HX_CSTRING("width"), width);
        result->Add(HX_CSTRING("height"), height);
        result->Add(HX_CSTRING("comp"), comp);


        return result;

      }

      void close(struct DecInputContext *input) {

        webm_free(input->webm_ctx);
        free(input->vpx_ctx);
        free(input->webm_ctx);
        free(input->decoder);
        fclose(input->file);

      }

      Array<unsigned char> to_haxe_bytes(unsigned char* image_bytes, int length) {

          Array<unsigned char> haxe_bytes = new Array_obj<unsigned char>(length,length);

          memcpy(haxe_bytes->GetBase(), image_bytes, length);

          free(image_bytes);

          return haxe_bytes;

      } //to_haxe_bytes

    } //empty namespace

    void usage_exit() {
      exit(EXIT_FAILURE);
    }

} //linc

#endif //_LINC_LIBVPX_CPP_
