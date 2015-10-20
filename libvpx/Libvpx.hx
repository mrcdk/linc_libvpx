package libvpx;

import cpp.UInt64;
import cpp.UInt32;

@:keep
@:include('linc_libvpx.h')
@:build(linc.Linc.touch())
@:build(linc.Linc.xml('libvpx'))
extern class Libvpx {

} //Empty

@:keep
@:include('linc_libvpx.h') @:native("::vpx_codec_ctx")
extern class CodecContext {
  var name(default, never):String;
  var iface(default, never):CodecInterface;
  var err(default, never):CodecError;
  var err_detail(default, never):String;
  var init_flags(default, never):UInt64;
  var config(default, never):CodecConfig;
}

@:keep
@:include('linc_libvpx.h') @:native("::vpx_codec_iface")
extern class CodecInterface {}

@:keep
@:include('linc_libvpx.h') @:native("::vpx_codec_dec_cfg")
extern class CodecDecCfg {
  var threads:UInt32;
  var w:Uint32;
  var h:Uint32;
}


typedef CodecConfig = {
  var dec:CodecDecCfg;
}

@:Enum
enum CodecError(Int) from Int to Int {
  /*!\brief Operation completed without error */
  var VPX_CODEC_OK = 0;

  /*!\brief Unspecified error */
  var VPX_CODEC_ERROR = 1;

  /*!\brief Memory operation failed */
  var VPX_CODEC_MEM_ERROR = 2;

  /*!\brief ABI version mismatch */
  var VPX_CODEC_ABI_MISMATCH = 3;

  /*!\brief Algorithm does not have required capability */
  var VPX_CODEC_INCAPABLE = 4;

  /*!\brief The given bitstream is not supported.
   *
   * The bitstream was unable to be parsed at the highest level. The decoder
   * is unable to proceed. This error \ref SHOULD be treated as fatal to the
   * stream. */
  var VPX_CODEC_UNSUP_BITSTREAM = 5;

  /*!\brief Encoded bitstream uses an unsupported feature
   *
   * The decoder does not implement a feature required by the encoder. This
   * return code should only be used for features that prevent future
   * pictures from being properly decoded. This error \ref MAY be treated as
   * fatal to the stream or \ref MAY be treated as fatal to the current GOP.
   */
  var VPX_CODEC_UNSUP_FEATURE = 6;

  /*!\brief The coded data for this stream is corrupt or incomplete
   *
   * There was a problem decoding the current frame.  This return code
   * should only be used for failures that prevent future pictures from
   * being properly decoded. This error \ref MAY be treated as fatal to the
   * stream or \ref MAY be treated as fatal to the current GOP. If decoding
   * is continued for the current GOP, artifacts may be present.
   */
  var VPX_CODEC_CORRUPT_FRAME = 7;

  /*!\brief An application-supplied parameter is not valid.
   *
   */
  var VPX_CODEC_INVALID_PARAM = 8;

  /*!\brief An iterator reached the end of list.
   *
   */
  var VPX_CODEC_LIST_END = 9;
}
