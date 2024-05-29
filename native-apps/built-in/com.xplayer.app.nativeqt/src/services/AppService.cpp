#include "AppService.h"

AppService* AppService::m_instance = nullptr;

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
    PmLogInfo(getPmLogContext(), "REGISTER_CALLBACK", 1, PMLOGJSON("payload", LSMessageGetPayload(msg)),  " ");
    pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
    if (!response["returnValue"].asBool()) return false;
    if (response.hasKey("event")) {
        std::string event = response["event"].asString();
        PmLogInfo(getPmLogContext(), "REGISTER_CALLBACK", 1, PMLOGKS("event", event.c_str()),  " ");
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