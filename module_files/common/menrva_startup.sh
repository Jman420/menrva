# Copied from Zackptg5's JamesDSP Magisk Module
# Modified by Jman420 for Menrva Magisk Module
(
while [ $(getprop sys.boot_completed) -ne 1 ] || [ "$(getprop init.svc.bootanim | tr '[:upper:]' '[:lower:]')" != "stopped" ]; do
  sleep 1
done

PACKAGE_NAME="com.monkeystable.menrva"
PACKAGE_APK="MenrvaApp.apk"
APP=$(pm list packages -3 | grep $PACKAGE_NAME)

if [ ! -d "$MODPATH" ]; then
  if [ "$APP" ]; then
    pm uninstall $PACKAGE_NAME
    rm -rf /data/data/$PACKAGE_NAME
  fi
  rm $0
  exit 0
elif [ -d "$MODPATH" ] && [ "$APP" ]; then
  if [ -f "$MODPATH/disable" ]; then
    pm disable $PACKAGE_NAME
  elif [ ! -f "$MODPATH/disable" ]; then
    pm enable $PACKAGE_NAME
  fi
elif [ ! -f "$MODPATH/disable" ] && [ ! "$APP" ]; then
  pm install $MODPATH/$PACKAGE_APK
fi
)&
