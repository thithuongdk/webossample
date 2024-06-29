#include "PlayerService.h"
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>

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
{
}

PlayerService::~PlayerService()
{
}

void PlayerService::init(std::string appName)
{
    m_appName = appName;
    connectSignalSlots();
    qmlRegister();

    onUpdatePath("");
}

void PlayerService::connectSignalSlots() {
    // connect(m_service, &AppService::createWindow,                 this, &PlayerService::onCreateWindow);
}

void PlayerService::qmlRegister() {
}

void PlayerService::onMusicLoad(std::string path)
{
    if(path.empty()) {
        path = m_musicPath;
    }
    std::string sjson =
        R"({
            "uri":")" + path + R"(",
            "type":"media",
            "payload":{  
                "option":{  
                    "appId":")" + m_appName + R"(",
                    "windowId":""
                }
            }
        })";

    LunaService::instance()->fLSCall("luna://com.webos.media/load",
                                    sjson.c_str(),
                                    [](LSHandle* sh, LSMessage* msg, void* context)->bool
                                    {return PlayerService::instance()->updateMediaId(msg);},
                                    this);
}

bool PlayerService::updateMediaId(LSMessage* msg)
{
    pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
    if (!response["returnValue"].asBool()
        || !response.hasKey("mediaId")) {
        return false;
    }
    std::string mediaId = response["mediaId"].asString();
    if (m_mediaId != mediaId) {
        m_mediaId = mediaId;
    }
    return true;
}

void PlayerService::onMusicPlay()
{
    std::string sjson = R"({"mediaId":")"+ m_mediaId + R"("})";

    LunaService::instance()->fLSCall("luna://com.webos.media/play",
                                    sjson.c_str(),
                                    [](LSHandle* sh, LSMessage* msg, void* context)->bool {
                                        pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
                                        return response["returnValue"].asBool();
                                    },
                                    this);
}

void PlayerService::onMusicPause()
{
    std::string sjson = R"({"mediaId":")"+ m_mediaId + R"("})";

    LunaService::instance()->fLSCall("luna://com.webos.media/play",
                                    sjson.c_str(),
                                    [](LSHandle* sh, LSMessage* msg, void* context)->bool {
                                        pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
                                        return response["returnValue"].asBool();
                                    },
                                    this);
}

void PlayerService::onUpdatePath(std::string path)
{
    m_musicPath = "storage:///media/multimedia/media/multimedia/music/cam_tu_cau.mp3";
}
