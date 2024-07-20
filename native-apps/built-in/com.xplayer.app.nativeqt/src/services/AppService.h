#ifndef APPSERVICES_H
#define APPSERVICES_H

#include <QObject>
#include <QVariant>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <iostream>
#include <string>
#include "Log.h"
#include "Glib.h"
#include "JsonConvert.h"
#include "LunaService.h"
#include "PlayerService.h"


class AppService: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int windowStatus READ getWindowStatus WRITE setWindowStatus NOTIFY windowStatusChanged);

private:
    static AppService* m_instance;
    explicit AppService(QObject* parent = nullptr);
    virtual ~AppService() override;
    void connectSignalSlots();
    void qmlRegister();

public:
    static AppService* instance(QObject* parent = nullptr);
    void init(std::string appName, GMainLoop *mainLoop);
    std::string getAppName() const {return m_appName;};
    GMainLoop* getMainLoop() const {return m_mainLoop;};
    int getWindowStatus() const {return m_windowStatus;};
    void setWindowStatus(int windowStatus);

signals:
    void windowStatusChanged         (int windowStatus = 0);

public:
    void registerApp();

public:
    Q_INVOKABLE void callCreateWindow();
    Q_INVOKABLE void callMinimumWindow();
    Q_INVOKABLE void callCloseWindow();
    Q_INVOKABLE void callLaunchHome();
    Q_INVOKABLE void callLaunchApp();

private:
    std::string m_appName;
    int m_windowStatus;         // -1:close   1:register  2:launch  3:hide  4:mini
    GMainLoop *m_mainLoop;
    QQmlApplicationEngine *m_engine;
};
#endif
