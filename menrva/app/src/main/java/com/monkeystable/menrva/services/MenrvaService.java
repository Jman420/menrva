/*
 * Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2019 Justin Giannone (aka Jman420)
 * File last modified : 4/20/19 9:16 AM
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

package com.monkeystable.menrva.services;

import android.app.Notification;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.support.v4.app.NotificationCompat;

import com.monkeystable.menrva.NotificationChannels;
import com.monkeystable.menrva.R;

public class MenrvaService extends Service {
    private final int SERVICE_ID = 420;

    private NotificationCompat.Builder _NotificationBuilder;

    @Override
    public IBinder onBind(Intent intent) {
        // TODO : Draft & Return Service API Surface
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();

        _NotificationBuilder = new NotificationCompat.Builder(this, NotificationChannels.SERVICE_CHANNEL_ID)
                .setContentTitle(NotificationChannels.SERVICE_CHANNEL_NAME)
                .setSmallIcon(R.drawable.ic_service_notification);
        updateNotification("Waking up Menrva...");
    }

    private void updateNotification(String caption) {
        Notification notification = _NotificationBuilder.setContentText(caption).build();
        startForeground(SERVICE_ID, notification);
    }
}