#!/bin/bash

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
    echo "Có lỗi xảy ra khi chỉnh sửa file."
    exit 1
fi

# Chạy câu lệnh bitbake
echo "chạy lệnh bitbake"
cd /home/$USER_VBEE_NAME/build-webos
source /home/$USER_VBEE_NAME/build-webos/oe-init-build-env
export BBPATH=/home/$USER_VBEE_NAME/build-webos
/home/$USER_VBEE_NAME/build-webos/bitbake/bin/bitbake com.xplayer.app.nativeqt
cp "$IPK_FILE_PATH" "$IPK_NEW_FILE_PATH"

# Kiểm tra lỗi sau khi chạy bitbake
if [ $? -ne 0 ]; then
    echo "Có lỗi xảy ra khi chạy bitbake."
    exit 1
fi

echo "Chỉnh sửa file và chạy bitbake thành công."