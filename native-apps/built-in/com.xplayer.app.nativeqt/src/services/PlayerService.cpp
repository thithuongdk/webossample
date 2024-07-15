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
    m_mediaList(list_v()),
    m_mediaCount(0),
    m_mediaIndex(0),
    m_musicPath(""),
    m_musicStorage(""),
    m_mediaStatus(map_v()),
    m_mediaData(map_v()),
    m_playState(0),
    m_shuffleStatus(0),
    m_mediaId(""),
    m_volume(90),
    m_rate(10),
    m_seek(0),
    m_duration(0),
    m_appSettings(map_v())
{
}

PlayerService::~PlayerService()
{
    PlayerService::instance()->deInit();
}

void PlayerService::deInit()
{
    PlayerService::instance()->callMediaUnSubscribe(PlayerService::instance()->getMediaId().toStdString());  
    PlayerService::instance()->callMediaUnLoad(PlayerService::instance()->getMediaId().toStdString());      
    PlayerService::instance()->callMediaUnRegisterPipeline(PlayerService::instance()->getMediaPipeId().toStdString());
}

void PlayerService::init(std::string appName)
{
    m_appName = appName;
    PlayerService::instance()->callAppSettings();
    PlayerService::instance()->callMIndexGetDeviceList();
    PlayerService::instance()->callMediaRegisterPipeline();

    connectSignalSlots();
    qmlRegister();
}

void PlayerService::connectSignalSlots() {
    // connect(m_service, &AppService::createWindow,                 this, &PlayerService::callCreateWindow);
}

void PlayerService::qmlRegister() {
}

/* get function */
std::string PlayerService::getAppName() const
{
    return m_appName;
}

QString PlayerService::getStoragePath() const
{
    return m_storagePath;
}

QString PlayerService::getFolderPath() const
{
    return m_folderPath;
}

list_v PlayerService::getMediaList() const
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

QString PlayerService::getMediaPipeId() const
{
    return m_mediaPipeId;
}

map_v PlayerService::getMediaStatus() const
{
    return m_mediaStatus;
}

map_v PlayerService::getMediaData() const
{
    return m_mediaData;
}

int PlayerService::getPlayState() const
{
    return m_playState;
}

int PlayerService::getShuffleStatus() const
{
    return m_shuffleStatus;
}

int PlayerService::getVolume() const
{
    return m_volume;
}

int PlayerService::getRate() const
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

