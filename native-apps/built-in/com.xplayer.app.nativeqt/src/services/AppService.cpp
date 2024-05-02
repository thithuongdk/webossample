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

#include "AppService.h"
#include "AppMain.h"

#define APP_MAIN AppMain::instance()

AppService* AppService::m_instance = nullptr;

static pbnjson::JValue convertStringToJson(const char *rawData)
{
    pbnjson::JInput input(rawData);
    pbnjson::JSchema schema = pbnjson::JSchemaFragment("{}");
    pbnjson::JDomParser parser;
    if (!parser.parse(input, schema)) {
        return pbnjson::JValue();
    }
    return parser.getDom();
}

static std::string convertJsonToString(const pbnjson::JValue json)
{
    return pbnjson::JGenerator::serialize(json, pbnjson::JSchemaFragment("{}"));
}

AppService* AppService::instance(QObject* parent)
{
    if(m_instance==nullptr) {
        m_instance = new AppService(parent);
    }
    return m_instance;
}

AppService::AppService(QObject *parent)
    : QObject(parent)
    , m_mainLoop(g_main_loop_new(nullptr, false))
    , m_serviceHandle(nullptr)
{
    m_appId = std::string("com.xplayer.app.nativeqt");
    m_serviceHandle = acquireHandle();
}

AppService::~AppService()
{
    clearHandle();
    if (m_mainLoop) {
        g_main_loop_quit(m_mainLoop); // optional!
        g_main_loop_unref(m_mainLoop);
        m_mainLoop = nullptr;
    }
}

LSHandle* AppService::acquireHandle()
{
    LSError lserror;
    LSErrorInit(&lserror);
    LSHandle* handle = nullptr;
    if (!LSRegister(m_appId.c_str(), &handle, &lserror)) {
        LSErrorPrint(&lserror, stderr);
        return nullptr;
    }
    if (!LSGmainAttach(handle, m_mainLoop, &lserror)) {
        LSErrorPrint(&lserror, stderr);
        return nullptr;
    }
    return handle;
}

void AppService::clearHandle()
{
    LSError lserror;
    LSErrorInit(&lserror);
    if (m_serviceHandle) {
        LSUnregister(m_serviceHandle, &lserror);
        m_serviceHandle = nullptr;
    }
}

bool AppService::registerAppCallback(LSHandle* sh, LSMessage* msg, void* context)
{
    Q_UNUSED(sh);
    PmLogInfo(Log::getPmLogContext(), "REGISTER_CALLBACK", 1, PMLOGJSON("payload", LSMessageGetPayload(msg)),  " ");
    pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
    if (!response["returnValue"].asBool()) return false;
    if (response.hasKey("event")) {
        std::string event = response["event"].asString();
        PmLogInfo(Log::getPmLogContext(), "REGISTER_CALLBACK", 1, PMLOGKS("event", event.c_str()),  " ");
        if (!strcmp(event.c_str(),"registered")) {
            if (context != nullptr) {
                emit ((AppService*)context)->createWindow();
            }
        } else if (!strcmp(event.c_str(),"relaunch")
                || !strcmp(event.c_str(),"pause")
                || !strcmp(event.c_str(),"close")) {
        }
    }
    return true;
}

bool AppService::registerApp()
{
    LSError lserror;
    LSErrorInit(&lserror);
    LSHandle* handle = getHandle();
    if (!LSCall(handle, "luna://com.webos.service.applicationmanager/registerApp", "{}",
                AppService::registerAppCallback, this, NULL, &lserror)) {
        LSErrorPrint(&lserror, stderr);
        return false;
    }
    return true;
}