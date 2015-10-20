#ifndef _LINC_LIBVPX_H_
#define _LINC_LIBVPX_H_

// #include "../lib/____"

#include "../lib/libvpx/third_party/libyuv/include/libyuv/convert.h"

#include "../lib/libvpx/vpx/vpx_decoder.h"
#include "../lib/libvpx/vpx/vp8dx.h"

#include "../lib/libvpx/tools_common.h"

#include "../lib/libvpx/webmdec.h"

#include <hxcpp.h>

namespace linc {

    namespace libvpx {

      struct DecInputContext {
        struct VpxInputContext *vpx_ctx;
        struct WebmInputContext *webm_ctx;
        vpx_codec_ctx_t *decoder;
        FILE *file;
      };

      typedef ::cpp::Struct<DecInputContext> DecInputCtx;
      typedef Array<unsigned char> ImgBytesData;

      extern struct DecInputContext load(char const *filename);
      extern Dynamic read_frame(struct DecInputContext *ctx);
      extern void close(struct DecInputContext *ctx);
    } //libvpx namespace

} //linc

void usage_exit();

#endif //_LINC_LIBVPX_H_
