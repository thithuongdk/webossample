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

#include <glib.h>
#include <string>
#include <luna-service2/lunaservice.h>
#include <PmLog.h>
#include <pbnjson.hpp>
#include <malloc.h>
#include <iostream>

static PmLogContext getPmLogContext()
{
    static PmLogContext s_context = 0;
    if (0 == s_context)
    {
        PmLogGetContext("NativeService", &s_context);
    }
    return s_context;
}

static pbnjson::JValue convertStringToJson(const char *rawData)
{
    pbnjson::JInput input(rawData);
    pbnjson::JSchema schema = pbnjson::JSchemaFragment("{}");
    pbnjson::JDomParser parser;
    if (!parser.parse(input, schema))
    {
        return pbnjson::JValue();
    }
    return parser.getDom();
}

static std::string convertJsonToString(const pbnjson::JValue json)
{
    return pbnjson::JGenerator::serialize(json, pbnjson::JSchemaFragment("{}"));
}

static bool onHello2(LSHandle *sh, LSMessage* message, void* ctx)
{
    PmLogInfo(getPmLogContext(), "HANDLE_HELLO2", 0, "hello method called");

    LSError lserror;
    LSErrorInit(&lserror);
    std::string key = "hello2";
    if (!LSSubscriptionAdd(sh, key.c_str(), message, &lserror))
    {
        return false;
    }

    PmLogInfo(getPmLogContext(), "HELLO_COUNT2", 0, std::to_string(LSSubscriptionGetHandleSubscribersCount(sh, key.c_str())).c_str());
    return true;
}

static bool onHello3(LSHandle *sh, LSMessage* message, void* ctx)
{
    PmLogInfo(getPmLogContext(), "HANDLE_HELLO3", 0, "hello method called");

    LSError lserror;
    LSErrorInit(&lserror);

    // _HEAPINFO heapInfo;
    // heapInfo._pentry = nullptr;
    // PmLogInfo(getPmLogContext(), "HEAPINFO_NOT_RELEASE", 0, "before");
    // while (_heapwalk(&heapInfo) == _HEAPOK) {
    //     PmLogInfo(getPmLogContext(), "HEAPINFO", 3,
    //             PMLOGJSON("Address",std::to_string(heapInfo._pentry).c_str()),
    //             PMLOGJSON("Size", std::to_string(heapInfo._size).c_str()),
    //             PMLOGJSON("Use", std::to_string(heapInfo._useflag).c_str()), " ");
    // }
    LSMessagePrint(message, stdout);
    std::cout << "HEAPINFO_NOT_RELEASE before" << std::endl;
    malloc_stats();

    std::string key = "hello";
    for(int i0=0; i0<4000; i0++) {
        for(int i1=0; i1<3; i1++) {
            if (!LSSubscriptionAdd(sh, key.c_str(), message, &lserror))
            {
                return false;
            }
        }

        PmLogInfo(getPmLogContext(), "HELLO_COUNT", 0, std::to_string(LSSubscriptionGetHandleSubscribersCount(sh, key.c_str())).c_str());
        if (LSSubscriptionGetHandleSubscribersCount(sh, key.c_str()))
        {
            LSSubscriptionIter *iter = nullptr;
            if(LSSubscriptionAcquire(sh, key.c_str(), &iter, &lserror))
            {
                while(LSSubscriptionHasNext(iter))
                {
                    LSMessage *msg = LSSubscriptionNext(iter);
                    PmLogInfo(getPmLogContext(), "HELLO_LSSubscriptionNext", 0, LSMessageGetSenderServiceName(msg));
                    std::string sjson = R"({"id":"hahahahahahaahha"})";
                    LSMessageReply(sh, msg, sjson.c_str(), &lserror);
                }
                // LSSubscriptionRelease(iter);
            }
        }
    }

    // heapInfo._pentry = nullptr;
    // PmLogInfo(getPmLogContext(), "HEAPINFO_NOT_RELEASE", 0, "after");
    // while (_heapwalk(&heapInfo) == _HEAPOK) {
    //     PmLogInfo(getPmLogContext(), "HEAPINFO", 3,
    //             PMLOGJSON("Address",std::to_string(heapInfo._pentry).c_str()),
    //             PMLOGJSON("Size", std::to_string(heapInfo._size).c_str()),
    //             PMLOGJSON("Use", std::to_string(heapInfo._useflag).c_str()), " ");
    // }
    std::cout << "HEAPINFO_NOT_RELEASE after" << std::endl;
    malloc_stats();

    PmLogInfo(getPmLogContext(), "HELLO_COUNT3", 0, std::to_string(LSSubscriptionGetHandleSubscribersCount(sh, key.c_str())).c_str());
    return true;
}

static bool onHello4(LSHandle *sh, LSMessage* message, void* ctx)
{
    PmLogInfo(getPmLogContext(), "HANDLE_HELLO4", 0, "hello method called");

    LSError lserror;
    LSErrorInit(&lserror);
    std::string key = "hello";
    if (!LSSubscriptionAdd(sh, key.c_str(), message, &lserror))
    {
        return false;
    }

    PmLogInfo(getPmLogContext(), "HELLO_COUNT", 0, std::to_string(LSSubscriptionGetHandleSubscribersCount(sh, key.c_str())).c_str());
    return true;
}

