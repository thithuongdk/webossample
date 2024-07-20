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

const char *LunaService::fLSMessageGetPayload(LSMessage * msg) 
{
    return LSMessageGetPayload(msg);
}

void LunaService::fMessagePrintLogCB(LSMessage * msg) 
{
    PmLogInfo(getPmLogContext(), "CALLBACK", 0, LSMessageGetMethod(msg), LSMessageGetPayload(msg));
}

void LunaService::fMessagePrintLogCB(LSHandle* sh, LSMessage* msg, void* context) 
{
    Q_UNUSED(sh);
    Q_UNUSED(context);
    PmLogInfo(getPmLogContext(), "CALLBACK", 0, LSMessageGetMethod(msg), LSMessageGetPayload(msg));
}

void LunaService::fLSCalln(std::string luna, std::string msg, fCallBack cbF, void* udata)
{
    // LSCall        (LSHandle * sh, const char * uri, const char * payload, LSFilterFunc callback, void * ctx, LSMessageToken * ret_token, LSError * lserror)
    LSError lserror;
    LSErrorInit(&lserror);
    PmLogInfo(getPmLogContext(), "LSCALL", 2, PMLOGKS("luna", luna.c_str()), PMLOGJSON("msg", msg.c_str()), " ");
    if(!LSCall(LunaService::instance()->getHandle(), luna.c_str(), msg.c_str(), cbF, udata, nullptr, &lserror)) {
        PmLogError(getPmLogContext(), luna.c_str(), 0, "so sad");
        LSErrorPrint(&lserror, stderr);
    }
}

void LunaService::fLSCall1(std::string luna, std::string msg, fCallBack cbF, void* udata)
{
    // LSCallOneReply(LSHandle * sh, const char * uri, const char * payload, LSFilterFunc callback, void * ctx, LSMessageToken * ret_token, LSError * lserror)
    LSError lserror;
    LSErrorInit(&lserror);
    PmLogInfo(getPmLogContext(), "LSCALL1", 2, PMLOGKS("luna", luna.c_str()), PMLOGJSON("msg", msg.c_str()), " ");
    if(!LSCallOneReply(LunaService::instance()->getHandle(), luna.c_str(), msg.c_str(), cbF, udata, nullptr, &lserror)) {
        PmLogError(getPmLogContext(), luna.c_str(), 0, "so sad");
        LSErrorPrint(&lserror, stderr);
    }
}

void LunaService::fSetTimeOutLSCall(LSMessageToken token, int timeout_ms)
{
    // LSCallSetTimeout (LSHandle * sh, LSMessageToken token, int timeout_ms, LSError * lserror)
    LSError lserror;
    LSErrorInit(&lserror);
    PmLogInfo(getPmLogContext(), "LSCALLTIME", 1, PMLOGKS("timeout_ms", timeout_ms), " ");
    if(!LSCallSetTimeout(LunaService::instance()->getHandle(), token, timeout_ms, &lserror)) {
        PmLogError(getPmLogContext(), "LSCallSetTimeoutErr", 0, "so sad");
        LSErrorPrint(&lserror, stderr);
    }
}

void LunaService::fLSSubscriptionReply(std::string key, std::string msg)
{
    LSError lserror;
    LSErrorInit(&lserror);
    if(!LSSubscriptionGetHandleSubscribersCount(LunaService::instance()->getHandle(), key.c_str())) {
        if(!LSSubscriptionReply(LunaService::instance()->getHandle(), key.c_str(), msg.c_str(), &lserror)) {
            LSErrorPrint(&lserror, stderr);
        }
    }
}

void LunaService::fLSMessageReply(LSHandle *sh, LSMessage * msg, void *ctx, std::string key)
{
}

/* call services */


void LunaService::sendRegisterApp(fCallBack cbF)
{
    LunaService::fLSCall1(
        "luna://com.webos.service.applicationmanager/registerApp",
        "{}",
        cbF,
        nullptr);
}

