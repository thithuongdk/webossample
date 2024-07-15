#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H

#include <set>
#include <vector>
#include <QUrl>
#include <QVariant>
#include <QSettings>
#include <QObject>
#include <QVariant>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <uMediaClient.h>
#include <cstdlib>
#include "Log.h"
#include "LunaService.h"

typedef QMap<QString, QVariant> map_v;
typedef QList<QVariant> list_v;


class PlayerService : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString             storagePath         READ getStoragePath         WRITE setStoragePath        NOTIFY storagePathChanged       );
    Q_PROPERTY( QString             folderPath          READ getFolderPath          WRITE setFolderPath         NOTIFY folderPathChanged        );
    Q_PROPERTY( list_v              mediaList           READ getMediaList           WRITE setMediaList          NOTIFY mediaListChanged         );
    Q_PROPERTY( int                 mediaCount          READ getMediaCount          WRITE setMediaCount         NOTIFY mediaCountChanged        );
    Q_PROPERTY( int                 mediaIndex          READ getMediaIndex          WRITE setMediaIndex         NOTIFY mediaIndexChanged        );
    Q_PROPERTY( QUrl                musicPath           READ getMusicPath           WRITE setMusicPath          NOTIFY musicPathChanged         );
    Q_PROPERTY( QString             musicStorage        READ getMusicStorage        WRITE setMusicStorage       NOTIFY musicStorageChanged      );
    Q_PROPERTY( QString             mediaId             READ getMediaId             WRITE setMediaId            NOTIFY mediaIdChanged           );
    Q_PROPERTY( QString             mediaPipeId         READ getMediaPipeId         WRITE setMediaPipeId        NOTIFY mediaPipeIdChanged       );
    Q_PROPERTY( map_v               mediaStatus         READ getMediaStatus         WRITE setMediaStatus        NOTIFY mediaStatusChanged       );
    Q_PROPERTY( map_v               mediaData           READ getMediaData           WRITE setMediaData          NOTIFY mediaDataChanged         );
    Q_PROPERTY( int                 playState           READ getPlayState           WRITE setPlayState          NOTIFY playStateChanged         );
    Q_PROPERTY( int                 shuffleStatus       READ getShuffleStatus       WRITE setShuffleStatus      NOTIFY shuffleStatusChanged     );
    Q_PROPERTY( int                 volume              READ getVolume              WRITE setVolume             NOTIFY volumeChanged            );
    Q_PROPERTY( int                 rate                READ getRate                WRITE setRate               NOTIFY rateChanged              );
    Q_PROPERTY( int                 seek                READ getSeek                WRITE setSeek               NOTIFY seekChanged              );
    Q_PROPERTY( int                 duration            READ getDuration            WRITE setDuration           NOTIFY durationChanged          );
    Q_PROPERTY( map_v               appSettings         READ getAppSettings         WRITE setAppSettings        NOTIFY appSettingsChanged       );

private:
    static PlayerService* m_instance;
    explicit PlayerService(QObject* parent = nullptr);
    ~PlayerService();

public:
    static PlayerService* instance(QObject* parent = nullptr);
    void deInit();
    void init(std::string appName);

public:
    std::string                     getAppName()                    const;
    QString                         getStoragePath()                const;
    QString                         getFolderPath()                 const;
    list_v                          getMediaList()                  const;
    int                             getMediaCount()                 const;
    int                             getMediaIndex()                 const;
    QUrl                            getMusicPath()                  const;
    QString                         getMusicStorage()               const;
    QString                         getMediaId()                    const;
    QString                         getMediaPipeId()                const;
    map_v                           getMediaStatus()                const;
    map_v                           getMediaData()                  const;
    int                             getPlayState()                  const;
    int                             getShuffleStatus()              const;
    int                             getVolume()                     const;
    int                             getRate()                       const;
    int                             getSeek()                       const;
    int                             getDuration()                   const;
    map_v                           getAppSettings()                const;

public:
    void setStoragePath             (QString storagePath)                           ;
    void setFolderPath              (QString folderPath)                            ;
    void setMediaList               (list_v mediaList)                              ;
    void setMediaCount              (int mediaCount)                                ;
    void setMediaIndex              (int mediaIndex)                                ;
    void setMusicPath               (QUrl musicPath)                                ;
    void setMusicStorage            (QString musicStorage)                          ;
    void setMediaId                 (QString mediaId)                               ;
    void setMediaPipeId             (QString mediaPipeId)                           ;
    void setMediaStatus             (map_v mediaStatus)                             ;
    void setMediaData               (map_v mediaData)                               ;
    void setPlayState               (int playState)                                 ;
    void setShuffleStatus           (int shuffleStatus)                             ;
    void setVolume                  (int volume)                                    ;
    void setRate                    (int rate)                                      ;
    void setSeek                    (int seek, bool pypass=false)                   ;
    void setDuration                (int duration)                                  ;
    void setAppSettings             (map_v appSettings)                             ;

