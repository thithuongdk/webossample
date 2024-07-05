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
    m_storagePath(""),
    m_folderPath(""),
    m_mediaList(pbnjson::Array()),
    m_mediaCount(0),
    m_mediaIndex(0),
    m_musicPath(""),
    m_musicStorage(""),
    m_mediaStatus(pbnjson::Object()),
    m_mediaData(pbnjson::Object()),
    m_playState(0),
    m_mediaId(""),
    m_volume(90),
    m_rate(1),
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
    callMIndexGetDeviceList();

    connectSignalSlots();
    qmlRegister();
}

void PlayerService::connectSignalSlots() {
    // connect(m_service, &AppService::createWindow,                 this, &PlayerService::callCreateWindow);
}

void PlayerService::qmlRegister() {
}

/* get function */
QString PlayerService::getStoragePath() const
{
    return m_storagePath;
}

QString PlayerService::getFolderPath() const
{
    return m_folderPath;
}

pbnjson::JValue PlayerService::getMediaList() const
{
    return m_mediaList;
}

int PlayerService::getMediaCount() const
{
    return m_mediaCount;
}

int PlayerService::getMediaIndex() const
{
    return m_mediaIndex;
}

QUrl PlayerService::getMusicPath() const
{
    return m_musicPath;
}

QString PlayerService::getMusicStorage() const
{
    return m_musicStorage;
}

QString PlayerService::getMediaId() const
{
    return m_mediaId;
}

pbnjson::JValue PlayerService::getMediaStatus() const
{
    return m_mediaStatus;
}

pbnjson::JValue PlayerService::getMediaData() const
{
    return m_mediaData;
}

int PlayerService::getPlayState() const
{
    return m_playState;
}

int PlayerService::getVolume() const
{
    return m_volume;
}

double PlayerService::getRate() const
{
    return m_rate;
}

int PlayerService::getSeek() const
{
    return m_seek;
}

int PlayerService::getDuration() const
{
    return m_duration;
}

/* set function */
void PlayerService::setStoragePath(QString storagePath) {
    PmLogInfo(getPmLogContext(), "setStoragePath", 1, PMLOGKS("storagePath", storagePath.toStdString().c_str()), " ");
    if (m_storagePath != storagePath) {
        m_storagePath = storagePath;
        emit storagePathChanged(storagePath);
    }
}

void PlayerService::setFolderPath(QString folderPath) {
    PmLogInfo(getPmLogContext(), "setFolderPath", 1, PMLOGKS("folderPath", folderPath.toStdString().c_str()), " ");
    if (m_folderPath != folderPath) {
        m_folderPath = folderPath;
        callMIndexRqScan(); //update and get list audio
        emit folderPathChanged(folderPath);
    }
}

void PlayerService::setMediaList(pbnjson::JValue mediaList) {
    PmLogInfo(getPmLogContext(), "setMediaList", 1, PMLOGKS("mediaList", "mediaList"), " ");
    if (m_mediaList != mediaList) {
        m_mediaList = mediaList;
        if(m_mediaIndex>=0 && m_mediaIndex < m_mediaCount
        && mediaList[m_mediaIndex].hasKey("file_path")
        && m_musicPath.toString().toStdString() == mediaList[m_mediaIndex]["file_path"].asString()) {
            //
        } else {
            int idex = 0;
            if(!m_musicPath.isEmpty()) {
                for(int i=0; i<m_mediaCount; i++) {
                    if(mediaList[i].hasKey("file_path")
                    && m_musicPath.toString().toStdString() == mediaList[i]["file_path"].asString()) {
                        idex = i;
                        break;
                    }
                }
            }
            setMediaIndex(idex);
        }
        emit mediaListChanged(mediaList);
    }
}

void PlayerService::setMediaCount(int mediaCount) {
    PmLogInfo(getPmLogContext(), "setMediaCount", 1, PMLOGKS("mediaCount", std::to_string(mediaCount).c_str()), " ");
    if (m_mediaCount != mediaCount) {
        m_mediaCount = mediaCount;
        emit mediaCountChanged(mediaCount);
    }
}

