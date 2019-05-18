/*
 * Menrva - Over-Engineered Tunable Android Audio Effects
 * Copyright (C) 2019 Justin Giannone (aka Jman420)
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

package com.monkeystable.menrva;

import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.os.Build;

import com.monkeystable.menrva.dependencyInjection.components.AppComponents;
import com.monkeystable.menrva.dependencyInjection.components.DaggerAppComponents;

import dagger.android.AndroidInjector;
import dagger.android.DaggerApplication;

public class MenrvaApp extends DaggerApplication {
    private AppComponents _AppComponents;

    public MenrvaApp() {
        _AppComponents = DaggerAppComponents.create();
    }

    @Override
    public void onCreate() {
        super.onCreate();

        createNotificationChannel();
    }

    @Override
    protected AndroidInjector<? extends DaggerApplication> applicationInjector() {
        return _AppComponents;
    }

    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.O) {
            return;
        }

        NotificationChannel serviceChannel = new NotificationChannel(NotificationChannels.SERVICE_CHANNEL_ID,
                                                                     NotificationChannels.SERVICE_CHANNEL_NAME,
                                                                     NotificationManager.IMPORTANCE_LOW);
        NotificationManager notificationManager = getSystemService(NotificationManager.class);
        notificationManager.createNotificationChannel(serviceChannel);
    }
}
