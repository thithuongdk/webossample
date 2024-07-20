
#ifdef MOCK_H
#define MOCK_H

#ifndef JSONCONVERT_H
#define JSONCONVERT_H
#include <string>
#include <iostream>
namespace pbnjson {
    class JValue {
        public:
            std::string value;
            JValue(): value("") {};
            JValue(std::string value_): value(value_) {};
            ~JValue() {value="";};
            bool hasKey(std::string key) {return key!="string";}
            bool asBool() {return true;}
            bool isArray() {return true;}
            int arraySize() {
                if(value.length()==0) return 0;
                int alen = int(value[0]-'0');
                std::cout << "arraysize = " << alen << std::endl;
                return alen;
            }
            std::string asString() {return value;}
            template <typename T>
            T asNumber() {
                int num =  std::stoi(value);
                return num;
            }
            JValue operator[](std::string key) {
                std::string sfind = R"(")" + key + R"(":")";
                int i0 = value.find(sfind);
                if(i0==-1) {
                    return JValue("");
                }
                i0+=sfind.length();
                int i1 = value.find('"',i0);
                std::string sout = value.substr(i0,i1-i0);
                std::cout << key << " = " << sout << std::endl;
                return JValue(sout);
            };
            JValue operator[](int index) {
                std::string svalue = value.substr(2,value.length()-2);
                int i0=0;
                int i1=0;
                value+=",";
                for(int i=0; i<=index; i++) {
                    i0 = i1;
                    i1 = value.find(",",i0);
                    if(i1==-1) {
                        return JValue("");
                    }
                }
                std::string sout = value.substr(i0,i1-i0);
                std::cout << index << " = " << sout << std::endl;
                return JValue(sout);
            };
    };
}  
static pbnjson::JValue convertStringToJson(const char *rawData)
{
    std::string sdata(rawData);
    return pbnjson::JValue(sdata);
}

static std::string convertJsonToString(const pbnjson::JValue jvalue)
{
    return jvalue.value;
}

#endif // JSONCONVERT_H

#ifndef LOG_H
#define LOG_H
#include <string>
#define PmLogInfo(...)
class PmLogContext {
    public:
        std::string value;
        PmLogContext(): value("") {};
        PmLogContext(std::string value_): value(value_) {};
        ~PmLogContext() {value="";};
};

static PmLogContext getPmLogContext()
{
    static PmLogContext s_context("");
    return s_context;
}

#endif // LOG_H

#ifndef GLIB_H
#define GLIB_H

class GMainLoop {
    public:
        std::string value;
        GMainLoop(): value("") {};
        GMainLoop(std::string value_): value(value_) {};
        ~GMainLoop() {value="";};
};
static GMainLoop *g_main_loop_new(void* a1, bool a2) {if(a1!=nullptr || a2) {return nullptr;}; return new GMainLoop("");};
static void g_main_loop_run(GMainLoop *mainLoop) {if(mainLoop!=nullptr) {};};
static void g_main_loop_unref(GMainLoop *mainLoop) {if(mainLoop!=nullptr) mainLoop=nullptr;};
static void g_main_loop_quit(GMainLoop *mainLoop) {if(mainLoop!=nullptr) mainLoop=nullptr;};

#endif // GLIB_H

#ifndef LUNASERVICE_H
#define LUNASERVICE_H

#include <QObject>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>

class LSHandle {
    public:
        std::string value;
        LSHandle(): value("") {};
        LSHandle(std::string value_): value(value_) {};
        ~LSHandle() {value="";};
};

class LSMessage {
    public:
        std::string value;
        LSMessage(): value("") {};
        LSMessage(std::string value_): value(value_) {};
        ~LSMessage() {value="";};
};

class LSMessageToken {
public:
    std::string value;
    LSMessageToken(): value("") {};
    LSMessageToken(std::string value_): value(value_) {};
    ~LSMessageToken() {value="";};
};

class LunaService: public QObject
{
    Q_OBJECT

public:
    typedef bool(*fCallBack)(LSHandle *sh, LSMessage *lsm, void* udata);

private:

