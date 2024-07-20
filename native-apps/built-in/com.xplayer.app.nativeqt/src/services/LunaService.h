#ifndef LUNASERVICE_H
#define LUNASERVICE_H

#include <QObject>
#include <glib.h>
#include <string>
#include <queue>
#include <unordered_map>
#include <luna-service2/lunaservice.h>
#include "Log.h"
#include "JsonConvert.h"

class LunaService: public QObject
{
    Q_OBJECT

public:
    typedef bool(*fCallBack)(LSHandle *sh, LSMessage *lsm, void* udata);

private:
    static LunaService* m_instance;
    explicit LunaService(QObject* parent = nullptr);
    virtual ~LunaService() override;

public:
    static LunaService* instance(QObject* parent = nullptr);
    LSHandle* getHandle() const { return m_handle; };
    void init(std::string appName, GMainLoop *mainloop);
    static const char *fLSMessageGetPayload(LSMessage * msg);
    static void fMessagePrintLogCB(LSMessage * msg);
    static void fMessagePrintLogCB(LSHandle* sh, LSMessage* msg, void* context);
    static void fLSCalln(std::string luna, std::string msg, fCallBack cbF, void* udata = 0);
    static void fLSSubscriptionReply(std::string key, std::string msg);
    static void fLSMessageReply(LSHandle *sh, LSMessage * msg, void *ctx, std::string key);
    static void fLSCall1(std::string luna, std::string msg, fCallBack cbF, void* udata = 0);
    static void fSetTimeOutLSCall(LSMessageToken token, int timeout_ms);

public:
    static void sendRegisterApp(fCallBack cbF);
    static void sendCloseWindow(std::string appName, fCallBack cbF);
    static void sendLaunchHome(fCallBack cbF);
    static void sendLaunchApp(std::string appName, fCallBack cbF);
    static void sendMediaPlay(std::string mediaId, fCallBack cbF);
    static void sendMediaPause(std::string mediaId, fCallBack cbF);
    static void sendMediaLoad(std::string appName, std::string uriFile, fCallBack cbF);
    static void sendMediaUnLoad(std::string mediaId, fCallBack cbF);
    static void sendMediaSeek(std::string mediaId, int seek, fCallBack cbF);
    static void sendMediaSetPlayRate(std::string mediaId, int playRate, fCallBack cbF);
    static void sendMediaSetVolume(std::string mediaId, int volume, fCallBack cbF);
    static void sendMediaSubscribe(std::string mediaId, fCallBack cbF);
    static void sendMediaUnSubscribe(std::string mediaId, fCallBack cbF);
    static void sendMediaStatus(std::string mediaId, fCallBack cbF);
    static void sendMediaRegisterPipeline(fCallBack cbF);
    static void sendMediaUnRegisterPipeline(std::string mediaPipeId, fCallBack cbF);
    static void sendMIndexGetDeviceList(fCallBack cbF);
    static void sendMIndexGetAudioList(std::string uriStorage, fCallBack cbF);
    static void sendMIndexGetAudioMetadata(std::string uriStorage, fCallBack cbF);
    static void sendMIndexRqScan(fCallBack cbF);
protected:
    LSHandle* acquireHandle(std::string appName);
    void clearHandle();

private:
    std::string m_appName;
    GMainLoop* m_mainLoop;
    LSHandle* m_handle;
    std::unordered_map<std::string, std::queue<LSMessage*>> subscribe_map;
};
#endif
