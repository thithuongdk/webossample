#!/bin/bash

USER_VBEE_NAME="thuong"
# ten cua app
APP_NAME="com.xplayer.app.nativeqt"

# Đường dẫn đến file cần chỉnh sửa
WEBOS_PATH_IPK="/media/internal/downloads/$APP_NAME.ipk"

# Chạy câu lệnh bitbake
echo "chạy lệnh run.sh"
/home/$USER_VBEE_NAME/webossample/native-apps/built-in/$APP_NAME/src/script/run.sh thuong com.xplayer.app.nativeqt
if [ $? -ne 0 ]; then
    echo "FAIL. build"
    exit 1
fi
echo "copy file ipk "
scp -O -P 6622 \
    /home/$USER_VBEE_NAME/webossample/native-apps/built-in/$APP_NAME/oe-workdir/deploy-ipks/qemux86_64/$APP_NAME.ipk \
    root@localhost:/media/internal/downloads
if [ $? -ne 0 ]; then
    echo "FAIL ssh webos virtual."
    exit 1
fi
echo "remove ipk"
plink -batch -ssh -P 6622 root@localhost "luna-send -n 1 -f luna://com.webos.applicationManager/closeByAppId '\{\"id\":\"$APP_NAME\"\}'"
plink -batch -ssh -P 6622 root@localhost opkg remove $APP_NAME
echo "install ipk"
plink -batch -ssh -P 6622 root@localhost opkg install $WEBOS_PATH_IPK
echo "reset ipk"
plink -batch -v -ssh -P 6622 root@localhost "luna-send -n 1 -f luna://com.webos.service.ls2/ls-control/scan-services \{\}"
plink -batch -ssh -P 6622 root@localhost systemctl restart sam
plink -batch -ssh -P 6622 root@localhost "luna-send -n 1 -f luna://com.webos.applicationManager/launch '\{\"id\":\"$APP_NAME\"\}'"

# Kiểm tra lỗi sau khi chạy bitbake
if [ $? -ne 0 ]; then
    echo "FAIL."
    exit 1
fi

echo "SUCESSS"