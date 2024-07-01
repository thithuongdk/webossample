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
    : QObject(parent),
    m_appName(""),
    m_folderPath(""),
    m_mediaCount(1),
    m_mediaList(pbnjson::Array()), 
    m_mediaIndex(0),
    m_musicPath(""),
    m_musicStoragePath(""),
    m_mediaData(pbnjson::Object()),
    m_mediaId(""),
    m_rate(1.0),
    m_volume(50),
    m_seek(0),
    m_duration(0),
    m_umc(nullptr)
{
}

PlayerService::~PlayerService()
{
}

void PlayerService::init(std::string appName)
{
    m_appName = appName;
    setFolderPath("storage:///media/multimedia/media/");
    setRate(1);
    setVolume(50);
    setSeek(0);
    m_umc = new uMediaServer::uMediaClient(false, UMS_CONNECTOR_PRIVATE_BUS);

    connectSignalSlots();
    qmlRegister();
}

void PlayerService::connectSignalSlots() {
    // connect(m_service, &AppService::createWindow,                 this, &PlayerService::callCreateWindow);
}

void PlayerService::qmlRegister() {
}

void PlayerService::callMediaPlay(std::string mediaId)
{
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/play",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "media/play_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaPause(std::string mediaId)
{
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/pause",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "media/pause_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaLoad(std::string appName, std::string uriFile)
{
    std::string sjson = R"({"uri":")" + uriFile +
                        R"(", "type":"media", "payload":{"option":{"appId":")" + appName +
                        R"(", "windowId":""}}})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/load",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "media/load_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            if (response.hasKey("mediaId")) {
                std::string mediaId = response["mediaId"].asString();
                PlayerService::instance()->setMediaId(mediaId);
            }
            return true;
        },
        this);
}

void PlayerService::callMediaUnLoad(std::string mediaId)
{
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/unload",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "media/unload_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaSeek(std::string mediaId, int seek)
{
    std::string sjson = R"({"mediaId":")" + mediaId +
                        R"(","position":)" + std::to_string(seek*1000) + R"(})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/seek",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "media/seek_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaSetPlayRate(std::string mediaId, double playRate)
{
    std::string sjson = R"({"mediaId":")" + mediaId +
                        R"(","playRate":)" + std::to_string(playRate) + R"(,"audioOutput":true})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/setPlayRate",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "media/setPlayRate_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaSetVolume(std::string mediaId, int volume)
{
    std::string sjson = R"({"mediaId":")" + mediaId +
                        R"(","volume":)" + std::to_string(volume) + R"(})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/setVolume",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "media/setVolume_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMIndexGetAudioList(std::string uriFile)
{
    std::string sjson = R"({"uri":")" + uriFile + R"("})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.service.mediaindexer/getAudioList",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "mediaindexer/getAudioList_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            if (response.hasKey("audioList")) {
                if (response["audioList"].hasKey("results") && response["audioList"].hasKey("count")) {
                    int mediaCount = response["audioList"]["count"].asNumber<int>();
                    PlayerService::instance()->setMediaCount(mediaCount);
                    pbnjson::JValue mediaList = response["audioList"]["results"];
                    PlayerService::instance()->setMediaList(mediaList);
                }
            }
            return true;
        },
        this);
}

void PlayerService::callMIndexGetAudioMetadata(std::string uriStorage)
{
    std::string sjson = R"({"uri":")" + uriStorage + R"("})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.service.mediaindexer/getAudioMetadata",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "mediaindexer/getAudioMetadata_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            if (response.hasKey("metadata")) {
                pbnjson::JValue metadata = response["metadata"];
                PlayerService::instance()->setMediaData(metadata);
            }
            return true;
        },
        this);
}
