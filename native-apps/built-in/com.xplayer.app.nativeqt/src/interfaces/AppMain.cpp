#include "AppMain.h"
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>

AppMain* AppMain::m_instance = nullptr;

AppMain::AppMain(QObject *parent)
    : QObject(parent)
    , m_service(nullptr)
    , m_engine(nullptr)
{
}

AppMain::~AppMain()
{
    if (m_service != nullptr) {
        delete m_service;
    }
    if (m_engine != nullptr) {
        delete m_engine;
    }
}

AppMain *AppMain::instance(QObject* parent)
{
    if(m_instance == nullptr) {
        m_instance = new AppMain(parent);
    }
    return m_instance;
}

void AppMain::initApplication()
{
    m_service = AppService::instance(this);
    m_engine = new QQmlApplicationEngine();
    connectSignalSlots();
    qmlRegister();
}

void AppMain::onCreateWindow()
{
    m_engine->load(QUrl(QStringLiteral("qrc:/src/resources/qmls/Main.qml")));
}

void AppMain::connectSignalSlots() {
    connect(m_service, &AppService::createWindow,                 this, &AppMain::onCreateWindow);
}

void AppMain::qmlRegister() {
    qmlRegisterType<MediaPlayer>("app.mediaplayer", 1, 0, "MediaPlayer");
    m_service->registerApp();
}