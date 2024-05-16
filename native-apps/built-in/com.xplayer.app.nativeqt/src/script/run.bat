echo hello world

set APP_NAME=com.xplayer.app.nativeqt
set PATH_IPK=/home/worker/samples/native-apps/built-in/%APP_NAME%/oe-workdir/deploy-ipks/qemux86_64/%APP_NAME%.ipk
set LOCAL_PATH_IPK=C:\Users\thuong3.nguyen\Downloads\%APP_NAME%.ipk
set WEBOS_PATH_IPK=/media/internal/downloads/%APP_NAME%.ipk
set KEY_SSH=C:/Users/thuong3.nguyen/.ssh/keyssh1.ppk
set SCRIPT_PATH=/home/worker/samples/native-apps/built-in/%APP_NAME%/src/script/run.sh

echo build ipk
plink -batch -ssh -i %KEY_SSH% -P 40750 worker@thuong3nguyen-webos.vbee.lge.com %SCRIPT_PATH%

echo copy ipk to local
scp -P 40750 worker@thuong3nguyen-webos.vbee.lge.com:%PATH_IPK% %LOCAL_PATH_IPK%

echo copy ipk to webos
scp -P 6622 %LOCAL_PATH_IPK% root@localhost:%WEBOS_PATH_IPK%

echo remove ipk
plink -batch -ssh -P 6622 root@localhost "luna-send -n 1 -f luna://com.webos.applicationManager/closeByAppId '{\"id\":\"%APP_NAME%\"}'"
plink -batch -ssh -P 6622 root@localhost opkg remove %APP_NAME%
echo install ipk
plink -batch -ssh -P 6622 root@localhost opkg install %WEBOS_PATH_IPK%
echo reset ipk
plink -batch -ssh -P 6622 root@localhost "luna-send -n 1 -f luna://com.webos.service.ls2/ls-control/scan-services '{}'"
plink -batch -ssh -P 6622 root@localhost systemctl restart sam
plink -batch -ssh -P 6622 root@localhost "luna-send -n 1 -f luna://com.webos.applicationManager/launch '{\"id\":\"%APP_NAME%\"}'"

