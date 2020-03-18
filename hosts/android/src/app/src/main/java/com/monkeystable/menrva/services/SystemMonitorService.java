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

import android.content.Intent;
import android.os.IBinder;

import com.monkeystable.menrva.NotificationChannels;
import com.monkeystable.menrva.R;
import com.monkeystable.menrva.abstracts.ServiceBase;
import com.monkeystable.menrva.interfaces.INotificationHandler;

import javax.inject.Inject;

public class SystemMonitorService extends ServiceBase {
    private final int SERVICE_ID = 420;

    @Inject
    protected INotificationHandler _NotificationHandler;

    @Override
    public int getServiceId() {
        return SERVICE_ID;
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO : Draft & Return Service API Surface

        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();

        _NotificationHandler.createNotification(this, NotificationChannels.SERVICE_CHANNEL_ID, NotificationChannels.SERVICE_CHANNEL_NAME);
        _NotificationHandler.setNotification(this, NotificationChannels.SERVICE_CHANNEL_ID, "Waking up...", R.drawable.ic_service_notification);

        // Initialize AudioEffectInterface Collection

        // Wrap Current Output Device as Finite State Machine

        // Initialize & Register Broadcast Receivers
        //   - AudioSession Created/Destroyed
        //
        //   - Bluetooth Connected/Disconnected
        //   - Headset Plug/Unplug
        //   - USB Audio Plug/Unplug

    }
}
