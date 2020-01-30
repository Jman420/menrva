MOD_COMMON_DIR="$MODPATH/common"
BACKEND_DIR="$MOD_COMMON_DIR/backend"
MOD_VENDOR_DIR="$MODPATH/system/vendor"
ORIG_VENDOR_ETC_DIR="$ORIGDIR/vendor/etc"
AUDIO_EFFECTS_CONFIG_FILE="audio_effects.xml"
ALT_AUDIO_EFFECTS_CONFIG_FILE="audio_effects.conf"

copy_backend_files() {
  cp_ch -n $ARCH_BACKEND_DIR/libc++_shared.so $MOD_VENDOR_DIR/$LIB_DIR/libc++_shared.so
  cp_ch -n $ARCH_BACKEND_DIR/libfftw3.so $MOD_VENDOR_DIR/$LIB_DIR/libfftw3.so
  cp_ch -n $ARCH_BACKEND_DIR/libkissfft.so $MOD_VENDOR_DIR/$LIB_DIR/libkissfft.so
  cp_ch -n $ARCH_BACKEND_DIR/libMenrvaEngine.so $MOD_VENDOR_DIR/$LIB_DIR/soundfx/libMenrvaEngine.so

  cp_ch -n $ORIGDIR/system/$LIB_DIR/libstdc++.so $MOD_VENDOR_DIR/$LIB_DIR/libstdc++.so
}

ui_print "Installing 32bit Backend Drivers..."
ARCH_BACKEND_DIR="$BACKEND_DIR/$ARCH32"
if [ $ARCH32 == "arm" ]; then
  ARCH_BACKEND_DIR="$BACKEND_DIR/armeabi-v7a"
fi
LIB_DIR="lib"
copy_backend_files

if [ $IS64BIT ]; then
  ui_print "Installing 64bit Backend Drivers..."
  ARCH_BACKEND_DIR="$BACKEND_DIR/$ABILONG"
  LIB_DIR="lib64"
  copy_backend_files
fi

ui_print "Copying Audio Effects Config file from vendor partition..."
CONFIG_FORMAT="xml"
if [ ! -f $ORIG_VENDOR_ETC_DIR/$AUDIO_EFFECTS_CONFIG_FILE ] && [ -f $ORIG_VENDOR_ETC_DIR/$ALT_AUDIO_EFFECTS_CONFIG_FILE ]; then
  AUDIO_EFFECTS_CONFIG_FILE=ALT_AUDIO_EFFECTS_CONFIG_FILE
  CONFIG_FORMAT="conf"
fi
MODULE_AUDIO_EFFECTS_CONFIG_FILE="$MOD_VENDOR_DIR/etc/$AUDIO_EFFECTS_CONFIG_FILE"
cp_ch -n $ORIGDIR/vendor/etc/$AUDIO_EFFECTS_CONFIG_FILE $MODULE_AUDIO_EFFECTS_CONFIG_FILE

ui_print "Patching Audio Effects Config File..."
. $MOD_COMMON_DIR/patch_effects_config.sh "$CONFIG_FORMAT" "$MODULE_AUDIO_EFFECTS_CONFIG_FILE"

ui_print "Installing Menrva Startup Script..."
install_script -l $MOD_COMMON_DIR/menrva_startup.sh
