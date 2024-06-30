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
            PmLogInfo(getPmLogContext(), "registered", 0, PMLOGKS("createWindow", (AppService::instance())), " ");
            // PmLogInfo(getPmLogContext(), "registered", 1, PMLOGKS("createWindow", &(AppService::instance()->createWindow)), " ");
            AppService::instance()->createWindow();
        } else if (!strcmp(event.c_str(),"relaunch")
                || !strcmp(event.c_str(),"pause")
                || !strcmp(event.c_str(),"close")) {
        }
    }
    return true;
}

void AppService::registerApp()
{
    LunaService::instance()->fLSCall("luna://com.webos.service.applicationmanager/registerApp", "{}",
                                    AppService::cbRegisterApp, this);
}

void AppService::connectSignalSlots() {
    // connect(this, &AppService::createWindow, this, &AppService::onCreateWindow);
}

void AppService::qmlRegister() {
    qmlRegisterUncreatableType<PlayerService>("app.playerservice", 1, 0, "PlayerService", "");
    m_engine->rootContext()->setContextProperty("playerService", PlayerService::instance());

}