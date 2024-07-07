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
    m_mediaList(QList<QVariant>()),
    m_mediaCount(0),
    m_mediaIndex(0),
    m_musicPath(""),
    m_musicStorage(""),
    m_mediaStatus(QMap<QString, QVariant>()),
    m_mediaData(QMap<QString, QVariant>()),
    m_playState(0),
    m_mediaId(""),
    m_volume(90),
    m_rate(10),
    m_seek(0),
    m_duration(0),
    m_appSettings(QMap<QString, QVariant>())
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
    PlayerService::instance()->callAppSettings(appName);
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
QString PlayerService::getStoragePath() const
{
    return m_storagePath;
}

QString PlayerService::getFolderPath() const
{
    return m_folderPath;
}

QList<QVariant> PlayerService::getMediaList() const
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

QMap<QString, QVariant> PlayerService::getMediaStatus() const
{
    return m_mediaStatus;
}

QMap<QString, QVariant> PlayerService::getMediaData() const
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

QMap<QString, QVariant> PlayerService::getAppSettings() const
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

void PlayerService::setMediaList(QList<QVariant> mediaList) {
    PmLogInfo(getPmLogContext(), "setMediaList", 1, PMLOGKS("mediaList", "mediaList"), " ");
    if (m_mediaList != mediaList) {
        m_mediaList = mediaList;
        if(m_mediaIndex>=0 && m_mediaIndex < m_mediaCount
        && mediaList[m_mediaIndex].toMap().find("file_path")!=mediaList[m_mediaIndex].toMap().end()
        && m_musicPath.toString() == mediaList[m_mediaIndex].toMap()["file_path"].toString() ) {
            //
        } else {
            int idex = 0;
            if(!m_musicPath.isEmpty()) {
                for(int i=0; i<m_mediaCount; i++) {
                    if(mediaList[i].toMap().find("file_path")!=mediaList[i].toMap().end()
                    && m_musicPath.toString() == mediaList[i].toMap()["file_path"].toString()) {
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
        && m_mediaList[0].toMap().find("file_path")!=m_mediaList[0].toMap().end()
        && m_musicPath.toString() == m_mediaList[0].toMap()["file_path"]) {
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
            QMap<QString, QVariant> mapMedia = m_mediaList[mediaIndex].toMap();
            if(mapMedia.find("file_path")!=mapMedia.end()) {
                setMusicPath(mapMedia["file_path"].toUrl());
            }
            if(mapMedia.find("uri")!=mapMedia.end()) {
                setMusicStorage(mapMedia["uri"].toString());
            }
            if(mapMedia.find("duration")!=mapMedia.end()) {
                setDuration(mapMedia["duration"].toInt());
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

void PlayerService::setMediaPipeId(QString mediaPipeId) {
    PmLogInfo(getPmLogContext(), "setMediaPipeId", 1, PMLOGKS("mediaId", mediaPipeId.toStdString().c_str()), " ");
    if (m_mediaPipeId != mediaPipeId) {
        m_mediaPipeId = mediaPipeId;
        emit mediaPipeIdChanged(mediaPipeId);
    }
};

void PlayerService::setMediaStatus(QMap<QString, QVariant> mediaStatus) {
    PmLogInfo(getPmLogContext(), "setMediaStatus", 1, PMLOGKS("mediaStatus", "mediaStatus"), " ");
    if (m_mediaStatus != mediaStatus) {
        m_mediaStatus = mediaStatus;
        emit mediaStatusChanged(mediaStatus);
    }
}

void PlayerService::setMediaData(QMap<QString, QVariant> mediaData) {
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
            callAppSettings("player");                      //save rate and volume
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

void PlayerService::setAppSettings(QMap<QString, QVariant> appSettings) {
    PmLogInfo(getPmLogContext(), "setAppSettings", 1, PMLOGKS("appSettings", "appSettings"), " ");
    if (m_appSettings != appSettings) {
        m_appSettings = appSettings;
        PlayerService::instance()->callAppSettings();
        emit appSettingsChanged(appSettings);
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
                        int newIndex = PlayerService::instance()->getMediaIndex()+1;
                        PlayerService::instance()->setMediaIndex(newIndex);
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
                    QMap<QString, QVariant> mapData;
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
            PmLogInfo(getPmLogContext(), "/getPipelineState",
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
                    QList<QVariant> mediaList;
                    for(int idx=0; idx<jmediaList.arraySize(); idx++) {
                        pbnjson::JValue jmetadata = jmediaList[idx];
                        QMap<QString, QVariant> mapMetaData;
                        // if (jmetadata.hasKey("file_path")) {
                            mapMetaData.insert("file_path",QString::fromStdString(jmetadata["file_path"].asString()));
                        // }
                        // if (jmetadata.hasKey("uri")) {
                            mapMetaData.insert("uri",QString::fromStdString(jmetadata["uri"].asString()));
                        // }
                        // if (jmetadata.hasKey("duration")) {
                            mapMetaData.insert("duration",QString::fromStdString(std::to_string(jmetadata["duration"].asNumber<int>())));
                        // }
                        // if (jmetadata.hasKey("title")) {
                            mapMetaData.insert("title",QString::fromStdString(jmetadata["title"].asString()));
                        // }
                        // if (jmetadata.hasKey("artist")) {
                            mapMetaData.insert("artist",QString::fromStdString(jmetadata["artist"].asString()));
                        // }
                        // if (jmetadata.hasKey("album")) {
                            mapMetaData.insert("album",QString::fromStdString(jmetadata["album"].asString()));
                        // }
                        mediaList.append(mapMetaData);
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
                    QMap<QString, QVariant> mapMetaData;
                    // if (metadata.hasKey("duration")) {
                        mapMetaData.insert("duration",QString::fromStdString(std::to_string(metadata["duration"].asNumber<int>())));
                    // }
                    // if (metadata.hasKey("file_path")) {
                        mapMetaData.insert("file_path",QString::fromStdString(metadata["file_path"].asString()));
                    // }
                    // if (metadata.hasKey("uri")) {
                        mapMetaData.insert("uri",QString::fromStdString(metadata["uri"].asString()));
                    // }
                    // if (metadata.hasKey("title")) {
                        mapMetaData.insert("title",QString::fromStdString(metadata["title"].asString()));
                    // }
                    // if (metadata.hasKey("artist")) {
                        mapMetaData.insert("artist",QString::fromStdString(metadata["artist"].asString()));
                    // }
                    // if (metadata.hasKey("album")) {
                        mapMetaData.insert("album",QString::fromStdString(metadata["album"].asString()));
                    // }
                    // if (metadata.hasKey("album_artist")) {
                        mapMetaData.insert("album_artist",QString::fromStdString(metadata["album_artist"].asString()));
                    // }
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
    static QSettings qappSettings(QString::fromStdString("My Company"), QString::fromStdString(appName));
    if(appName.empty()) {
        QMap<QString, QVariant> appSettings;
        appSettings.insert("theme/boderColor",qappSettings.value("theme/boderColor", "#D9D9D9"));
        appSettings.insert("theme/backGrColor",qappSettings.value("theme/backGrColor", "#F4F4F4"));
        appSettings.insert("theme/textColor",qappSettings.value("theme/textColor", "#333333"));
        appSettings.insert("theme/text2Color",qappSettings.value("theme/text2Color", "#1DB954"));
        appSettings.insert("theme/iconColor",qappSettings.value("theme/iconColor", "#1DB954"));
        PlayerService::instance()->setAppSettings(appSettings);
        PlayerService::instance()->setVolume(qappSettings.value("player/volume", 90).toInt());
        PlayerService::instance()->setRate(qappSettings.value("player/volume", 10).toInt());
    } else if(appName == "theme") {
        qappSettings.setValue("theme/boderColor", PlayerService::instance()->getAppSettings()["theme/boderColor"]);
        qappSettings.setValue("theme/backGrColor", PlayerService::instance()->getAppSettings()["theme/backGrColor"]);
        qappSettings.setValue("theme/textColor", PlayerService::instance()->getAppSettings()["theme/textColor"]);
        qappSettings.setValue("theme/text2Color", PlayerService::instance()->getAppSettings()["theme/text2Color"]);
        qappSettings.setValue("theme/iconColor", PlayerService::instance()->getAppSettings()["theme/iconColor"]);
    } else if(appName == "player") {
        qappSettings.setValue("player/volume", PlayerService::instance()->getAppSettings()["player/volume"]);
        qappSettings.setValue("player/rate", PlayerService::instance()->getAppSettings()["player/rate"]);
    }
}