void PlayerService::setMediaIndex(int mediaIndex) {
    PmLogInfo(getPmLogContext(), "setMediaIndex", 1, PMLOGKS("mediaIndex", std::to_string(mediaIndex).c_str()), " ");
    if(mediaIndex && m_mediaCount==1 
        && m_mediaList[0].hasKey("file_path")
        && m_musicPath.toString().toStdString() == m_mediaList[0]["file_path"].asString()) {
        mediaIndex=0;
        setSeek(0);
        callMediaPlay(m_mediaId.toStdString());
    } else {
        if(mediaIndex >= m_mediaCount) {
            mediaIndex = 0;
        } else if(mediaIndex < 0) {
            mediaIndex = m_mediaCount-1;
        }
        if(mediaIndex>=0 && mediaIndex<m_mediaCount) {
            if(m_mediaList[mediaIndex].hasKey("file_path")) {
                std::string musicPath = m_mediaList[mediaIndex]["file_path"].asString();
                setMusicPath(QUrl(QString::fromStdString(musicPath)));
            }
            if(m_mediaList[mediaIndex].hasKey("uri")) {
                std::string musicStorage = m_mediaList[mediaIndex]["uri"].asString();
                setMusicStorage(QString::fromStdString(musicStorage));
            }
            if(m_mediaList[mediaIndex].hasKey("duration")) {
                int duration = m_mediaList[mediaIndex]["duration"].asNumber<int>();
                setDuration(duration);
            }
        } else { // no file
            mediaIndex = -1;
            setPlayState(0); // stop
            setMusicPath(QUrl(""));
            setDuration(0);
        }
    }
    if (m_mediaIndex != mediaIndex) {
        m_mediaIndex = mediaIndex;
        emit mediaIndexChanged(mediaIndex);
    }
}

void PlayerService::setMusicPath(QUrl musicPath) {
    PmLogInfo(getPmLogContext(), "setMusicPath", 1, PMLOGKS("musicPath", musicPath.toString().toStdString().c_str()), " ");
    if (m_musicPath != musicPath) {
        callMediaUnLoad(m_mediaId.toStdString());             // unload old media
        setMediaId("");
        callMediaLoad(m_appName, musicPath.toString().toStdString());    //subscribe new mediaId
        m_musicPath = musicPath;
        emit musicPathChanged(musicPath);
    } else {
        callMediaSeek(m_mediaId.toStdString(), 0);            //set time 0
    }
}

void PlayerService::setMusicStorage(QString musicStorage) {
    PmLogInfo(getPmLogContext(), "setMusicStorage", 1, PMLOGKS("musicStorage", musicStorage.toStdString().c_str()), " ");
    if (m_musicStorage != musicStorage) {
        callMIndexGetAudioMetadata(musicStorage.toStdString());   //get meta data
        m_musicStorage = musicStorage;
        emit musicStorageChanged(musicStorage);
    }
}

void PlayerService::setMediaId(QString mediaId) {
    PmLogInfo(getPmLogContext(), "setMediaId", 1, PMLOGKS("mediaId", mediaId.toStdString().c_str()), " ");
    if (m_mediaId != mediaId) {
        m_mediaId = mediaId;
        emit mediaIdChanged(mediaId);
    }
};

void PlayerService::setMediaStatus(pbnjson::JValue mediaStatus) {
    PmLogInfo(getPmLogContext(), "setMediaStatus", 1, PMLOGKS("mediaStatus", "mediaStatus"), " ");
    if (m_mediaStatus != mediaStatus) {
        m_mediaStatus = mediaStatus;
        emit mediaStatusChanged(mediaStatus);
    }
}

void PlayerService::setMediaData(pbnjson::JValue mediaData) {
    PmLogInfo(getPmLogContext(), "setMediaData", 1, PMLOGKS("mediaData", "mediaData"), " ");
    if (m_mediaData != mediaData) {
        m_mediaData = mediaData;
        emit mediaDataChanged(mediaData);
    }
}

