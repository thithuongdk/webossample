#include "PlayerService.h"

PlayerService* PlayerService::m_instance = nullptr;

PlayerService* PlayerService::instance(QObject* parent)
{
    if(m_instance==nullptr) {
        m_instance = new PlayerService(parent);
    }
    return m_instance;
}

PlayerService::PlayerService(QObject *parent)
    : QObject(parent)
    , m_serviceHandle(nullptr)
{
    m_appId = std::string("com.xplayer.app.nativeqt");
    m_serviceHandle = acquireHandle();
}

PlayerService::~PlayerService()
{
    clearHandle();
}

LSHandle* PlayerService::acquireHandle()
{
    return nullptr;
}

void PlayerService::clearHandle()
{
}

bool PlayerService::registerAppCallback(LSHandle* sh, LSMessage* msg, void* context)
{
    return true;
}

bool PlayerService::registerApp()
{
    return true;
}