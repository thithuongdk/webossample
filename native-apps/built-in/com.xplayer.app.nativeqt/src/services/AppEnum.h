#ifndef APPENUM_H
#define APPENUM_H

#include <QObject>

class AppEnum : public QObject
{
    Q_OBJECT
public:
    AppEnum(QObject* parent=nullptr);
    ~AppEnum();

    enum E_APP_PLAYER_STATE {
            APP_PLAYER_ERROR = -1,
            APP_PLAYER_STOP = 0,
            APP_PLAYER_PLAY,
            APP_PLAYER_PAUSE
        };

    Q_ENUM(E_APP_PLAYER_STATE);
};

#endif
