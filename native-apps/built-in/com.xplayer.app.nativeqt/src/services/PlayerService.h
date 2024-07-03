#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H

#include <set>
#include <vector>
#include <QObject>
#include <QVariant>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <uMediaClient.h>
#include "Log.h"
#include "LunaService.h"


class PlayerService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(std::string folderPath READ getFolderPath WRITE setFolderPath NOTIFY folderPathChanged);
    Q_PROPERTY(pbnjson::JValue mediaList READ getMediaList WRITE setMediaList NOTIFY mediaListChanged);
    Q_PROPERTY(int mediaCount READ getMediaCount WRITE setMediaCount NOTIFY mediaCountChanged);
    Q_PROPERTY(int mediaIndex READ getMediaIndex WRITE setMediaIndex NOTIFY mediaIndexChanged);
    Q_PROPERTY(std::string musicPath READ getMusicPath WRITE setMusicPath NOTIFY musicPathChanged);
    Q_PROPERTY(std::string musicStoragePath READ getMusicStoragePath WRITE setMusicStoragePath NOTIFY musicStoragePathChanged);
    Q_PROPERTY(std::string mediaId READ getMediaId WRITE setMediaId NOTIFY mediaIdChanged);
    Q_PROPERTY(pbnjson::JValue mediaStatus READ getMediaStatus WRITE setMediaStatus NOTIFY mediaStatusChanged);
    Q_PROPERTY(pbnjson::JValue mediaData READ getMediaData WRITE setMediaData NOTIFY mediaDataChanged);
    Q_PROPERTY(int playState READ getPlayState WRITE setPlayState NOTIFY playStateChanged);
    Q_PROPERTY(int volume READ getVolume WRITE setVolume NOTIFY volumeChanged);
    Q_PROPERTY(double rate READ getRate WRITE setRate NOTIFY rateChanged);
    Q_PROPERTY(int seek READ getSeek WRITE setSeek NOTIFY seekChanged);
    Q_PROPERTY(int duration READ getDuration WRITE setDuration NOTIFY durationChanged);

private:
    static PlayerService* m_instance;
    explicit PlayerService(QObject* parent = nullptr);
    ~PlayerService();
public:
    static PlayerService* instance(QObject* parent = nullptr);
    void init(std::string appName);

    std::string getFolderPath() const  { return m_folderPath;};
    void setFolderPath(std::string folderPath) {
        if (m_folderPath != folderPath) {
            m_folderPath = folderPath;
            callMIndexGetAudioList(folderPath);   //get list audio
            emit folderPathChanged();
        }
    };

    pbnjson::JValue getMediaList() const  { return m_mediaList;};
    void setMediaList(pbnjson::JValue mediaList) {
        if (m_mediaList != mediaList) {
            m_mediaList = mediaList;
            if(m_mediaIndex>=0 && m_mediaIndex < m_mediaCount
            && mediaList[m_mediaIndex].hasKey("file_path")
            && m_musicPath == mediaList[m_mediaIndex]["file_path"].asString()) {
                //
            } else {
                int idex = 0;
                if(!m_musicPath.empty()) {
                    for(int i=0; i<m_mediaCount; i++) {
                        if(mediaList[i].hasKey("file_path")
                        && m_musicPath == mediaList[i]["file_path"].asString()) {
                            idex = i;
                            break;
                        }
                    }
                }
                setMediaIndex(idex);
            }
            emit mediaListChanged();
        }
    };

    int getMediaCount() const  { return m_mediaCount;};
    void setMediaCount(int mediaCount) {
        if (m_mediaCount != mediaCount) {
            m_mediaCount = mediaCount;
            emit mediaCountChanged();
        }
    };

    int getMediaIndex() const  { return m_mediaIndex;};
    void setMediaIndex(int mediaIndex) {
        if(mediaIndex && m_mediaCount==1 
            && m_mediaList[0].hasKey("file_path")
            && m_musicPath == m_mediaList[0]["file_path"].asString()) {
            mediaIndex=0;
            setSeek(0);
            callMediaPlay(m_mediaId);
        } else {
            if(mediaIndex >= m_mediaCount) {
                mediaIndex = 0;
            } else if(mediaIndex < 0) {
                mediaIndex = m_mediaCount-1;
            }
            if(mediaIndex>=0) {
                if(m_mediaList[mediaIndex].hasKey("file_path")) {
                    std::string musicPath = m_mediaList[mediaIndex]["file_path"].asString();
                    setMusicPath(musicPath);
                }
                if(m_mediaList[mediaIndex].hasKey("uri")) {
                    std::string musicStoragePath = m_mediaList[mediaIndex]["uri"].asString();
                    setMusicStoragePath(musicStoragePath);
                }
                if(m_mediaList[mediaIndex].hasKey("duration")) {
                    int duration = m_mediaList[mediaIndex]["duration"].asNumber<int>();
                    setDuration(duration);
                }
            } else { // no file
                setPlayState(0); // stop
            }
        }
        if (m_mediaIndex != mediaIndex) {
            m_mediaIndex = mediaIndex;
            emit mediaIndexChanged();
        }
    };

    std::string getMusicPath() const  { return m_musicPath;};
    void setMusicPath(std::string musicPath, int countinue=false) {
        if (m_musicPath != musicPath) {
            callMediaUnLoad(m_mediaId);             // unload old media
            callMediaLoad(m_appName, musicPath);    //subscribe new mediaId      
            m_musicPath = musicPath;
            emit musicPathChanged();
        } else {
            callMediaSeek(m_mediaId, 0);            //set time 0
        }
        if(countinue) callMediaPlay(m_mediaId);     //play mediaId
    };

    std::string getMusicStoragePath() const  { return m_musicStoragePath;};
    void setMusicStoragePath(std::string musicStoragePath) {
        if (m_musicStoragePath != musicStoragePath) {
            callMIndexGetAudioMetadata(musicStoragePath);   //get meta data
            m_musicStoragePath = musicStoragePath;
            emit musicStoragePathChanged();
        }
    };

    std::string getMediaId() const  { return m_mediaId;};
    void setMediaId(std::string mediaId) {
        if (m_mediaId != mediaId) {
            m_mediaId = mediaId;
            emit mediaIdChanged();
        }
    };

    pbnjson::JValue getMediaStatus() const { return m_mediaStatus;};
    void setMediaStatus(pbnjson::JValue mediaStatus) {
        if (m_mediaStatus != mediaStatus) {
            m_mediaStatus = mediaStatus;
            emit mediaStatusChanged();
        }
    };

    pbnjson::JValue getMediaData() const { return m_mediaData;};
    void setMediaData(pbnjson::JValue mediaData) {
        if (m_mediaData != mediaData) {
            m_mediaData = mediaData;
            emit mediaDataChanged();
        }
    };

    int getPlayState() const  { return m_playState;};
    void setPlayState(int playState) {
        // playState = 0:stop 1:pause 2:play 
        if (m_playState != playState) {
            if(playState==0) {
                callMediaPause(m_mediaId);        //pause mediaId
                callMediaSeek(m_mediaId, 0);      //set time of mediaId
            } else if(playState==1) {
                callMediaUnSubscribe(m_mediaId);            //subscribe new mediaId
                callMediaPause(m_mediaId);        //pause mediaId
            } else if(playState==2) {
                callMediaSubscribe(m_mediaId);            //subscribe new mediaId
                callMediaPlay(m_mediaId);         //play mediaId
            } else {
                //
            }
            m_playState = playState;
            emit playStateChanged();
        }
    };

    int getVolume() const  { return m_volume;};
    void setVolume(int volume) {
        if (m_volume != volume) {
            callMediaSetVolume(m_mediaId, volume);  //set volume of mediaId
            m_volume = volume;
            emit volumeChanged();
        }
    };

    double getRate() const  { return m_rate;};
    void setRate(double rate) {
        if (m_rate != rate) {
            callMediaSetPlayRate(m_mediaId, rate);  //set rate of mediaId
            m_rate = rate;
            emit rateChanged();
        }
    };

    int getSeek() const  { return m_seek;};
    void setSeek(int seek, bool pypass=false) {
        if (m_seek != seek) {
            if(seek<m_seek || (seek-m_seek)>3000) {
                if(!pypass) callMediaSeek(m_mediaId, seek);     //set seek(s) of mediaId
            }
            m_seek = seek;
            emit seekChanged();
        }
    };

    double getDuration() const  { return m_duration;};
    void setDuration(int duration) {
        if (m_duration != duration) {
            m_duration = duration;
            emit durationChanged();
        }
    };

