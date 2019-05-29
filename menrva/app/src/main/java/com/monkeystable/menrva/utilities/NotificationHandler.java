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

package com.monkeystable.menrva.utilities;

import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.os.Build;
import android.support.v4.app.NotificationCompat;

import com.monkeystable.menrva.abstracts.ServiceBase;
import com.monkeystable.menrva.interfaces.INotificationHandler;

import java.util.Hashtable;

public class NotificationHandler implements INotificationHandler {
    private Hashtable<String, NotificationCompat.Builder> _NotificationBuilders;

    public NotificationHandler() {
        _NotificationBuilders = new Hashtable<>();
    }

    @Override
    public void createNotification(Service context, String channelId, String channelName) {
        createNotification(context, channelId, channelName, null);
    }

    @Override
    public void createNotification(Service context, String channelId, String channelName, Integer importanceLevel) {
        if (_NotificationBuilders.containsKey(channelId)) {
            return;
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            if (importanceLevel == null) {
                importanceLevel = NotificationManager.IMPORTANCE_LOW;
            }

            NotificationChannel serviceChannel = new NotificationChannel(channelId, channelName, importanceLevel);
            NotificationManager notificationManager = context.getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(serviceChannel);
        }

        NotificationCompat.Builder notificationBuilder = new NotificationCompat.Builder(context, channelId)
                .setContentTitle(channelName);
        _NotificationBuilders.put(channelId, notificationBuilder);
    }

    @Override
    public boolean setNotification(ServiceBase context, String channelId, String caption) {
        return setNotification(context, channelId, caption, null);
    }

    @Override
    public boolean setNotification(ServiceBase context, String channelId, Integer icon) {
        return setNotification(context, channelId, null, icon);
    }

    @Override
    public boolean setNotification(ServiceBase context, String channelId, String caption, Integer icon) {
        NotificationCompat.Builder notificationBuilder = _NotificationBuilders.get(channelId);
        if (notificationBuilder == null) {
            return false;
        }

        if (caption != null) {
            notificationBuilder.setContentText(caption);
        }
        if (icon != null) {
            notificationBuilder.setSmallIcon(icon);
        }

        context.startForeground(context.getServiceId(), notificationBuilder.build());
        return true;
    }
}