map_v PlayerService::getAppSettings() const
{
    return m_appSettings;
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

void PlayerService::setMediaList(list_v mediaList) {
    PmLogInfo(getPmLogContext(), "setMediaList", 1, PMLOGKS("mediaList", "mediaList"), " ");
    if (m_mediaList != mediaList) {
        int idex = 0;
        if(m_mediaIndex>=0 && m_mediaIndex < m_mediaCount && !m_musicPath.isEmpty()) {
            if(m_musicPath.toString() == mediaList[m_mediaIndex].toMap()["file_path"].toString()) {
                idex = m_mediaIndex;
            } else {
                for(int i=0; i<m_mediaCount; i++) {
                    if(m_musicPath.toString() == mediaList[i].toMap()["file_path"].toString()) {
                        idex = i;
                        break;
                    }
                }
            }
        }
        m_mediaList = mediaList;
        setMediaIndex(idex);
        emit mediaListChanged(mediaList);
    }
}

void PlayerService::setMediaCount(int mediaCount) {
    PmLogInfo(getPmLogContext(), "setMediaCount", 1, PMLOGKS("mediaCount", std::to_string(mediaCount).c_str()), " ");
    if (m_mediaCount != mediaCount) {
        m_mediaCount = mediaCount;
        emit mediaCountChanged(mediaCount);
        if(m_mediaCount==0) {
            setPlayState(0); // stop
            setMusicPath(QUrl(""));
            setDuration(0);
        } 
    }
}

void PlayerService::setMediaIndex(int mediaIndex) {
    PmLogInfo(getPmLogContext(), "setMediaIndex", 1, PMLOGKS("mediaIndex", std::to_string(mediaIndex).c_str()), " ");
    if(m_mediaCount==0) {
        m_mediaIndex = -1;
        emit mediaIndexChanged(-1);
        return;
    } else if(m_mediaCount==1 || mediaIndex >= m_mediaCount) {
        mediaIndex=0;
    } else if(mediaIndex==-1) {
        mediaIndex=m_mediaCount-1;
    }
    map_v mapMedia = m_mediaList[mediaIndex].toMap();
    if(m_musicPath.toString() != mapMedia["file_path"]) {
        setMusicPath(mapMedia["file_path"].toUrl());
        setMusicStorage(mapMedia["uri"].toString());
        setDuration(mapMedia["duration"].toInt());
    }
    if (m_mediaIndex != mediaIndex) {
        m_mediaIndex = mediaIndex;
        emit mediaIndexChanged(mediaIndex);
    }
}

void PlayerService::setMusicPath(QUrl musicPath) {
    PmLogInfo(getPmLogContext(), "setMusicPath", 1, PMLOGKS("musicPath", musicPath.toString().toStdString().c_str()), " ");
    if (m_musicPath != musicPath) {
        // unload + load
        callMediaUnLoad(m_mediaId.toStdString());
        setSeek(0);
        callMediaLoad(m_appName, musicPath.toString().toStdString());
        m_musicPath = musicPath;
        emit musicPathChanged(musicPath);
    } else {
        setSeek(0);            //set time 0
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

void PlayerService::setMediaPipeId(QString mediaPipeId) {
    PmLogInfo(getPmLogContext(), "setMediaPipeId", 1, PMLOGKS("mediaId", mediaPipeId.toStdString().c_str()), " ");
    if (m_mediaPipeId != mediaPipeId) {
        m_mediaPipeId = mediaPipeId;
        emit mediaPipeIdChanged(mediaPipeId);
    }
};

void PlayerService::setMediaStatus(map_v mediaStatus) {
    PmLogInfo(getPmLogContext(), "setMediaStatus", 1, PMLOGKS("mediaStatus", "mediaStatus"), " ");
    if (m_mediaStatus != mediaStatus) {
        m_mediaStatus = mediaStatus;
        emit mediaStatusChanged(mediaStatus);
    }
}

void PlayerService::setMediaData(map_v mediaData) {
    PmLogInfo(getPmLogContext(), "setMediaData", 1, PMLOGKS("mediaData", "mediaData"), " ");
    if (m_mediaData != mediaData) {
        m_mediaData = mediaData;
        emit mediaDataChanged(mediaData);
    }
}

void PlayerService::setPlayState(int playState) {
    // playState = 0:stop 1:pause 2:play 3:statusMax
    PmLogInfo(getPmLogContext(), "setPlayState", 1, PMLOGKS("playState", std::to_string(playState).c_str()), " ");
    if (m_playState != playState) {
        if(playState==0) {                                  // when stop: unsubcribe + pause + seek0 + unload
            callMediaPause(m_mediaId.toStdString());
            setSeek(0);
            callMediaUnLoad(m_mediaId.toStdString());
        } else if(playState==1) {                           // when pause: unsubcribe + pause
            callMediaPause(m_mediaId.toStdString());
        } else if(playState==2) {                           // when play:  load + play + subcribe
            callMediaPlay(m_mediaId.toStdString());
            // callAppSettings("player");                      //save rate and volume
        } else {
            //
        }
        callAppSettings("player");
        m_playState = playState;
        emit playStateChanged(playState);
    }
}

void PlayerService::setShuffleStatus(int shuffleStatus) {
    // shuffleStatus = 0:noRepeat 1:shuffle 2:repeatAll 3:repeatOne 4:repeatMax
    PmLogInfo(getPmLogContext(), "setShuffleStatus", 1, PMLOGKS("shuffleStatus", std::to_string(shuffleStatus).c_str()), " ");
    if (m_shuffleStatus != shuffleStatus) {
        if(shuffleStatus>=4) {
            shuffleStatus=0;
        }
        m_shuffleStatus = shuffleStatus;
        emit shuffleStatusChanged(shuffleStatus);
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

void PlayerService::setRate(int rate) {
    PmLogInfo(getPmLogContext(), "setRate", 1, PMLOGKS("rate", std::to_string(rate/10).c_str()), " ");
    if (m_rate != rate) {
        callMediaSetPlayRate(m_mediaId.toStdString(), rate);  //set rate of mediaId
        m_rate = rate;
        emit rateChanged(rate);
    }
}

void PlayerService::setSeek(int seek, bool pypass) {
    if (m_seek != seek) {
        if(seek<m_seek || (seek-m_seek)>3000) {
            if(!pypass) {
                PmLogInfo(getPmLogContext(), "setSeek", 1, PMLOGKS("seek", std::to_string(seek).c_str()), " ");
                callMediaSeek(m_mediaId.toStdString(), seek);     //set seek(s) of mediaId
            }
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

void PlayerService::setAppSettings(map_v appSettings) {
    PmLogInfo(getPmLogContext(), "setAppSettings", 1, PMLOGKS("appSettings", "appSettings"), " ");
    if (m_appSettings != appSettings) {
        m_appSettings = appSettings;
        // PlayerService::instance()->callAppSettings();
        emit appSettingsChanged(appSettings);
    }
}

/* call services */
void PlayerService::callMediaPlay(std::string mediaId)
{
    if(mediaId.empty()) {           // if empty then load before
        PlayerService::instance()->callMediaLoad(m_appName, PlayerService::instance()->getMusicPath().toString().toStdString());
        return;
    } 
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
                PlayerService::instance()->callMediaSeek(mediaId, PlayerService::instance()->getSeek());        // time 0
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
    PlayerService::instance()->setMediaId(""); 
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

void PlayerService::callMediaSetPlayRate(std::string mediaId, int playRate)
{
    if(mediaId.empty()) return;
    std::string sjson = R"({"mediaId":")" + mediaId +
                        R"(","playRate":)" + std::to_string(playRate/10) + R"(,"audioOutput":true})";
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

void PlayerService::callEndToNextMediaIndex() {
    PmLogInfo(getPmLogContext(), "nextMediaIndex", 1, PMLOGKS("mediaCount", std::to_string(m_mediaCount).c_str()), " ");
    callMediaPause(m_mediaId.toStdString());
    setSeek(0);
    if(m_shuffleStatus==0){
        if(m_mediaIndex<m_mediaCount-1) {
            setMediaIndex(m_mediaIndex+1);
        } else {
            setPlayState(1);
        }
    } else if(m_shuffleStatus==1){
        if(m_mediaCount==1) {
            callMediaPlay(m_mediaId.toStdString());
        } else {
            setMediaIndex(m_mediaIndex+1);
        }
    } else if(m_shuffleStatus==2){
        callMediaPlay(m_mediaId.toStdString());
    } else {
        if(m_mediaCount==1) {
            callMediaPlay(m_mediaId.toStdString());
        } else {
            int mediaIndex;
            do {
                mediaIndex = std::rand()%m_mediaCount;
            } while(mediaIndex==m_mediaIndex);
            setMediaIndex(mediaIndex);
        }
    }
}

void PlayerService::callMediaSubscribe(std::string mediaId)
{
    if(mediaId.empty()) return;
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
    LunaService::instance()->fLSCalln(
        "luna://com.webos.media/subscribe",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            if (response.hasKey("returnValue") && !response["returnValue"].asBool()) {
                PmLogInfo(getPmLogContext(), "/subscribe",
                            1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
                return false;
            }
            if (response.hasKey("endOfStream") && response["endOfStream"].hasKey("mediaId")) {
                PmLogInfo(getPmLogContext(), "/subscribe",
                            1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
                std::string mediaId = response["endOfStream"]["mediaId"].asString();
                if (PlayerService::instance()->getMediaId().toStdString()==mediaId) {
                    PlayerService::instance()->callEndToNextMediaIndex();
                }
            } else if (response.hasKey("currentTime")) {
                int currentTime = response["currentTime"].asNumber<int>();
                PlayerService::instance()->setSeek(currentTime, true);    // pypass send data media/seek
            } else {
                PmLogInfo(getPmLogContext(), "/subscribe",
                            1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
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
    std::string sjson = R"({"mediaId":")" + mediaId + R"("})";
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
                    pbnjson::JValue jdata = response["data"];
                    map_v mapData;
                    if (jdata.hasKey("seekPos")) {
                        mapData.insert("seekPos",QString::fromStdString(std::to_string(jdata["seekPos"].asNumber<int>())));
                    }
                    PlayerService::instance()->setMediaStatus(mapData);
                }
            }
            return true;
        },
        this);
}

void PlayerService::callMediaRegisterPipeline()
{
    std::string sjson = R"({"type":"media"})";
    LunaService::instance()->fLSCall1(
        "luna://com.webos.media/registerPipeline",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/registerPipeline",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            // if (!response["returnValue"].asBool()) return false;
            if (response.hasKey("connectionId")) {
                std::string mediaPipeId = response["connectionId"].asString();
                PlayerService::instance()->setMediaPipeId(QString::fromStdString(mediaPipeId));
                if(!PlayerService::instance()->getStoragePath().isEmpty()) {
                    PlayerService::instance()->setFolderPath("/media/multimedia");
                }
            }
            return true;
        },
        this);
}

void PlayerService::callMediaUnRegisterPipeline(std::string mediaPipeId)
{
    if(mediaPipeId.empty()) return;
    std::string sjson = R"({"mediaPipeId":")" + mediaPipeId + R"("})";
    LunaService::instance()->fLSCall1(
        "luna://com.webos.media/registerPipeline",
        sjson.c_str(),
        [](LSHandle* sh, LSMessage* msg, void* context)->bool {
            PmLogInfo(getPmLogContext(), "/getPipelineState",
                        1, PMLOGJSON("callbackpayload", LSMessageGetPayload(msg)), " ");
            pbnjson::JValue response = convertStringToJson(LSMessageGetPayload(msg));
            // if (!response["returnValue"].asBool()) return false;
            if (response.hasKey("mediaPipeId")) {
                std::string mediaPipeId = response["mediaPipeId"].asString();
                PlayerService::instance()->setMediaPipeId(QString::fromStdString(mediaPipeId));
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
                std::string uriStorage="";
                for(int ipi=0; ipi<pluginList.arraySize(); ipi++) {
                    if(pluginList[ipi].hasKey("deviceList") && pluginList[ipi]["deviceList"].isArray()) {
                        pbnjson::JValue deviceList = pluginList[ipi]["deviceList"];
                        for(int idv=0; idv<deviceList.arraySize(); idv++) {
                            if(deviceList[idv].hasKey("audioCount")
                                && deviceList[idv]["audioCount"].asNumber<int>()>0
                                && deviceList[idv].hasKey("uri")) {
                                uriStorage = deviceList[idv]["uri"].asString();
                                break;
                            }
                        }
                        if(!uriStorage.empty()) break;
                    }
                }
                PlayerService::instance()->setStoragePath(QString::fromStdString(uriStorage));
                if(!PlayerService::instance()->getMediaPipeId().isEmpty()) {
                    PlayerService::instance()->setFolderPath("/media/multimedia");
                }
            }
            return true;
        },
        this);
}

void PlayerService::callMIndexGetAudioList(std::string uriStorage)
{
    std::string sjson = R"({"uri":")"  + uriStorage + R"(/"})";
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
                    pbnjson::JValue jmediaList = response["audioList"]["results"];
                    list_v mediaList;
                    for(int idx=0; idx<jmediaList.arraySize(); idx++) {
                        pbnjson::JValue jmetadata = jmediaList[idx];
                        if (jmetadata.hasKey("file_path")) {
                            map_v mapMetaData;
                            mapMetaData.insert("file_path",QString::fromStdString(jmetadata["file_path"].asString()));
                            mapMetaData.insert("uri",QString::fromStdString(jmetadata["uri"].asString()));
                            mapMetaData.insert("duration",QString::fromStdString(std::to_string(jmetadata["duration"].asNumber<int>())));
                            mapMetaData.insert("title",QString::fromStdString(jmetadata["title"].asString()));
                            mapMetaData.insert("artist",QString::fromStdString(jmetadata["artist"].asString()));
                            mapMetaData.insert("album",QString::fromStdString(jmetadata["album"].asString()));
                            mediaList.append(mapMetaData);
                        }
                    }
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
                    map_v mapMetaData;
                    mapMetaData.insert("duration",QString::fromStdString(std::to_string(metadata["duration"].asNumber<int>())));
                    mapMetaData.insert("file_path",QString::fromStdString(metadata["file_path"].asString()));
                    mapMetaData.insert("uri",QString::fromStdString(metadata["uri"].asString()));
                    mapMetaData.insert("title",QString::fromStdString(metadata["title"].asString()));
                    mapMetaData.insert("artist",QString::fromStdString(metadata["artist"].asString()));
                    mapMetaData.insert("album",QString::fromStdString(metadata["album"].asString()));
                    mapMetaData.insert("album_artist",QString::fromStdString(metadata["album_artist"].asString()));
                    PlayerService::instance()->setMediaData(mapMetaData);
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
            std::string uristorage = PlayerService::instance()->getStoragePath().toStdString()
                                    + PlayerService::instance()->getFolderPath().toStdString();
            PlayerService::instance()->callMIndexGetAudioList(uristorage);
            return true;
        },
        this);
}

void PlayerService::callAppSettings(std::string appName)
{
    PmLogInfo(getPmLogContext(), "callAppSettings", 1, PMLOGKS("status", appName.c_str()), " ");
    QSettings qappSettings(QString::fromStdString("My Company"), QString::fromStdString(m_appName));
    if(appName.empty()) {
        // qappSettings.sync();
        map_v appSettings;
        appSettings.insert("theme/boderColor",qappSettings.value("theme/boderColor", "#D9D9D9"));
        appSettings.insert("theme/backGrColor",qappSettings.value("theme/backGrColor", "#F4F4F4"));
        appSettings.insert("theme/textColor",qappSettings.value("theme/textColor", "#333333"));
        appSettings.insert("theme/text2Color",qappSettings.value("theme/text2Color", "#1DB954"));
        appSettings.insert("theme/iconColor",qappSettings.value("theme/iconColor", "#1DB954"));
        PlayerService::instance()->setAppSettings(appSettings);
        // PmLogInfo(getPmLogContext(), "setVolumeTest", 1, PMLOGKS("volume", qappSettings.value("player/volume", 90).toInt()), " ");
        // PmLogInfo(getPmLogContext(), "setRateTest", 1, PMLOGKS("rate", qappSettings.value("player/rate", 10).toInt()), " ");
        // PlayerService::instance()->setVolume(qappSettings.value("player/volume", 90).toInt());
        // PlayerService::instance()->setRate(qappSettings.value("player/rate", 10).toInt());
    // } else if(appName == PlayerService::instance()->getAppName()) {
    //     qappSettings.setValue("theme/boderColor", PlayerService::instance()->getAppSettings()["theme/boderColor"]);
    //     qappSettings.setValue("theme/backGrColor", PlayerService::instance()->getAppSettings()["theme/backGrColor"]);
    //     qappSettings.setValue("theme/textColor", PlayerService::instance()->getAppSettings()["theme/textColor"]);
    //     qappSettings.setValue("theme/text2Color", PlayerService::instance()->getAppSettings()["theme/text2Color"]);
    //     qappSettings.setValue("theme/iconColor", PlayerService::instance()->getAppSettings()["theme/iconColor"]);
    //     qappSettings.setValue("player/volume", PlayerService::instance()->getAppSettings()["player/volume"]);
    //     qappSettings.setValue("player/rate", PlayerService::instance()->getAppSettings()["player/rate"]);
    //     qappSettings.sync();
    } else if(appName == "theme") {
        qappSettings.setValue("theme/boderColor", PlayerService::instance()->getAppSettings()["theme/boderColor"]);
        qappSettings.setValue("theme/backGrColor", PlayerService::instance()->getAppSettings()["theme/backGrColor"]);
        qappSettings.setValue("theme/textColor", PlayerService::instance()->getAppSettings()["theme/textColor"]);
        qappSettings.setValue("theme/text2Color", PlayerService::instance()->getAppSettings()["theme/text2Color"]);
        qappSettings.setValue("theme/iconColor", PlayerService::instance()->getAppSettings()["theme/iconColor"]);
        qappSettings.sync();
    } else if(appName == "player") {
        qappSettings.setValue("player/volume", PlayerService::instance()->getAppSettings()["player/volume"]);
        qappSettings.setValue("player/rate", PlayerService::instance()->getAppSettings()["player/rate"]);
        qappSettings.sync();
    }
}