signals:
    void storagePathChanged         (QString storagePath = "");
    void folderPathChanged          (QString folderPath = "");
    void mediaListChanged           (list_v = list_v());
    void mediaCountChanged          (int mediaCount = 0);
    void mediaIndexChanged          (int mediaIndex = 0);
    void musicPathChanged           (QUrl musicPath = QUrl(""));
    void musicStorageChanged        (QString musicStorage = "");
    void mediaIdChanged             (QString mediaId = "");
    void mediaPipeIdChanged         (QString mediaPipeId = "");
    void mediaStatusChanged         (map_v mediaStatus = map_v());
    void mediaDataChanged           (map_v mediaData = map_v());
    void playStateChanged           (int playState = 0);
    void shuffleStatusChanged       (int shuffleStatus = 0);
    void volumeChanged              (int volume = 90);
    void rateChanged                (int rate = 10);
    void seekChanged                (int seek = 0);
    void durationChanged            (int duration = 0);
    void appSettingsChanged         (map_v appSettings);

private:
    void connectSignalSlots();
    void qmlRegister();

public:
    Q_INVOKABLE void callMediaPlay                                  (std::string mediaId="")                           ;
    Q_INVOKABLE void callMediaPause                                 (std::string mediaId="")                           ;
    Q_INVOKABLE void callMediaLoad                                  (std::string appName="", std::string uriFile="")   ;
    Q_INVOKABLE void callMediaUnLoad                                (std::string mediaId="")                           ;
    Q_INVOKABLE void callMediaSeek                                  (std::string mediaId="", int seek=0)               ;
    Q_INVOKABLE void callMediaSetPlayRate                           (std::string mediaId="", int playRate=10)          ;
    Q_INVOKABLE void callMediaSetVolume                             (std::string mediaId="", int volume=50)            ;
    Q_INVOKABLE void callEndToNextMediaIndex                        ()                                                 ;
    Q_INVOKABLE void callMediaSubscribe                             (std::string mediaId="")                           ;
    Q_INVOKABLE void callMediaUnSubscribe                           (std::string mediaId="")                           ;
    Q_INVOKABLE void callMediaStatus                                (std::string mediaId="")                           ;
    Q_INVOKABLE void callMediaRegisterPipeline                      ()                                                 ;
    Q_INVOKABLE void callMediaUnRegisterPipeline                    (std::string mediaPipeId="")                       ;
    Q_INVOKABLE void callMIndexGetDeviceList                        ()                                                 ;
    Q_INVOKABLE void callMIndexGetAudioList                         (std::string uriFile="")                           ;
    Q_INVOKABLE void callMIndexGetAudioMetadata                     (std::string uriStorage="")                        ;
    Q_INVOKABLE void callMIndexRqScan                               ()                                                 ;
    Q_INVOKABLE void callAppSettings                                (std::string appName="")                           ;

public:
    std::string                         m_appName               ;       
    QString                             m_storagePath           ;       //      storage:///media/multimedia
    QString                             m_folderPath            ;       //      /meida/xxxx        // luna-send -n 1 -f luna://com.webos.service.mediaindexer/getAudioList '{"uri":"$m_storagePath$m_folderPath"}' -> 1:x | 2:m_mediaList+m_mediaCount
    list_v                              m_mediaList             ;           // m_mediaList[m_mediaIndex]!=m_musicPath => find m_musicPath in m_mediaList => m_mediaIndex | m_mediaIndex=0
    int                                 m_mediaCount            ;
    int                                 m_mediaIndex            ;           // => m_musicPath
    QUrl                                m_musicPath             ;       //      file:///xxxx         // luna://com.webos.media/unload | luna-send -n 1 -f luna://com.webos.media/load '{"uri":"$m_musicPath","type":"media","payload":{"option":{"appId":"$m_appName","windowId":""}}}' => m_mediaId+m_mediaData
    QString                             m_musicStorage          ;
    map_v                               m_mediaStatus           ;
    map_v                               m_mediaData             ;
    int                                 m_playState             ;       // playState = 0:stop 1:pause 2:play
    int                                 m_shuffleStatus         ;       // shuffleStatus = 0:noRepeat 1:shuffle 2:repeatAll 3:repeatOne
    QString                             m_mediaId               ;
    QString                             m_mediaPipeId           ;
    int                                 m_volume                ;
    int                                 m_rate                  ;
    int                                 m_seek                  ;
    int                                 m_duration              ;
    map_v                               m_appSettings           ;
};
#endif
