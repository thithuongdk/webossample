// Copyright (c) 2020 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#include "Log.h"
#include "AppService.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QWindow>
#include <QScreen>
#include <qpa/qplatformnativeinterface.h>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(true);
    PmLogInfo(getPmLogContext(), "MAIN_ARGV1", 1, PMLOGKFV("argv", "%s", argv[1]),  " ");
    GMainLoop *mainLoop = g_main_loop_new(nullptr, false);
    AppService::instance()->init("com.xplayer.app.nativeqt", mainLoop);
    g_main_loop_run(mainLoop);
    return app.exec();
}
