#include "MediaPlayer.h"
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>

MediaPlayer::MediaPlayer(QObject *parent)
    : QObject(parent)
    , m_service(nullptr)
{
}

MediaPlayer::~MediaPlayer()
{
    if (m_service != nullptr) {
        delete m_service;
    }
}

void MediaPlayer::initApplication()
{
    m_service = PlayerService::instance(this);
    connectSignalSlots();
    qmlRegister();
}

void MediaPlayer::connectSignalSlots() {
    // connect(m_service, &AppService::createWindow,                 this, &MediaPlayer::onCreateWindow);
}

void MediaPlayer::qmlRegister() {
}

int MediaPlayer::getPlayerState() const {
    return m_playerState;
}

void MediaPlayer::setPlayerState(int state) {
    if (state != m_playerState) {
        m_playerState = state;
        emit playerStateChanged();
    }
}

QString MediaPlayer::getMusicPath() const {
    return m_musicPath;
}

void MediaPlayer::setMusicPath(QString path) {
    if (path != m_musicPath) {
        m_musicPath = path;
        emit musicPathChanged();
    }
}
