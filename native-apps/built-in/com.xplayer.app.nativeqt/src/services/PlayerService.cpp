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
    m_mediaCount(0),
    m_mediaList(pbnjson::Array()), 
    m_mediaIndex(0),
    m_musicPath(""),
    m_musicStoragePath(""),
    m_mediaData(pbnjson::Object()),
    m_mediaId(""),
    m_rate(0),
    m_volume(0),
    m_seek(0),
    m_duration(0)
{
}

PlayerService::~PlayerService()
{
}

void PlayerService::init(std::string appName)
{
    m_appName = appName;
    callMIndexRqScan();
    setFolderPath("storage:///media/multimedia");
    setRate(1);
    setVolume(80);
    setSeek(0);

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
    if(mediaId.empty()) return;
    PlayerService::instance()->callMediaSubscribe(mediaId);
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/play",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/play_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaPause(std::string mediaId)
{
    if(mediaId.empty()) return;
    PlayerService::instance()->callMediaUnSubscribe(mediaId);
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/pause",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/pause_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaLoad(std::string appName, std::string uriFile)
{
    if(uriFile.empty()) return;
    std::string sjson = R"({"uri":")" + uriFile +
                        R"(", "type":"media", "payload":{"option":{"appId":")" + appName +
                        R"(", "windowId":""}}})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/load",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/load_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            if (response.hasKey("mediaId")) {
                std::string mediaId = response["mediaId"].asString();
                PlayerService::instance()->setMediaId(mediaId);             // update new media
                PlayerService::instance()->callMediaSeek(mediaId,0);        // time 0
                if(PlayerService::instance()->getPlayState()==2) {
                    PlayerService::instance()->callMediaPlay(mediaId);      // continue play next music
                }
            }
            return true;
        },
        this);
}

void PlayerService::callMediaUnLoad(std::string mediaId)
{
    if(mediaId.empty()) return;
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    PlayerService::instance()->callMediaUnSubscribe(mediaId);
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/unload",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/unload_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaSeek(std::string mediaId, int seek)
{
    if(mediaId.empty()) return;
    std::string sjson = R"({"mediaId":")" + mediaId +
                        R"(","position":)" + std::to_string(seek) + R"(})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/seek",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/seek_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaSetPlayRate(std::string mediaId, double playRate)
{
    if(mediaId.empty()) return;
    std::string sjson = R"({"mediaId":")" + mediaId +
                        R"(","playRate":)" + std::to_string(playRate) + R"(,"audioOutput":true})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/setPlayRate",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/setPlayRate_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaSetVolume(std::string mediaId, int volume)
{
    if(mediaId.empty()) return;
    std::string sjson = R"({"mediaId":")" + mediaId +
                        R"(","volume":)" + std::to_string(volume) + R"(})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/setVolume",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/setVolume_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaSubscribe(std::string mediaId)
{
    if(mediaId.empty()) return;
    std::string sjson = R"({"mediaId":")" + mediaId + R"(})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/subscribe",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/subscribe_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (response.hasKey("returnValue") && !response["returnValue"].asBool()) return false;
            if (response.hasKey("endOfStream") && response["endOfStream"].hasKey("mediaId")) {
                std::string mediaId = response["endOfStream"]["mediaId"].asString();
                PlayerService::instance()->callMediaPause(mediaId);
                if(PlayerService::instance()->getMediaCount()==1) {
                    PlayerService::instance()->setSeek(0);
                    PlayerService::instance()->callMediaPlay(mediaId);
                } else {
                    PlayerService::instance()->setMediaIndex(PlayerService::instance()->getMediaIndex()+1);
                }
            } else if (response.hasKey("currentTime") && response["endOfStream"].hasKey("mediaId")) {
                int currentTime = response["currentTime"].asNumber<int>();
                PlayerService::instance()->setSeek(currentTime, true);    // pypass send data media/seek
            }

            return true;
        },
        this);
}

void PlayerService::callMediaUnSubscribe(std::string mediaId)
{
    if(mediaId.empty()) return;
    std::string sjson = R"({"mediaId":")" + mediaId + R"(})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/subscribe",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/subscribe_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaStatus(std::string mediaId)
{
    if(mediaId.empty()) return;
    std::string sjson = R"({"mediaId":")" + mediaId + R"(})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.media/getPipelineState",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/getPipelineState_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            if (response.hasKey("data")){
                pbnjson::JValue data = response["data"];
                PlayerService::instance()->setMediaStatus(data);
            }
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
            PmLogInfo(getPmLogContext(), "/getAudioList_cb",
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
    if(uriStorage.empty()) return;
    std::string sjson = R"({"uri":")" + uriStorage + R"("})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.service.mediaindexer/getAudioMetadata",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/getAudioMetadata_cb",
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

void PlayerService::callMIndexRqScan()
{
    std::string sjson = R"({"path": "/media"})";
    LunaService::instance()->fLSCall(
        "luna://com.webos.service.mediaindexer/requestMediaScan",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/requestMediaScan_cb",
                        1, PMLOGJSON("payload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}