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
    , m_engine(nullptr)
    , m_appName("")
    , m_windowStatus(0)
    , m_mainLoop(nullptr)
{
}

AppService::~AppService()
{
    if (m_engine != nullptr) {
        delete m_engine;
    }
}

void AppService::init(const std::string appName, GMainLoop *mainLoop)
{
    m_appName = appName;
    m_mainLoop = mainLoop;
    setWindowStatus(0);
    LunaService::instance()->init(appName, mainLoop);
    PlayerService::instance();
    m_engine = new QQmlApplicationEngine();

    registerApp();
    connectSignalSlots();
    qmlRegister();
}

void AppService::registerApp()
{
    LunaService::instance()->fLSCall1(
        "luna://com.webos.service.applicationmanager/registerApp",
        "{}",
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/registerApp",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            if (response.hasKey("event")) {
                std::string event = response["event"].asString();
                if (!strcmp(event.c_str(),"registered")) {
                    AppService::instance()->callCreateWindow();
                    AppService::instance()->setWindowStatus(1);
                } else if (!strcmp(event.c_str(),"close")) {
                    AppService::instance()->setWindowStatus(-1);
                    g_main_loop_quit(AppService::instance()->getMainLoop());
                } else if (!strcmp(event.c_str(),"relaunch")) {
                    AppService::instance()->setWindowStatus(2);
                } else if (!strcmp(event.c_str(),"pause")) {
                }
            }
            return true;
        },
        this);
}

void AppService::connectSignalSlots() {
    // connect(this, &AppService::createWindow, this, &AppService::onCreateWindow);
}

void AppService::qmlRegister() {
    qmlRegisterUncreatableType<PlayerService>("app.playerservice", 1, 0, "PlayerService", "");
    m_engine->rootContext()->setContextProperty("playerService", PlayerService::instance());

    qmlRegisterUncreatableType<AppService>("app.appservice", 1, 0, "AppService", "");
    m_engine->rootContext()->setContextProperty("appService", AppService::instance());
}

void AppService::callCreateWindow()
{
    PmLogInfo(getPmLogContext(), "AppService", 0, "callCreateWindow()");
    m_engine->load(QUrl(QStringLiteral("qrc:/src/resources/qmls/Main.qml")));
    PlayerService::instance()->init(m_appName);
}

void AppService::callMinimumWindow() {
}

void AppService::callCloseWindow() {
    std::string sjson = R"({"id":")" + AppService::instance()->getAppName() + R"("})";
    LunaService::instance()->fLSCall1(
        "luna://com.webos.service.applicationmanager/close",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/close",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            PlayerService::instance()->callAppSettings(AppService::instance()->getAppName());
            return true;
        },
        this);
}

void AppService::callLaunchHome() {
    std::string sjson = R"({"id":"com.webos.app.home"})";
    LunaService::instance()->fLSCall1(
        "luna://com.webos.service.applicationmanager/launch",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/launch",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void AppService::callLaunchApp() {
    std::string sjson = R"({"id":")" + AppService::instance()->getAppName() + R"("})";
    LunaService::instance()->fLSCall1(
        "luna://com.webos.service.applicationmanager/launch",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/launch",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void AppService::setWindowStatus(int windowStatus) {
    PmLogInfo(getPmLogContext(), "setWindowStatus", 1, PMLOGKS("windowStatus", std::to_string(windowStatus).c_str()), " ");
    if (m_windowStatus != windowStatus) {
        if(windowStatus==-1) {
            PlayerService::instance()->deInit();
            callCloseWindow();
        } else if(windowStatus==3) {
            callLaunchHome();
        } else if(windowStatus==2) {
            callLaunchApp();
        }
        m_windowStatus = windowStatus;
        emit windowStatusChanged(windowStatus);
    }
}