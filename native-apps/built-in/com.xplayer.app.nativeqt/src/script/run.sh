#!/bin/bash

build_app() {
    if [ -z "$1" ]; then
        # Gán giá trị "worker" cho biến USER_VBEE_NAME
        USER_VBEE_NAME="worker"
    else
        USER_VBEE_NAME="$1"
    fi

    if [ -z "$2" ]; then
        # Gán giá trị "app" cho biến APP_NAME
        APP_NAME="com.xplayer.app.nativeqt"
    else
        APP_NAME="$2"
    fi
    # ten cua app


    # Đường dẫn đến file cần chỉnh sửa
    FILE_PATH="/home/$USER_VBEE_NAME/build-webos/meta-webosose/meta-webos/recipes-webos/$APP_NAME/$APP_NAME.bb"

    # Biểu thức tìm kiếm để xác định dòng cần chỉnh sửa
    SEARCH_EXPR="WEBOS_VERSION"

    # Tạo chuỗi số với định dạng yyyymmddHHMMSS
    TIMESTAMP=$(date +"%Y%m%d-%H%M%S")

    VERSION="1.0.0-$TIMESTAMP"

    # Nội dung mới để thay thế dòng cũ
    NEW_LINE="WEBOS_VERSION = \"$VERSION\""

    # đường dẫn file ipk output
    IPK_FILE_PATH="/home/$USER_VBEE_NAME/build-webos/BUILD/work/qemux86_64-webos-linux/$APP_NAME/$VERSION-r0.local0/deploy-ipks/qemux86_64/${APP_NAME}_$VERSION-r0.local0_qemux86_64.ipk"

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
    cd /home/$USER_VBEE_NAME/build-webos
    source /home/$USER_VBEE_NAME/build-webos/oe-init-build-env
    export BBPATH=/home/$USER_VBEE_NAME/build-webos
    /home/$USER_VBEE_NAME/build-webos/bitbake/bin/bitbake com.xplayer.app.nativeqt

    if [ $? -ne 0 ]; then
        echo "Co loi xay ra khi bitbake."
        exit 1
    fi
    # copy output
    cp "$IPK_FILE_PATH" "$IPK_NEW_FILE_PATH"
}


USER_VBEE_NAME="thuong"
# ten cua app
APP_NAME="com.xplayer.app.nativeqt"

# Đường dẫn đến file cần chỉnh sửa
WEBOS_PATH_IPK="/media/internal/downloads/$APP_NAME.ipk"

# Chạy câu lệnh bitbake
echo "chạy lệnh run.sh"
build_app $USER_VBEE_NAME $APP_NAME
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
plink -batch -ssh -P 6622 root@localhost /usr/sbin/ls-control scan-services
plink -batch -ssh -P 6622 root@localhost systemctl restart sam
plink -batch -ssh -P 6622 root@localhost "luna-send -n 1 -f luna://com.webos.applicationManager/launch '\{\"id\":\"$APP_NAME\"\}'"


echo "Chinh sua file va chay bitbake thanh cong"