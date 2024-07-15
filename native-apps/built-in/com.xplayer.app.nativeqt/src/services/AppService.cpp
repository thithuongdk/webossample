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

void AppService::createWindow()
{
    PmLogInfo(getPmLogContext(), "AppService", 0, "onCreateWindow()");
    m_engine->load(QUrl(QStringLiteral("qrc:/src/resources/qmls/Main.qml")));
    PlayerService::instance()->init(m_appName);
}

bool AppService::cbRegisterApp(LSHandle* sh, LSMessage* msg, void* context)
{
    Q_UNUSED(sh);
    PmLogInfo(getPmLogContext(), "REGISTER_CALLBACK", 1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
    pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
    if (!response["returnValue"].asBool()) return false;
    if (response.hasKey("event")) {
        std::string event = response["event"].asString();
        PmLogInfo(getPmLogContext(), "REGISTER_CALLBACK", 1, PMLOGKS("event", event.c_str()), " ");
        if (!strcmp(event.c_str(),"registered")) {
            PmLogInfo(getPmLogContext(), "registered", 0, "ok");
            AppService::instance()->createWindow();
            AppService::instance()->setWindowStatus(1);
        } else if (!strcmp(event.c_str(),"close")) {
            PmLogInfo(getPmLogContext(), "quit", 0, "ok");
            AppService::instance()->setWindowStatus(-1);
            g_main_loop_quit(AppService::instance()->getMainLoop());
        } else if (!strcmp(event.c_str(),"relaunch")) {
            AppService::instance()->setWindowStatus(2);
        } else if (!strcmp(event.c_str(),"pause")) {
        }
    }
    return true;
}

void AppService::registerApp()
{
    LunaService::instance()->fLSCalln("luna://com.webos.service.applicationmanager/registerApp", "{}",
                                    AppService::cbRegisterApp, this);
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

void AppService::minimumWindow() {
}

void AppService::closeWindow() {
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

void AppService::launchHome() {
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

void AppService::launchApp() {
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
            closeWindow();
        } else if(windowStatus==3) {
            launchHome();
        } else if(windowStatus==2 && m_windowStatus==1) {
            // launchApp();
        }
        m_windowStatus = windowStatus;
        emit windowStatusChanged(windowStatus);
    }
}