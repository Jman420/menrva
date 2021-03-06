CONFIG_FORMAT=$1
AUDIO_EFFECTS_CONFIG_FILE=$2

LIBRARY_FILE_NAME="libMenrvaEngine.so"
MODULE_NAME="menrva"
EFFECT_LIBRARY_NAME="$MODULE_NAMEModule"
EFFECT_ENGINE_NAME="$MODULE_NAMEEngine"
EFFECT_UUID="a91fdfe4-d09e-11e8-a8d5-f2801f1b9fd1"

case $CONFIG_FORMAT in
  xml)
    sed -i "/$MODULE_NAME/d" $AUDIO_EFFECTS_CONFIG_FILE
    sed -i "/<libraries>/a\        <library name=\"$EFFECT_LIBRARY_NAME\" path=\"$LIBRARY_FILE_NAME\"/>" $AUDIO_EFFECTS_CONFIG_FILE
    sed -i "/<effects>/a\        <effect name=\"$EFFECT_ENGINE_NAME\" library=\"$EFFECT_LIBRARY_NAME\" uuid=\"$EFFECT_UUID\"/>" $AUDIO_EFFECTS_CONFIG_FILE
    ;;
  conf)
    sed -i "/$EFFECT_LIBRARY_NAME/,+2d" $AUDIO_EFFECTS_CONFIG_FILE
    sed -i "/$EFFECT_ENGINE_NAME/,+3d" $AUDIO_EFFECTS_CONFIG_FILE
    sed -i "/libraries {/a\  $EFFECT_LIBRARY_NAME {\n    path /vendor/lib/soundfx/$LIBRARY_FILE_NAME\n  }" $AUDIO_EFFECTS_CONFIG_FILE
    sed -i "/effects {/a\  $EFFECT_ENGINE_NAME {\n    library $EFFECT_LIBRARY_NAME\n    uuid $EFFECT_UUID\n  }" $AUDIO_EFFECTS_CONFIG_FILE
    ;;
esac
