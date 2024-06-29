#ifndef LUNASERVICE_H
#define LUNASERVICE_H

#include <QObject>
#include <glib.h>
#include <string>
#include <queue>
#include <unordered_map>
#include <luna-service2/lunaservice.h>
#include "Log.h"
#include "JsonConvert.h"

class LunaService: public QObject
{
    Q_OBJECT

private:
    static LunaService* m_instance;
    explicit LunaService(QObject* parent = nullptr);
    virtual ~LunaService() override;

public:
    static LunaService* instance(QObject* parent = nullptr);
    LSHandle* getHandle() const { return m_handle; };
    void init(std::string appName, GMainLoop *mainloop);
    void fLSCall(std::string luna, std::string msg, bool(*cbF)(LSHandle *sh, LSMessage *lsm, void* udata), void* udata = 0);
    void fLSSubscriptionReply(std::string key, std::string msg);
    void fLSMessageReply(LSHandle *sh, LSMessage * msg, void *ctx, std::string key);

protected:
    LSHandle* acquireHandle();
    void clearHandle();

private:
    std::string m_appName;
    GMainLoop* m_mainLoop;
    LSHandle* m_handle;
    std::unordered_map<std::string, std::queue<LSMessage*>> subscribe_map;
};
#endif
