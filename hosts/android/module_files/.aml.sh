#libMenrvaEngine~a91fdfe4-d09e-11e8-a8d5-f2801f1b9fd1.sh

AUDIO_EFFECTS_PATH="/vendor/lib/soundfx"
LIBRARY_FILE_NAME="libMenrvaEngine.so"
EFFECT_LIBRARY_NAME="menrvaModule"
EFFECT_ENGINE_NAME="menrvaEngine"
EFFECT_UUID="a91fdfe4-d09e-11e8-a8d5-f2801f1b9fd1"

patch_cfgs $EFFECT_ENGINE_NAME $EFFECT_UUID $EFFECT_LIBRARY_NAME $AUDIO_EFFECTS_PATH/$LIBRARY_FILE_NAME
