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

package com.monkeystable.menrva.interfaces;

import android.app.Service;

import com.monkeystable.menrva.abstracts.ServiceBase;

public interface INotificationHandler {
    void createNotification(Service owner, String channelId, String channelName);
    void createNotification(Service owner, String channelId, String channelName, Integer importanceLevel);

    boolean setNotification(ServiceBase owner, String channelId, String caption);
    boolean setNotification(ServiceBase owner, String channelId, Integer icon);
    boolean setNotification(ServiceBase owner, String channelId, String caption, Integer icon);
}