signals:
    void folderPathChanged();
    void mediaListChanged();
    void mediaCountChanged();
    void mediaIndexChanged();
    void musicPathChanged();
    void musicStoragePathChanged();
    void mediaIdChanged();
    void mediaStatusChanged();
    void mediaDataChanged();
    void playStateChanged();
    void volumeChanged();
    void rateChanged();
    void seekChanged();
    void durationChanged();

private:
    void connectSignalSlots();
    void qmlRegister();

public:
    Q_INVOKABLE void callMediaPlay(std::string mediaId="");
    Q_INVOKABLE void callMediaPause(std::string mediaId="");
    Q_INVOKABLE void callMediaLoad(std::string appName="", std::string uriFile="");
    Q_INVOKABLE void callMediaUnLoad(std::string mediaId="");
    Q_INVOKABLE void callMediaSeek(std::string mediaId="", int seek=0);
    Q_INVOKABLE void callMediaSetPlayRate(std::string mediaId="", double playRate=1.0);
    Q_INVOKABLE void callMediaSetVolume(std::string mediaId="", int volume=50);
    Q_INVOKABLE void callMediaSubscribe(std::string mediaId="");
    Q_INVOKABLE void callMediaUnSubscribe(std::string mediaId="");
    Q_INVOKABLE void callMediaStatus(std::string mediaId="");
    Q_INVOKABLE void callMIndexGetAudioList(std::string uriFile="");
    Q_INVOKABLE void callMIndexGetAudioMetadata(std::string uriStorage="");
    Q_INVOKABLE void callMIndexRqScan();

public:
    std::string m_appName;
    std::string m_folderPath;           // luna-send -n 1 -f luna://com.webos.service.mediaindexer/getAudioList '{"uri":"storage:///media/multimedia/$m_folderPath"}' -> 1:x | 2:m_mediaList+m_mediaCount
    int m_mediaCount;
    pbnjson::JValue m_mediaList;        // m_mediaList[m_mediaIndex]!=m_musicPath => find m_musicPath in m_mediaList => m_mediaIndex | m_mediaIndex=0
    int m_mediaIndex;                   // => m_musicPath
    std::string m_musicPath;            // luna://com.webos.media/unload | luna-send -n 1 -f luna://com.webos.media/load '{"uri":"file:///$m_musicPath","type":"media","payload":{"option":{"appId":"$m_appName","windowId":""}}}' => m_mediaId+m_mediaData
    std::string m_musicStoragePath;
    pbnjson::JValue m_mediaStatus;
    pbnjson::JValue m_mediaData;
    int m_playState;
    std::string m_mediaId;
    double m_rate;
    int m_volume;
    int m_seek;
    int m_duration;
    // uMediaServer::uMediaClient* m_umc;
};
#endif
