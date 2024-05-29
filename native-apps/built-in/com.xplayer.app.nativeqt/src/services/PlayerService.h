#ifndef PLAYERSERVICES_H
#define PLAYERSERVICES_H

#include <QObject>
#include <glib.h>
#include <string>
#include <luna-service2/lunaservice.h>
#include "Log.h"
#include "JsonConvert.h"

class PlayerService: public QObject
{
    Q_OBJECT

private:
    static PlayerService* m_instance;
    explicit PlayerService(QObject* parent = nullptr);

public:
    static PlayerService* instance(QObject* parent = nullptr);
    virtual ~PlayerService() override;
    LSHandle* getHandle() const { return m_serviceHandle; }
    bool registerApp();
    static bool registerAppCallback(LSHandle* sh, LSMessage* msg, void* context);

signals :
    // Signals to GUI thread
    void createWindow();

protected:
    LSHandle* acquireHandle();
    void clearHandle();

private:
    LSHandle* m_serviceHandle;
    std::string m_appId;
};
#endif