void PlayerService::setPlayState(int playState) {
    PmLogInfo(getPmLogContext(), "setPlayState", 1, PMLOGKS("playState", std::to_string(playState).c_str()), " ");
    // playState = 0:stop 1:pause 2:play 
    if(m_mediaId.isEmpty()) {
        playState = 0;
        m_playState = playState;
        emit playStateChanged(playState);
    } else if (m_playState != playState) {
        if(playState==0) {
            callMediaPause(m_mediaId.toStdString());        //pause mediaId
            callMediaSeek(m_mediaId.toStdString(), 0);      //set time of mediaId
        } else if(playState==1) {
            callMediaPause(m_mediaId.toStdString());        //pause mediaId
        } else if(playState==2) {
            callMediaPlay(m_mediaId.toStdString());         //play mediaId
        } else {
            //
        }
        m_playState = playState;
        emit playStateChanged(playState);
    }
}

void PlayerService::setVolume(int volume) {
    PmLogInfo(getPmLogContext(), "setVolume", 1, PMLOGKS("volume", std::to_string(volume).c_str()), " ");
    if (m_volume != volume) {
        callMediaSetVolume(m_mediaId.toStdString(), volume);  //set volume of mediaId
        m_volume = volume;
        emit volumeChanged(volume);
    }
}

void PlayerService::setRate(double rate) {
    PmLogInfo(getPmLogContext(), "setRate", 1, PMLOGKS("rate", std::to_string(rate).c_str()), " ");
    if (m_rate != rate) {
        callMediaSetPlayRate(m_mediaId.toStdString(), rate);  //set rate of mediaId
        m_rate = rate;
        emit rateChanged(rate);
    }
}

void PlayerService::setSeek(int seek, bool pypass) {
    if (m_seek != seek) {
        if(seek<m_seek || (seek-m_seek)>3000) {
            PmLogInfo(getPmLogContext(), "setSeek", 1, PMLOGKS("seek", std::to_string(seek).c_str()), " ");
            if(!pypass) callMediaSeek(m_mediaId.toStdString(), seek);     //set seek(s) of mediaId
        }
        m_seek = seek;
        emit seekChanged(seek);
    }
}

void PlayerService::setDuration(int duration) {
    PmLogInfo(getPmLogContext(), "setDuration", 1, PMLOGKS("duration", std::to_string(duration).c_str()), " ");
    if (m_duration != duration) {
        m_duration = duration;
        emit durationChanged(duration);
    }
}

/* call services */
void PlayerService::callMediaPlay(std::string mediaId)
{
    if(mediaId.empty()) return;
    PlayerService::instance()->callMediaSubscribe(mediaId);
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::instance()->fLSCall1(
        "luna://com.webos.media/play",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/play",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
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
    LunaService::instance()->fLSCall1(
        "luna://com.webos.media/pause",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/pause",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
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
                        R"(", "type":"media", "callbackpayload":{"option":{"appId":")" + appName +
                        R"(", "windowId":""}}})";
    LunaService::instance()->fLSCall1(
        "luna://com.webos.media/load",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/load",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            if (response.hasKey("mediaId")) {
                std::string mediaId = response["mediaId"].asString();
                PlayerService::instance()->setMediaId(QString::fromStdString(mediaId));             // update new media
                PlayerService::instance()->callMediaSeek(mediaId,0);        // time 0
                PlayerService::instance()->callMediaSetVolume(mediaId, PlayerService::instance()->getVolume());
                PlayerService::instance()->callMediaSetPlayRate(mediaId, PlayerService::instance()->getRate());
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
    LunaService::instance()->fLSCall1(
        "luna://com.webos.media/unload",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/unload",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            if (response.hasKey("mediaId")
                && PlayerService::instance()->getMediaId().toStdString()==response["mediaId"].asString()) {
                PlayerService::instance()->setMediaId("");             // update new media
            }
            return true;
        },
        this);
}

void PlayerService::callMediaSeek(std::string mediaId, int seek)
{
    if(mediaId.empty()) return;
    std::string sjson = R"({"mediaId":")" + mediaId +
                        R"(","position":)" + std::to_string(seek) + R"(})";
    LunaService::instance()->fLSCall1(
        "luna://com.webos.media/seek",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/seek",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
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
    LunaService::instance()->fLSCall1(
        "luna://com.webos.media/setPlayRate",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/setPlayRate",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
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
    LunaService::instance()->fLSCall1(
        "luna://com.webos.media/setVolume",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/setVolume",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            return true;
        },
        this);
}

