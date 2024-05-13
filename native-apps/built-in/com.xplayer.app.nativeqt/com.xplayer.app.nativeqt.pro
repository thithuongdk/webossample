# Copyright (c) 2020 LG Electronics, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0

TARGET = xplayer

CONFIG += qt
QT += \
        core \
        gui-private \
        qml \
        quick \
        quickcontrols2 \
        quickdialogs2 \


CONFIG += link_pkgconfig
PKGCONFIG += luna-service2 glib-2.0 pbnjson_cpp PmLogLib

SOURCES += \
            main.cpp \
            src/common/Log.cpp \
            src/interfaces/AppMain.cpp \
            src/services/AppService.cpp \

HEADERS += \
            src/common/Log.h \
            src/interfaces/AppMain.h \
            src/services/AppService.h \


INCLUDEPATH += \
            src/common \
            src/interfaces \
            src/services \

RESOURCES += qmls.qrc

INSTALL_APPDIR = $${WEBOS_INSTALL_WEBOS_APPLICATIONSDIR}/com.xplayer.app.nativeqt

target.path = $${INSTALL_APPDIR}

icon.path = $${INSTALL_APPDIR}
icon.files = music.svg

appinfo.path = $${INSTALL_APPDIR}
appinfo.files = appinfo.json

INSTALLS += target icon appinfo
