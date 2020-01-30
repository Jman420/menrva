# Copied from Zackptg5's JamesDSP Magisk Module
# Modified by Jman420 for Menrva Magisk Module
(
while [ $(getprop sys.boot_completed) -ne 1 ] || [ "$(getprop init.svc.bootanim | tr '[:upper:]' '[:lower:]')" != "stopped" ]; do
  sleep 1
done

APP=$(pm list packages -3 | grep menrva)

if [ ! -d "$MODPATH" ]; then
  if [ "$APP" ]; then
    pm uninstall menrva
    rm -rf /data/data/menrva
  fi
  rm $0
  exit 0
elif [ "$APP" ]; then
  STATUS="$(pm list packages -d | grep 'menrva')"
  if [ -f "$MODPATH/disable" ] && [ ! "$STATUS" ]; then
    pm disable menrva
  elif [ ! -f "$MODPATH/disable" ] && [ "$STATUS" ]; then
    pm enable menrva
  fi
elif [ ! -f "$MODPATH/disable" ] && [ ! "$APP" ]; then
  pm install $MODPATH/MenrvaApp.apk
  pm disable menrva
  pm enable menrva
fi
)&
