#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QVariant>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include "AppService.h"
#include "PlayerService.h"
#include "Log.h"


class MediaPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int playerState READ getPlayerState WRITE setPlayerState NOTIFY playerStateChanged);
    Q_PROPERTY(QString musicPath READ getMusicPath WRITE setMusicPath NOTIFY musicPathChanged);

public:
    MediaPlayer(QObject *parent = nullptr);
    ~MediaPlayer();
    void initApplication();

    int getPlayerState() const;
    void setPlayerState(int state);

    QString getMusicPath() const;
    void setMusicPath(QString path);

signals:
    void playerStateChanged();
    void musicPathChanged();

private:
    void connectSignalSlots();
    void qmlRegister();

private:
    PlayerService* m_service;

    int m_playerState;
    QString m_musicPath;
};
#endif
