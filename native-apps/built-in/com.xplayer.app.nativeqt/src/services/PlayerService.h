#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H

#include <set>
#include <vector>
#include <QString>
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
    Q_PROPERTY( QString              storagePath        READ getStoragePath             WRITE setStoragePath               NOTIFY storagePathChanged           );
    Q_PROPERTY( QString              folderPath         READ getFolderPath              WRITE setFolderPath                NOTIFY folderPathChanged            );
    Q_PROPERTY( pbnjson::JValue      mediaList          READ getMediaList               WRITE setMediaList                 NOTIFY mediaListChanged             );
    Q_PROPERTY( int                  mediaCount         READ getMediaCount              WRITE setMediaCount                NOTIFY mediaCountChanged            );
    Q_PROPERTY( int                  mediaIndex         READ getMediaIndex              WRITE setMediaIndex                NOTIFY mediaIndexChanged            );
    Q_PROPERTY( QString              musicPath          READ getMusicPath               WRITE setMusicPath                 NOTIFY musicPathChanged             );
    Q_PROPERTY( QString              musicStorage       READ getMusicStorage            WRITE setMusicStorage              NOTIFY musicStorageChanged          );
    Q_PROPERTY( QString              mediaId            READ getMediaId                 WRITE setMediaId                   NOTIFY mediaIdChanged               );
    Q_PROPERTY( pbnjson::JValue      mediaStatus        READ getMediaStatus             WRITE setMediaStatus               NOTIFY mediaStatusChanged           );
    Q_PROPERTY( pbnjson::JValue      mediaData          READ getMediaData               WRITE setMediaData                 NOTIFY mediaDataChanged             );
    Q_PROPERTY( int                  playState          READ getPlayState               WRITE setPlayState                 NOTIFY playStateChanged             );
    Q_PROPERTY( int                  volume             READ getVolume                  WRITE setVolume                    NOTIFY volumeChanged                );
    Q_PROPERTY( double               rate               READ getRate                    WRITE setRate                      NOTIFY rateChanged                  );
    Q_PROPERTY( int                  seek               READ getSeek                    WRITE setSeek                      NOTIFY seekChanged                  );
    Q_PROPERTY( int                  duration           READ getDuration                WRITE setDuration                  NOTIFY durationChanged              );

private:
    static PlayerService* m_instance;
    explicit PlayerService(QObject* parent = nullptr);
    ~PlayerService();

public:
    static PlayerService* instance(QObject* parent = nullptr);
    void init(std::string appName);

public:
    QString                         getStoragePath()                const;
    QString                         getFolderPath()                 const;
    pbnjson::JValue                 getMediaList()                  const;
    int                             getMediaCount()                 const;
    int                             getMediaIndex()                 const;
    QString                         getMusicPath()                  const;
    QString                         getMusicStorage()               const;
    QString                         getMediaId()                    const;
    pbnjson::JValue                 getMediaStatus()                const;
    pbnjson::JValue                 getMediaData()                  const;
    int                             getPlayState()                  const;
    int                             getVolume()                     const;
    double                          getRate()                       const;
    int                             getSeek()                       const;
    int                             getDuration()                   const;

public:
    void setStoragePath             (QString storagePath)           ;
    void setFolderPath              (QString folderPath)            ;
    void setMediaList               (pbnjson::JValue mediaList)     ;
    void setMediaCount              (int mediaCount)                ;
    void setMediaIndex              (int mediaIndex)                ;
    void setMusicPath               (QString musicPath)             ;
    void setMusicStorage            (QString musicStorage)          ;
    void setMediaId                 (QString mediaId)               ;
    void setMediaStatus             (pbnjson::JValue)               ;
    void setMediaData               (pbnjson::JValue)               ;
    void setPlayState               (int playState)                 ;
    void setVolume                  (int volume)                    ;
    void setRate                    (double rate)                   ;
    void setSeek                    (int seek, bool pypass=false)   ;
    void setDuration                (int duration)                  ;

signals:
    void storagePathChanged         ();
    void folderPathChanged          ();
    void mediaListChanged           ();
    void mediaCountChanged          ();
    void mediaIndexChanged          ();
    void musicPathChanged           ();
    void musicStorageChanged        ();
    void mediaIdChanged             ();
    void mediaStatusChanged         ();
    void mediaDataChanged           ();
    void playStateChanged           ();
    void volumeChanged              ();
    void rateChanged                ();
    void seekChanged                ();
    void durationChanged            ();

private:
    void connectSignalSlots();
    void qmlRegister();

public:
    Q_INVOKABLE void callMediaPlay                                  (std::string mediaId="")                           ;
    Q_INVOKABLE void callMediaPause                                 (std::string mediaId="")                           ;
    Q_INVOKABLE void callMediaLoad                                  (std::string appName="", std::string uriFile="")   ;
    Q_INVOKABLE void callMediaUnLoad                                (std::string mediaId="")                           ;
    Q_INVOKABLE void callMediaSeek                                  (std::string mediaId="", int seek=0)               ;
    Q_INVOKABLE void callMediaSetPlayRate                           (std::string mediaId="", double playRate=1.0)      ;
    Q_INVOKABLE void callMediaSetVolume                             (std::string mediaId="", int volume=50)            ;
    Q_INVOKABLE void callMediaSubscribe                             (std::string mediaId="")                           ;
    Q_INVOKABLE void callMediaUnSubscribe                           (std::string mediaId="")                           ;
    Q_INVOKABLE void callMediaStatus                                (std::string mediaId="")                           ;
    Q_INVOKABLE void callMIndexGetDeviceList                        ()                                                 ;
    Q_INVOKABLE void callMIndexGetAudioList                         (std::string uriFile="")                           ;
    Q_INVOKABLE void callMIndexGetAudioMetadata                     (std::string uriStorage="")                        ;
    Q_INVOKABLE void callMIndexRqScan                               ()                                                 ;

public:
    std::string         m_appName               ;
    QString             m_storagePath           ;
    QString             m_folderPath            ;           // luna-send -n 1 -f luna://com.webos.service.mediaindexer/getAudioList '{"uri":"storage:///media/multimedia/$m_folderPath"}' -> 1:x | 2:m_mediaList+m_mediaCount
    int                 m_mediaCount            ;
    pbnjson::JValue     m_mediaList             ;           // m_mediaList[m_mediaIndex]!=m_musicPath => find m_musicPath in m_mediaList => m_mediaIndex | m_mediaIndex=0
    int                 m_mediaIndex            ;           // => m_musicPath
    QString             m_musicPath             ;           // luna://com.webos.media/unload | luna-send -n 1 -f luna://com.webos.media/load '{"uri":"file:///$m_musicPath","type":"media","payload":{"option":{"appId":"$m_appName","windowId":""}}}' => m_mediaId+m_mediaData
    QString             m_musicStorage      ;
    pbnjson::JValue     m_mediaStatus           ;
    pbnjson::JValue     m_mediaData             ;
    int                 m_playState             ;
    QString             m_mediaId               ;
    double              m_rate                  ;
    int                 m_volume                ;
    int                 m_seek                  ;
    int                 m_duration              ;
};
#endif
