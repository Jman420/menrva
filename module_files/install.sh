BACKEND_DIR="$MODPATH/common/backend"
ARCH_BACKEND_DIR="$BACKEND_DIR/$ARCH32"
VENDOR_LIB_DIR="$MODPATH/system/vendor/$ARCH32"
ORIG_VENDOR_ETC_DIR="$ORIGDIR/vendor/etc"
AUDIO_EFFECTS_CONFIG_FILE="audio_effects.xml"
ALT_AUDIO_EFFECTS_CONFIG_FILE="audio_effects.conf"
CONFIG_FORMAT="xml"
LIB_DIR="lib"

copy_backend_files() {
  cp_ch -n $ARCH_BACKEND_DIR/libc++_shared.so $VENDOR_LIB_DIR/libc++_shared.so
  cp_ch -n $ARCH_BACKEND_DIR/libfftw3.so $VENDOR_LIB_DIR/libfftw3.so
  cp_ch -n $ARCH_BACKEND_DIR/libkissfft.so $VENDOR_LIB_DIR/libkissfft.so
  cp_ch -n $ARCH_BACKEND_DIR/libMenrvaEngine.so $VENDOR_LIB_DIR/soundfx/libMenrvaEngine.so

  cp_ch -n $ORIGDIR/system/$LIB_DIR/libstdc++.so $VENDOR_LIB_DIR/libstdc++.so
}

copy_backend_files

if [ $IS64BIT ]; then
  ARCH_BACKEND_DIR="$BACKEND_DIR/$ARCH"
  VENDOR_LIB_DIR="$MODPATH/system/vendor/$ARCH"
  LIB_DIR="lib64"
  
  copy_backend_files
fi

if [ ! -f $ORIG_VENDOR_ETC_DIR/$AUDIO_EFFECTS_CONFIG_FILE ] && [ -f $ORIG_VENDOR_ETC_DIR/$ALT_AUDIO_EFFECTS_CONFIG_FILE ]; then
  AUDIO_EFFECTS_CONFIG_FILE=ALT_AUDIO_EFFECTS_CONFIG_FILE
  CONFIG_FORMAT="conf"
fi
MODULE_AUDIO_EFFECTS_CONFIG_FILE="$VENDOR_LIB_DIR/etc/$AUDIO_EFFECTS_CONFIG_FILE"
cp_ch -n $ORIGDIR/vendor/etc/$AUDIO_EFFECTS_CONFIG_FILE $MODULE_AUDIO_EFFECTS_CONFIG_FILE

./patch_effects_config.sh "$CONFIG_FORMAT" "$MODULE_AUDIO_EFFECTS_CONFIG_FILE"

pm install $MODPATH/MenrvaApp.apk

rm -r $BACKEND_DIR
