#ifndef APPSERVICES_H
#define APPSERVICES_H

#include <QObject>
#include <QVariant>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <glib.h>
#include <string>
#include "Log.h"
#include "JsonConvert.h"
#include "LunaService.h"
#include "PlayerService.h"


class AppService: public QObject
{
    Q_OBJECT

private:
    static AppService* m_instance;
    explicit AppService(QObject* parent = nullptr);
    virtual ~AppService() override;
    void connectSignalSlots();
    void qmlRegister();

public:
    static AppService* instance(QObject* parent = nullptr);
    void init(std::string appName, GMainLoop *mainLoop);

public:
    void registerApp();

public:
    static bool cbRegisterApp(LSHandle* sh, LSMessage* msg, void* context);

public:
    void createWindow();

// public slots:
    // void onCreateWindow();
// signals :
    // void createWindow();

private:
    std::string m_appName;
    QQmlApplicationEngine *m_engine;
};
#endif
