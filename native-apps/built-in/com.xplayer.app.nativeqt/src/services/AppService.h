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

#ifndef APPSERVICES_H
#define APPSERVICES_H

#include <QObject>
#include <glib.h>
#include <string>
#include <luna-service2/lunaservice.h>
#include <pbnjson.hpp>
#include <QtGui/QWindow>
#include "Log.h"

class AppService: public QObject
{
    Q_OBJECT

private:
    static AppService* m_instance;
    explicit AppService(QObject* parent = nullptr);

public:
    static AppService* instance(QObject* parent = nullptr);
    virtual ~AppService() override;
    LSHandle* getHandle() const { return m_serviceHandle; }
    bool registerApp();
    static bool registerAppCallback(LSHandle* sh, LSMessage* msg, void* context);

signals :
    // Signals to GUI thread
    void createWindow();

protected:
    LSHandle* acquireHandle();
    void clearHandle();

private:
    GMainLoop* m_mainLoop;
    LSHandle* m_serviceHandle;
    std::string m_appId;
};
#endif
