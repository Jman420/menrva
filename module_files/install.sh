LIB_DIR="lib"
if $IS64BIT; then
  LIB_DIR="lib64"
fi

BACKEND_DIR="$MODPATH/common/backend"
ARCH_BACKEND_DIR="$BACKEND_DIR/$ARCH"
VENDOR_LIB_DIR="$MODPATH/system/vendor/$LIB_DIR"
ORIG_VENDOR_ETC_DIR="$ORIGDIR/vendor/etc"
AUDIO_EFFECTS_CONFIG_FILE="audio_effects.xml"
ALT_AUDIO_EFFECTS_CONFIG_FILE="audio_effects.conf"
CONFIG_FORMAT="xml"

# Copy appropriate backend libraries to $MODPATH/system/vendor/lib & $MODPATH/system/vendor/lib/soundfx using $ARCH
cp_ch -n $ARCH_BACKEND_DIR/libc++_shared.so $VENDOR_LIB_DIR/libc++_shared.so
cp_ch -n $ARCH_BACKEND_DIR/libfftw3.so $VENDOR_LIB_DIR/libfftw3.so
cp_ch -n $ARCH_BACKEND_DIR/libkissfft.so $VENDOR_LIB_DIR/libkissfft.so
cp_ch -n $ARCH_BACKEND_DIR/libMenrvaEngine.so $VENDOR_LIB_DIR/soundfx/libMenrvaEngine.so

# Copy libstdc++ from /system/lib to $MODPATH/system/vendor/lib
cp_ch -n $ORIGDIR/system/$LIB_DIR/libstdc++.so $VENDOR_LIB_DIR/libstdc++.so

# Copy audio_effects config from /vendor/lib/etc to $MODPATH/system/vendor/etc
if [ ! -f $ORIG_VENDOR_ETC_DIR/$AUDIO_EFFECTS_CONFIG_FILE ] && [ -f $ORIG_VENDOR_ETC_DIR/$ALT_AUDIO_EFFECTS_CONFIG_FILE ]; then
  AUDIO_EFFECTS_CONFIG_FILE=ALT_AUDIO_EFFECTS_CONFIG_FILE
  CONFIG_FORMAT="conf"
fi
MODULE_AUDIO_EFFECTS_CONFIG_FILE="$VENDOR_LIB_DIR/etc/$AUDIO_EFFECTS_CONFIG_FILE"
cp_ch -n $ORIGDIR/vendor/etc/$AUDIO_EFFECTS_CONFIG_FILE $MODULE_AUDIO_EFFECTS_CONFIG_FILE

./patch_effects_config.sh "$CONFIG_FORMAT" "$MODULE_AUDIO_EFFECTS_CONFIG_FILE"

# Install frontend via pm
pm install $MODPATH/MenrvaApp.apk

# Clean up unused backend libraries
rm -r $BACKEND_DIR
