// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


import QtQuick 2.0
import QtCamera 1.0

CameraToolIcon {
    width: 55
    height: 55
    opacity: 0.5

    border.color: cameraStyle.borderColor
    border.width: 1
    radius: width / 3

    color: pressed ? cameraStyle.pressedColor : cameraStyle.releasedColor

    iconSource: cameraTheme.modeButtonIconId

    onClicked: {
        if (plugins.count <= 1) {
            return;
        } else if (plugins.count == 2) {
            // just switch
            var plugin = plugins.at(0)
            if (plugin.uuid != settings.plugin) {
                settings.plugin = plugin.uuid;
            } else {
                plugin = plugins.at(1)
                settings.plugin = plugin.uuid;
            }
        } else {
            pluginSelector.show()
        }
    }
}