    explicit LunaService(QObject* parent = nullptr) : QObject(parent), m_appName(""), m_mainLoop(nullptr), m_handle(nullptr) {};
    virtual ~LunaService() override {};
public:
    static LunaService* instance(QObject* parent = nullptr) {static LunaService* m_instance= new LunaService(parent);return m_instance;};
    LSHandle* getHandle() const { return m_handle; };
    void init(std::string appName, GMainLoop *mainloop) {Q_UNUSED(appName);Q_UNUSED(mainloop);};
    static const char *fLSMessageGetPayload(LSMessage * msg) {return msg->value.c_str();};
    static void fMessagePrintLogCB(LSMessage * msg) {
        std::cout << " fMessagePrintLogCB " << msg->value <<std::endl;
    };
    static void fMessagePrintLogCB(LSHandle* sh, LSMessage* msg, void* context) {
        Q_UNUSED(sh);Q_UNUSED(context);
        std::cout << " fMessagePrintLogCB " << msg->value <<std::endl;
    };
    static void fLSCalln(std::string luna, std::string msg, fCallBack cbF, void* udata = 0) {Q_UNUSED(luna);Q_UNUSED(msg);Q_UNUSED(cbF);Q_UNUSED(udata);};
    static void fLSSubscriptionReply(std::string key, std::string msg) {Q_UNUSED(key);Q_UNUSED(msg);};
    static void fLSMessageReply(LSHandle *sh, LSMessage * msg, void *ctx, std::string key) {Q_UNUSED(sh);Q_UNUSED(msg);Q_UNUSED(ctx);Q_UNUSED(key);};
    static void fLSCall1(std::string luna, std::string msg, fCallBack cbF, void* udata = 0) {Q_UNUSED(luna);Q_UNUSED(msg);Q_UNUSED(cbF);Q_UNUSED(udata);};
    static void fSetTimeOutLSCall(LSMessageToken token, int timeout_ms) {Q_UNUSED(token);Q_UNUSED(timeout_ms);};

public:
    static void sendRegisterApp(fCallBack cbF) {
        cbF(nullptr, new LSMessage(R"("event":"registered")"),nullptr);
        cbF(nullptr, new LSMessage(R"("event":"relaunch")"),nullptr);
    };
    static void sendCloseWindow(std::string appName, fCallBack cbF) {Q_UNUSED(appName);Q_UNUSED(cbF);};
    static void sendLaunchHome(fCallBack cbF) {Q_UNUSED(cbF);};
    static void sendLaunchApp(std::string appName, fCallBack cbF) {Q_UNUSED(appName);Q_UNUSED(cbF);};
    static void sendMediaPlay(std::string mediaId, fCallBack cbF) {Q_UNUSED(mediaId);Q_UNUSED(cbF);};
    static void sendMediaPause(std::string mediaId, fCallBack cbF) {Q_UNUSED(mediaId);Q_UNUSED(cbF);};
    static void sendMediaLoad(std::string appName, std::string uriFile, fCallBack cbF) {
        Q_UNUSED(appName);Q_UNUSED(uriFile);
        cbF(nullptr, new LSMessage(R"("mediaId":"mediaid")"),nullptr);
    };
    static void sendMediaUnLoad(std::string mediaId, fCallBack cbF) {
        Q_UNUSED(mediaId);
        cbF(nullptr, new LSMessage(R"("mediaId":"mediaid")"),nullptr);
    };
    static void sendMediaSeek(std::string mediaId, int seek, fCallBack cbF) {Q_UNUSED(mediaId);Q_UNUSED(seek);Q_UNUSED(cbF);};
    static void sendMediaSetPlayRate(std::string mediaId, int playRate, fCallBack cbF) {Q_UNUSED(mediaId);Q_UNUSED(playRate);Q_UNUSED(cbF);};
    static void sendMediaSetVolume(std::string mediaId, int volume, fCallBack cbF) {Q_UNUSED(mediaId);Q_UNUSED(volume);Q_UNUSED(cbF);};
    static void sendMediaSubscribe(std::string mediaId, fCallBack cbF) {Q_UNUSED(mediaId);Q_UNUSED(cbF);};
    static void sendMediaUnSubscribe(std::string mediaId, fCallBack cbF) {Q_UNUSED(mediaId);Q_UNUSED(cbF);};
    static void sendMediaStatus(std::string mediaId, fCallBack cbF) {Q_UNUSED(mediaId);Q_UNUSED(cbF);};
    static void sendMediaRegisterPipeline(fCallBack cbF) {
        cbF(nullptr, new LSMessage(R"(connectionid)"),nullptr);
    };
    static void sendMediaUnRegisterPipeline(std::string mediaPipeId, fCallBack cbF) {Q_UNUSED(mediaPipeId);Q_UNUSED(cbF);};
    static void sendMIndexGetDeviceList(fCallBack cbF) {
        cbF(nullptr, new LSMessage(R"(0)"),nullptr);
    };
    static void sendMIndexGetAudioList(std::string uriStorage, fCallBack cbF) {
        Q_UNUSED(uriStorage);Q_UNUSED(cbF);
    };
    static void sendMIndexGetAudioMetadata(std::string uriStorage, fCallBack cbF) {Q_UNUSED(uriStorage);Q_UNUSED(cbF);};
    static void sendMIndexRqScan(fCallBack cbF) {
        cbF(nullptr, new LSMessage(R"()"),nullptr);
    };
protected:
    LSHandle* acquireHandle(std::string appName) {return new LSHandle(appName);};
    void clearHandle() {};

private:
    std::string m_appName;
    GMainLoop* m_mainLoop;
    LSHandle* m_handle;
    std::unordered_map<std::string, std::queue<LSMessage*>> subscribe_map;
};
#endif // LUNASERVICE_H

#endif // MOCK_H
