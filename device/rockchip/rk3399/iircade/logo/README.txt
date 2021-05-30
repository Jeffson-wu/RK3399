[About bootanimation.zip format]

 * https://android.googlesource.com/platform/frameworks/base/+/master/cmds/bootanimation/FORMAT.md
 * adb remount && adb push bootanimation.zip /system/media/ && adb reboot

[About logo.bmp format]

 * PC bitmap, Windows 3.x format
 * convert logo_src.bmp -alpha set -define bmp:format=bmp logo.bmp
