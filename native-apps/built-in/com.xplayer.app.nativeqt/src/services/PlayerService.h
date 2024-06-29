#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H

#include <vector>
#include <QObject>
#include <QVariant>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include "Log.h"
#include "LunaService.h"


class PlayerService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(pbnjson::JValue mediaData READ getMediaData WRITE setMediaData NOTIFY mediaDataChanged);
    Q_PROPERTY(std::string musicPath READ getMusicPath WRITE setMusicPath NOTIFY musicPathChanged);

private:
    static PlayerService* m_instance;
    explicit PlayerService(QObject* parent = nullptr);
    ~PlayerService();
public:
    static PlayerService* instance(QObject* parent = nullptr);
    void init(std::string appName);

    pbnjson::JValue getMediaData() const { return m_mediaData;};
    void setMediaData(pbnjson::JValue mediaData) {
        if (m_mediaData != mediaData) {
            m_mediaData = mediaData;
            emit mediaDataChanged();
        }
    };

    std::string getMusicPath() const  { return m_musicPath;};
    void setMusicPath(std::string musicPath) {
        if (m_musicPath != musicPath) {
            m_musicPath = musicPath;
            emit musicPathChanged();
        }
    };

signals:
    void mediaDataChanged();
    void musicPathChanged();

private:
    void connectSignalSlots();
    void qmlRegister();

public:
    Q_INVOKABLE void onMusicPlay();
    Q_INVOKABLE void onMusicPause();
    Q_INVOKABLE void onMusicLoad(std::string path);
    Q_INVOKABLE void onUpdatePath(std::string path);

public:
    bool updateMediaId(LSMessage* msg);
public:
    pbnjson::JValue m_mediaData;
    std::string m_appName;
    std::string m_folderPath;
    std::string m_musicPath;
    std::string m_mediaId;
    int m_index;
    int m_speed;
    int m_volume;
    std::vector<pbnjson::JValue> m_listMusic;
};
#endif