static bool onHello(LSHandle *sh, LSMessage* message, void* ctx)
{
    PmLogInfo(getPmLogContext(), "HANDLE_HELLO", 0, "hello method called");

    pbnjson::JValue reply = pbnjson::Object();
    if (reply.isNull())
        return false;

    reply.put("returnValue", true);
    reply.put("answer", "Hello, Native Service!!");

    LSError lserror;
    LSErrorInit(&lserror);

    // _HEAPINFO heapInfo;
    // heapInfo._pentry = nullptr;
    // PmLogInfo(getPmLogContext(), "HEAPINFO_RELEASE", 0, "before");
    // while (_heapwalk(&heapInfo) == _HEAPOK) {
    //     PmLogInfo(getPmLogContext(), "HEAPINFO", 3,
    //             PMLOGJSON("Address",std::to_string(heapInfo._pentry).c_str()),
    //             PMLOGJSON("Size", std::to_string(heapInfo._size).c_str()),
    //             PMLOGJSON("Use", std::to_string(heapInfo._useflag).c_str()), " ");
    // }
    LSMessagePrint(message, stdout);
    std::cout << "HEAPINFO_RELEASE before" << std::endl;
    malloc_stats();

    std::string key = "hello";
    for(int i0=0; i0<4000; i0++) {
        for(int i1=0; i1<3; i1++) {
            if (!LSSubscriptionAdd(sh, key.c_str(), message, &lserror))
            {
                return false;
            }
        }

        PmLogInfo(getPmLogContext(), "HELLO_COUNT", 0, std::to_string(LSSubscriptionGetHandleSubscribersCount(sh, key.c_str())).c_str());
        if (LSSubscriptionGetHandleSubscribersCount(sh, key.c_str()))
        {
            LSSubscriptionIter *iter = nullptr;
            if(LSSubscriptionAcquire(sh, key.c_str(), &iter, &lserror))
            {
                while(LSSubscriptionHasNext(iter))
                {
                    LSMessage *msg = LSSubscriptionNext(iter);
                    PmLogInfo(getPmLogContext(), "HELLO_LSSubscriptionNext", 0, LSMessageGetSenderServiceName(msg));
                    std::string sjson = R"({"id":"hahahahahahaahha"})";
                    LSMessageReply(sh, msg, sjson.c_str(), &lserror);
                }
                LSSubscriptionRelease(iter);
            }
        }
    }

    // heapInfo._pentry = nullptr;
    // PmLogInfo(getPmLogContext(), "HEAPINFO_RELEASE", 0, "after");
    // while (_heapwalk(&heapInfo) == _HEAPOK) {
    //     PmLogInfo(getPmLogContext(), "HEAPINFO", 3,
    //             PMLOGJSON("Address",std::to_string(heapInfo._pentry).c_str()),
    //             PMLOGJSON("Size", std::to_string(heapInfo._size).c_str()),
    //             PMLOGJSON("Use", std::to_string(heapInfo._useflag).c_str()), " ");
    // }
    std::cout << "HEAPINFO_RELEASE after" << std::endl;
    malloc_stats();

    if (!LSMessageReply(sh, message, reply.stringify().c_str(), &lserror))
    {
        PmLogError(getPmLogContext(), "HANDLE_HELLO", 0, "Message reply error!!");
        LSErrorPrint(&lserror, stdout);

        return false;
    }
    return true;
}

static bool cbGetTime(LSHandle *sh, LSMessage *msg, void *user_data)
{
    LSError lserror;
    LSErrorInit(&lserror);

    PmLogInfo(getPmLogContext(), "GETTIME_CALLBACK", 1, PMLOGJSON("payload", LSMessageGetPayload(msg)),  " ");

    pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
    bool successCallback = response["returnValue"].asBool();
    if (successCallback)
    {
        int64_t utc= response["utc"].asNumber<int64_t>();
        PmLogInfo(getPmLogContext(), "GETTIME_CALLBACK", 1, PMLOGKFV("UTC", "%lld", utc),  " ");
    }

    return true;
}

static LSMethod serviceMethods[] = {
    { "hello", onHello },
    { "hello2", onHello2 },
    { "hello3", onHello3 },
    { "hello4", onHello4 }
};

int main(int argc, char* argv[])
{
    PmLogInfo(getPmLogContext(), "SERVICE_MAIN", 0, "start com.example.service.native");

    LSError lserror;
    LSErrorInit(&lserror);

    GMainLoop* mainLoop = g_main_loop_new(nullptr, false);
    LSHandle *m_handle = nullptr;

    if(!LSRegister("com.example.service.native", &m_handle, &lserror))
    {
        PmLogError(getPmLogContext(), "LS_REGISTER", 0, "Unable to register to luna-bus");
        LSErrorPrint(&lserror, stdout);

        return false;
    }

    if (!LSRegisterCategory(m_handle, "/", serviceMethods, NULL, NULL, &lserror))
    {
        PmLogError(getPmLogContext(), "LS_REGISTER", 0, "Unable to register category and method");
        LSErrorPrint(&lserror, stdout);

        return false;
    }

    if(!LSGmainAttach(m_handle, mainLoop, &lserror))
    {
        PmLogError(getPmLogContext(), "LS_REGISTER", 0, "Unable to attach service");
        LSErrorPrint(&lserror, stdout);

        return false;
    }

    if (!LSCall(m_handle,
                "luna://com.webos.service.systemservice/clock/getTime",
                "{}",
                cbGetTime,
                NULL,
                NULL,
                &lserror))
    {
        PmLogError(getPmLogContext(), "LSCALL_GETTIME", 0, "Cannot call getTime");
        LSErrorPrint(&lserror, stderr);
    }

    g_main_loop_run(mainLoop);

    if(!LSUnregister(m_handle, &lserror))
    {
        PmLogError(getPmLogContext(), "LS_REGISTER", 0, "Unable to unregister service");
        LSErrorPrint(&lserror, stdout);

        return false;
    }

    g_main_loop_unref(mainLoop);
    mainLoop = nullptr;

    return 0;
}

