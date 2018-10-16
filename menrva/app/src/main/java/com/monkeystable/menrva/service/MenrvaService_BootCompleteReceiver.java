package com.monkeystable.menrva.service;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class MenrvaService_BootCompleteReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        context.startService(new Intent(context, MenrvaService.class));
    }
}