void LunaService::sendCloseWindow(std::string appName, fCallBack cbF)
{
    std::string sjson = R"({"id":")" + appName + R"("})";
    LunaService::fLSCall1(
        "luna://com.webos.service.applicationmanager/close",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendLaunchHome(fCallBack cbF)
{
    std::string sjson = R"({"id":"com.webos.app.home"})";
    LunaService::fLSCall1(
        "luna://com.webos.service.applicationmanager/launch",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendLaunchApp(std::string appName, fCallBack cbF)
{
    std::string sjson = R"({"id":")" + appName + R"("})";
    LunaService::fLSCall1(
        "luna://com.webos.service.applicationmanager/launch",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMediaPlay(std::string mediaId, fCallBack cbF)
{
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::fLSCall1(
        "luna://com.webos.media/play",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMediaPause(std::string mediaId, fCallBack cbF)
{
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::fLSCall1(
        "luna://com.webos.media/pause",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMediaLoad(std::string appName, std::string uriFile, fCallBack cbF)
{
    std::string sjson = R"({"uri":")" + uriFile +
                        R"(", "type":"media", "callbackpayload":{"option":{"appId":")" + appName +
                        R"(", "windowId":""}}})";
    LunaService::fLSCall1(
        "luna://com.webos.media/load",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMediaUnLoad(std::string mediaId, fCallBack cbF)
{
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::fLSCall1(
        "luna://com.webos.media/unload",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMediaSeek(std::string mediaId, int seek, fCallBack cbF)
{
    std::string sjson = R"({"mediaId":")" + mediaId +
                        R"(","position":)" + std::to_string(seek) + R"(})";
    LunaService::fLSCall1(
        "luna://com.webos.media/seek",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMediaSetPlayRate(std::string mediaId, int playRate, fCallBack cbF)
{
    std::string sjson = R"({"mediaId":")" + mediaId +
                        R"(","playRate":)" + std::to_string(playRate) + R"(,"audioOutput":true})";
    LunaService::fLSCall1(
        "luna://com.webos.media/setPlayRate",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMediaSetVolume(std::string mediaId, int volume, fCallBack cbF)
{
    std::string sjson = R"({"mediaId":")" + mediaId +
                        R"(","volume":)" + std::to_string(volume) + R"(})";
    LunaService::fLSCall1(
        "luna://com.webos.media/setVolume",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMediaSubscribe(std::string mediaId, fCallBack cbF)
{
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::fLSCalln(
        "luna://com.webos.media/subscribe",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMediaUnSubscribe(std::string mediaId, fCallBack cbF)
{
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::fLSCall1(
        "luna://com.webos.media/unsubscribe",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMediaStatus(std::string mediaId, fCallBack cbF)
{
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::fLSCall1(
        "luna://com.webos.media/getPipelineState",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMediaRegisterPipeline(fCallBack cbF)
{
    std::string sjson = R"({"type":"media"})";
    LunaService::fLSCall1(
        "luna://com.webos.media/registerPipeline",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMediaUnRegisterPipeline(std::string mediaPipeId, fCallBack cbF)
{
    std::string sjson = R"({"mediaPipeId":")" + mediaPipeId + R"("})";
    LunaService::fLSCall1(
        "luna://com.webos.media/registerPipeline",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMIndexGetDeviceList(fCallBack cbF)
{
    std::string sjson = R"({"subscribe":true})";
    LunaService::fLSCall1(
        "luna://com.webos.service.mediaindexer/getDeviceList",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMIndexGetAudioList(std::string uriStorage, fCallBack cbF)
{
    std::string sjson = R"({"uri":")"  + uriStorage + R"(/"})";
    LunaService::fLSCall1(
        "luna://com.webos.service.mediaindexer/getAudioList",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMIndexGetAudioMetadata(std::string uriStorage, fCallBack cbF)
{
    std::string sjson = R"({"uri":")" + uriStorage + R"("})";
    LunaService::fLSCall1(
        "luna://com.webos.service.mediaindexer/getAudioMetadata",
        sjson.c_str(),
        cbF,
        nullptr);
}

void LunaService::sendMIndexRqScan(fCallBack cbF)
{
    std::string sjson = R"({"path": "/"})";
    LunaService::fLSCall1(
        "luna://com.webos.service.mediaindexer/requestMediaScan",
        sjson.c_str(),
        cbF,
        nullptr);
}
