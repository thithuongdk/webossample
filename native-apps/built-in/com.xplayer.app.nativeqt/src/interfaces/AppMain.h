#ifndef APPMAIN_H
#define APPMAIN_H

#include <QObject>
#include <QVariant>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include "AppService.h"
#include "MediaPlayer.h"
#include "Log.h"

class AppMain : public QObject
{
    Q_OBJECT

private:
    static AppMain* m_instance;
    explicit AppMain(QObject *parent = nullptr);

public:
    static AppMain* instance(QObject *parent = nullptr);
    ~AppMain();
    void initApplication();

public slots:
    void onCreateWindow();

private:
    void connectSignalSlots();
    void qmlRegister();

private:
    AppService* m_service;
    QQmlApplicationEngine* m_engine;
};
#endif
