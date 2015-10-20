package libvpx;

import haxe.io.BytesData;

typedef FrameData = {

  var status:Int;

  var width:Int;
  var height:Int;
  var comp:Int;
  var data:BytesData;

}

@:keep
@:include('linc_libvpx.h')
@:build(linc.Linc.touch())
@:build(linc.Linc.xml('libvpx'))
extern class WebmPlayer {

  @:native('linc::libvpx::load')
  static function load(filename:String):DecInputContext;

  @:native('linc::libvpx::read_frame')
  static function read_frame(ctx:DecInputContextRef):FrameData;

  @:native('linc::libvpx::close')
  static function close(ctx:DecInputContextRef):Void;

} //Empty

@:include('linc_libvpx.h')
@:native('linc::libvpx::DecInputContext')
private extern class DecInputContext {}

typedef DecInputContextPtr = cpp.Pointer<DecInputContext>;

@:native('::cpp::Struct<linc::libvpx::DecInputContext>')
private extern class DecInputContextStruct extends DecInputContext {}

@:native('::cpp::Reference<linc::libvpx::DecInputContext>')
private extern class DecInputContextRef extends DecInputContext {}
