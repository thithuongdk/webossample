#!/bin/bash

build_app() {

    if [ -z "$1" ]; then
        # Gán giá trị "app" cho biến APP_NAME
        APP_NAME="com.xplayer.app.nativeqt"
    else
        APP_NAME="$1"
    fi
    # ten cua app


    # Đường dẫn đến file cần chỉnh sửa
    FILE_PATH="$WORKER_PATH/build-webos/meta-webosose/meta-webos/recipes-webos/$APP_NAME/$APP_NAME.bb"

    # Biểu thức tìm kiếm để xác định dòng cần chỉnh sửa
    SEARCH_EXPR="WEBOS_VERSION"

    # Tạo chuỗi số với định dạng yyyymmddHHMMSS
    TIMESTAMP=$(date +"%Y%m%d-%H%M%S")

    VERSION="1.0.0-$TIMESTAMP"

    # Nội dung mới để thay thế dòng cũ
    NEW_LINE="WEBOS_VERSION = \"$VERSION\""

    # đường dẫn file ipk output
    IPK_FILE_PATH="$WORKER_PATH/build-webos/BUILD/work/qemux86_64-webos-linux/$APP_NAME/$VERSION-r0.local0/deploy-ipks/qemux86_64/${APP_NAME}_$VERSION-r0.local0_qemux86_64.ipk"

    IPK_NEW_FILE_PATH="${IPK_FILE_PATH/_$VERSION-r0.local0_qemux86_64/}"

    # Chỉnh sửa dòng trong file sử dụng sed
    # -i là cờ để chỉnh sửa trực tiếp trong file
    # 's/pattern/replacement/' là cú pháp để thay thế
    sed -i 's/^WEBOS_VERSION.*/'"$NEW_LINE"'/' "$FILE_PATH"

    # Kiểm tra lỗi sau khi chỉnh sửa
    if [ $? -ne 0 ]; then
        echo "Co loi xay ra khi chih sua file."
        exit 1
    fi

    # Chạy câu lệnh bitbake
    echo "chạy lệnh bitbake"
    cd $WORKER_PATH/build-webos
    source $WORKER_PATH/build-webos/oe-init-build-env
    export BBPATH=$WORKER_PATH/build-webos
    $WORKER_PATH/build-webos/bitbake/bin/bitbake com.xplayer.app.nativeqt

    if [ $? -ne 0 ]; then
        echo "Co loi xay ra khi bitbake."
        exit 1
    fi
    # copy output
    cp "$IPK_FILE_PATH" "$IPK_NEW_FILE_PATH"
}

# ten cua app
APP_NAME="com.xplayer.app.nativeqt"

# Đường dẫn đến file cần chỉnh sửa
WEBOS_PATH_IPK="/media/internal/downloads/$APP_NAME.ipk"

# Đường dẫn host webos
LOCAL_HOST_WEBOS="localhost"

WORKER_PATH="/home/worker"
if [ -d /home/thuong/ ]; then
  WORKER_PATH="/home/thuong"
fi
echo "WORKER_PATH = $WORKER_PATH "


echo "close app $APP_NAME "
ssh -p 6622 root@$LOCAL_HOST_WEBOS "luna-send -n 1 -f luna://com.webos.applicationManager/closeByAppId '\{\"id\":\"$APP_NAME\"\}'"
if [ $? -ne 0 ]; then
    LOCAL_HOST_WEBOS="10.220.56.237"
    echo "connect ssh -p 6622 root@$LOCAL_HOST_WEBOS webos virtual."
    ssh -p 6622 root@$LOCAL_HOST_WEBOS "luna-send -n 1 -f luna://com.webos.applicationManager/closeByAppId '\{\"id\":\"$APP_NAME\"\}'"
    if [ $? -ne 0 ]; then
        echo "FAIL ssh webos virtual."
        exit 1
    fi
fi

# Chạy câu lệnh bitbake
echo "chạy lệnh run.sh"
build_app $APP_NAME
if [ $? -ne 0 ]; then
    echo "FAIL. build"
    exit 1
fi
echo "copy file ipk "

scp -O -P 6622 \
    $WORKER_PATH/webossample/native-apps/built-in/$APP_NAME/oe-workdir/deploy-ipks/qemux86_64/$APP_NAME.ipk \
    root@$LOCAL_HOST_WEBOS:/media/internal/downloads
if [ $? -ne 0 ]; then
    echo "FAIL ssh webos virtual."
    exit 1
fi
echo "install ipk"
ssh -p 6622 root@$LOCAL_HOST_WEBOS opkg remove $APP_NAME
ssh -p 6622 root@$LOCAL_HOST_WEBOS opkg install $WEBOS_PATH_IPK
echo "reset ipk"
ssh -p 6622 root@$LOCAL_HOST_WEBOS /usr/sbin/ls-control scan-services
ssh -p 6622 root@$LOCAL_HOST_WEBOS systemctl restart sam
sleep 1
ssh -p 6622 root@$LOCAL_HOST_WEBOS "luna-send -n 1 -f luna://com.webos.applicationManager/launch '\{\"id\":\"$APP_NAME\"\}'"


echo "Chinh sua file va chay bitbake thanh cong"