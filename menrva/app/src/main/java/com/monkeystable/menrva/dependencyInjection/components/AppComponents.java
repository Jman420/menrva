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

package com.monkeystable.menrva.dependencyInjection.components;

import com.monkeystable.menrva.abstracts.MenrvaLogger;
import com.monkeystable.menrva.dependencyInjection.modules.ActivitiesModule;
import com.monkeystable.menrva.dependencyInjection.modules.LogModule;
import com.monkeystable.menrva.dependencyInjection.modules.NotificationHandlerModule;
import com.monkeystable.menrva.dependencyInjection.modules.ServicesModule;
import com.monkeystable.menrva.interfaces.INotificationHandler;

import dagger.Component;
import dagger.android.AndroidInjectionModule;
import dagger.android.AndroidInjector;
import dagger.android.DaggerApplication;

@Component(modules = { AndroidInjectionModule.class, LogModule.class, ActivitiesModule.class, ServicesModule.class, NotificationHandlerModule.class, })
public interface AppComponents extends AndroidInjector<DaggerApplication> {
    MenrvaLogger getLogger();
    INotificationHandler getNotificationHandler();
}
