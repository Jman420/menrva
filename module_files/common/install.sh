MOD_COMMON_DIR="$MODPATH/common"
BACKEND_DIR="$MOD_COMMON_DIR/backend"
MOD_VENDOR_DIR="$MODPATH/system/vendor"
ORIG_VENDOR_ETC_DIR="$ORIGDIR/vendor/etc"
AUDIO_EFFECTS_CONFIG_FILE="audio_effects.xml"
ALT_AUDIO_EFFECTS_CONFIG_FILE="audio_effects.conf"

install_backend_files() {
  TARGET_ARCH=$1
  VENDOR_DIR=$2
  LIB_DIR=$3
  TARGET_DIR="$VENDOR_DIR/$LIB_DIR"
  
  if [ $TARGET_ARCH == "arm" ]; then
    TARGET_ARCH="armeabi-v7a"
  fi
  
  mkdir -p $TARGET_DIR
  busybox unzip -j $MODPATH/MenrvaApp.apk "lib/$TARGET_ARCH/*.so" -x "*[Tt]est*" -d $TARGET_DIR
  
  mkdir $TARGET_DIR/soundfx
  mv $TARGET_DIR/libMenrvaEngine.so $TARGET_DIR/soundfx/libMenrvaEngine.so
  
  cp_ch -n $ORIGDIR/system/$LIB_DIR/libstdc++.so $TARGET_DIR/libstdc++.so
}

ui_print "Installing 32bit Backend Drivers..."
install_backend_files $ARCH32 $MOD_VENDOR_DIR "lib"

if [ $IS64BIT ]; then
  ui_print "Installing 64bit Backend Drivers..."
  install_backend_files $ABILONG $MOD_VENDOR_DIR "lib64"
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
. $MOD_COMMON_DIR/patch_effects_config.sh $CONFIG_FORMAT $MODULE_AUDIO_EFFECTS_CONFIG_FILE

ui_print "Installing Menrva Startup Script..."
install_script -l $MOD_COMMON_DIR/menrva_startup.sh
