#ifndef APPSERVICES_H
#define APPSERVICES_H

#include <QObject>
#include <glib.h>
#include <string>
#include <luna-service2/lunaservice.h>
#include "Log.h"
#include "JsonConvert.h"

class AppService: public QObject
{
    Q_OBJECT

private:
    static AppService* m_instance;
    explicit AppService(QObject* parent = nullptr);

public:
    static AppService* instance(QObject* parent = nullptr);
    virtual ~AppService() override;
    LSHandle* getHandle() const { return m_serviceHandle; }
    bool registerApp();
    static bool registerAppCallback(LSHandle* sh, LSMessage* msg, void* context);

signals :
    // Signals to Service
    void createWindow();

protected:
    LSHandle* acquireHandle();
    void clearHandle();

private:
    GMainLoop* m_mainLoop;
    LSHandle* m_serviceHandle;
    std::string m_appId;
};
#endif
