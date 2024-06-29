#include "LunaService.h"

LunaService* LunaService::m_instance = nullptr;

LunaService::LunaService(QObject *parent)
    : QObject(parent)
    , m_appName("")
    , m_mainLoop(nullptr)
    , m_handle(nullptr)
{
}

LunaService::~LunaService()
{
    clearHandle();
}

LunaService* LunaService::instance(QObject* parent)
{
    if(m_instance==nullptr) {
        m_instance = new LunaService(parent);
    }
    return m_instance;
}

void LunaService::init(std::string appName, GMainLoop *mainloop)
{
    m_appName = appName;
    m_mainLoop = mainloop;
    m_handle = acquireHandle();
}

LSHandle* LunaService::acquireHandle()
{
    LSError lserror;
    LSErrorInit(&lserror);
    LSHandle *handle = nullptr;
    int itry=5;
    while(!handle && itry--) {
        if(!LSRegister(m_appName.c_str(), &handle, &lserror)) {
            usleep(100000);
        }
    }
    if(!handle) {
        exit(1);
    }
    while (!LSGmainAttach(handle, m_mainLoop, &lserror)) {
        usleep(100000);
    }
    
    return handle;
}

void LunaService::clearHandle()
{
    LSError lserror;
    LSErrorInit(&lserror);
    if(m_handle && !LSUnregister(m_handle, &lserror)) {
        LSErrorPrint(&lserror, stderr);
    }
    if (m_mainLoop) {
        g_main_loop_quit(m_mainLoop); // optional!
        g_main_loop_unref(m_mainLoop);
    }
    m_handle = nullptr;
    m_mainLoop = nullptr;
}

void LunaService::fLSCall(std::string luna, std::string msg, bool(*cbF)(LSHandle *sh, LSMessage *lsm, void* udata), void* udata)
{
    LSError lserror;
    LSErrorInit(&lserror);
    if(!LSCall(m_handle, luna.c_str(), msg.c_str(), cbF, udata, nullptr, &lserror)) {
        LSErrorPrint(&lserror, stderr);
    }
}

void LunaService::fLSSubscriptionReply(std::string key, std::string msg)
{
    LSError lserror;
    LSErrorInit(&lserror);
    if(!LSSubscriptionGetHandleSubscribersCount(m_handle, key.c_str())) {
        if(!LSSubscriptionReply(m_handle, key.c_str(), msg.c_str(), &lserror)) {
            LSErrorPrint(&lserror, stderr);
        }
    }
}

void LunaService::fLSMessageReply(LSHandle *sh, LSMessage * msg, void *ctx, std::string key)
{
}