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
    static bool cbRegisterApp(LSHandle* sh, LSMessage* msg, void* context);

public:
    void createWindow();
    Q_INVOKABLE void minimumWindow();
    Q_INVOKABLE void closeWindow();
    Q_INVOKABLE void launchHome();
    Q_INVOKABLE void launchApp();

// public slots:
    // void onCreateWindow();
// signals :
    // void createWindow();

private:
    std::string m_appName;
    int m_windowStatus;         // 0:   1:register  2:launch  3:hide 4:close
    GMainLoop *m_mainLoop;
    QQmlApplicationEngine *m_engine;
};
#endif