void PlayerService::callMediaSubscribe(std::string mediaId)
{
    if(mediaId.empty()) return;
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::instance()->fLSCalln(
        "luna://com.webos.media/subscribe",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/subscribe",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (response.hasKey("returnValue") && !response["returnValue"].asBool()) return false;
            if (response.hasKey("endOfStream") && response["endOfStream"].hasKey("mediaId")) {
                std::string mediaId = response["endOfStream"]["mediaId"].asString();
                if (PlayerService::instance()->getMediaId().toStdString()==mediaId) {
                    PlayerService::instance()->callMediaPause(mediaId);
                    PlayerService::instance()->setSeek(0);
                    if(PlayerService::instance()->getMediaCount()==1) {
                        PlayerService::instance()->callMediaPlay(mediaId);
                    } else {
                        PlayerService::instance()->setMediaIndex(PlayerService::instance()->getMediaIndex()+1);
                    }
                }
            } else if (response.hasKey("currentTime")) {
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
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::instance()->fLSCall1(
        "luna://com.webos.media/unsubscribe",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/unsubscribe",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
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
    LunaService::instance()->fLSCall1(
        "luna://com.webos.media/getPipelineState",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/getPipelineState",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            if (PlayerService::instance()->getMediaId().toStdString()==response["mediaId"].asString()) {
                if (response.hasKey("data")){
                    pbnjson::JValue data = response["data"];
                    PlayerService::instance()->setMediaStatus(data);
                }
            }
            return true;
        },
        this);
}

void PlayerService::callMIndexGetDeviceList()
{
    std::string sjson = R"({"subscribe":true})";
    LunaService::instance()->fLSCall1(
        "luna://com.webos.service.mediaindexer/getDeviceList",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/getDeviceList",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            if (response.hasKey("pluginList") && response["pluginList"].isArray()) {
                pbnjson::JValue pluginList = response["pluginList"];
                std::string uri="";
                for(int ipi=0; ipi<pluginList.arraySize(); ipi++) {
                    if(pluginList[ipi].hasKey("deviceList") && pluginList[ipi]["deviceList"].isArray()) {
                        pbnjson::JValue deviceList = pluginList[ipi]["deviceList"];
                        for(int idv=0; idv<deviceList.arraySize(); idv++) {
                            if(deviceList[idv].hasKey("audioCount")
                                && deviceList[idv]["audioCount"].asNumber<int>()>0
                                && deviceList[idv].hasKey("uri")) {
                                uri = deviceList[idv]["uri"].asString();
                                break;
                            }
                        }
                        if(!uri.empty()) break;
                    }
                }
                PlayerService::instance()->setStoragePath(QString::fromStdString(uri));
                PlayerService::instance()->setFolderPath(QString::fromStdString(uri));
            }
            return true;
        },
        this);
}

void PlayerService::callMIndexGetAudioList(std::string uriFile)
{
    std::string sjson = R"({"uri":")" + uriFile + R"("})";
    LunaService::instance()->fLSCall1(
        "luna://com.webos.service.mediaindexer/getAudioList",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/getAudioList",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
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
    LunaService::instance()->fLSCall1(
        "luna://com.webos.service.mediaindexer/getAudioMetadata",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/getAudioMetadata",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            if (response.hasKey("metadata")) {
                pbnjson::JValue metadata = response["metadata"];
                if (metadata.hasKey("file_path")
                    && PlayerService::instance()->getMusicPath().toString().toStdString()==metadata["file_path"].asString()) {
                    PlayerService::instance()->setMediaData(metadata);
                }
            }
            return true;
        },
        this);
}

void PlayerService::callMIndexRqScan()
{
    std::string sjson = R"({"path": "/"})";
    LunaService::instance()->fLSCall1(
        "luna://com.webos.service.mediaindexer/requestMediaScan",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/requestMediaScan",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (!response["returnValue"].asBool()) return false;
            PlayerService::instance()->callMIndexGetAudioList(PlayerService::instance()->getFolderPath().toStdString());
            return true;
        },
        this);
}