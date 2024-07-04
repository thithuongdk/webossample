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
    m_handle = acquireHandle(appName);
}

LSHandle* LunaService::acquireHandle(std::string appName)
{
    LSError lserror;
    LSErrorInit(&lserror);
    LSHandle *handle = nullptr;
    int itry=5;
    while(!handle && itry--) {
        if(!LSRegister(appName.c_str(), &handle, &lserror)) {
            PmLogError(getPmLogContext(), "LS_REGISTER", 0, "Unable to register to luna-bus");
            usleep(100000);
        }
    }
    if(!handle) {
        exit(1);
    }
    while (!LSGmainAttach(handle, m_mainLoop, &lserror)) {
        usleep(100000);
    }
    PmLogInfo(getPmLogContext(), "REGISTER", 1, PMLOGKS("status", "OK"), " ");
    
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

void LunaService::fLSCalln(std::string luna, std::string msg, bool(*cbF)(LSHandle *sh, LSMessage *lsm, void* udata), void* udata)
{
    // LSCall        (LSHandle * sh, const char * uri, const char * payload, LSFilterFunc callback, void * ctx, LSMessageToken * ret_token, LSError * lserror)
    LSError lserror;
    LSErrorInit(&lserror);
    PmLogInfo(getPmLogContext(), "LSCall", 2, PMLOGKS("luna", luna.c_str()), PMLOGJSON("msg", msg.c_str()), " ");
    if(!LSCall(m_handle, luna.c_str(), msg.c_str(), cbF, udata, nullptr, &lserror)) {
        PmLogError(getPmLogContext(), luna.c_str(), 0, "so sad");
        LSErrorPrint(&lserror, stderr);
    }
}

void LunaService::fLSCall1(std::string luna, std::string msg, bool(*cbF)(LSHandle *sh, LSMessage *lsm, void* udata), void* udata)
{
    // LSCallOneReply(LSHandle * sh, const char * uri, const char * payload, LSFilterFunc callback, void * ctx, LSMessageToken * ret_token, LSError * lserror)
    LSError lserror;
    LSErrorInit(&lserror);
    PmLogInfo(getPmLogContext(), "LSCallOneReply", 2, PMLOGKS("luna", luna.c_str()), PMLOGJSON("msg", msg.c_str()), " ");
    if(!LSCallOneReply(m_handle, luna.c_str(), msg.c_str(), cbF, udata, nullptr, &lserror)) {
        PmLogError(getPmLogContext(), luna.c_str(), 0, "so sad");
        LSErrorPrint(&lserror, stderr);
    }
}

void LunaService::fSetTimeOutLSCall(LSMessageToken token, int timeout_ms)
{
    // LSCallSetTimeout (LSHandle * sh, LSMessageToken token, int timeout_ms, LSError * lserror)
    LSError lserror;
    LSErrorInit(&lserror);
    PmLogInfo(getPmLogContext(), "LSCallSetTimeout", 1, PMLOGKS("timeout_ms", timeout_ms), " ");
    if(!LSCallSetTimeout(m_handle, token, timeout_ms, &lserror)) {
        PmLogError(getPmLogContext(), "LSCallSetTimeoutErr", 0, "so sad");
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