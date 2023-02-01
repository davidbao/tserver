#! /bin/bash
############################################################
# Name       :      tserverd install shell
# Author     :      Baowei
# Version    :      1.0
# Description:
# Notes      :
#
############################################################

TARGET_PATH=$(pwd)
SERVICE_PATH=/etc/init.d
SERVICE_FILE=$SERVICE_PATH/tserverd
USER_NAME=tserver
GROUP_NAME=tserver
TAR_FILE=$TARGET_PATH/tserver.tar.gz

for arg in $*
do
    # echo "arg: $arg"
  if [[ "$arg" =~ ^--target-path=.* ]]; then
      TARGET_PATH=${arg#*--target-path=}
  fi
done

echo "target path: $TARGET_PATH"

# extract tar file from the current file.
ARCHIVE=$(awk '/^__ARCHIVE_BELOW__/ {print NR + 1; exit 0; }' "$0")
tail -n+$ARCHIVE "$0" > $TAR_FILE
if [ $? -ne 0 ]
then
    echo "Error: prepare env error, failed create extract tserver..." >&2
    rm -rf $TAR_FILE
    exit 1
fi

# stop tserverd if started.
if [ -f "/etc/init.d/tserverd" ];then
  /etc/init.d/tserverd stop
fi

# add tserver group & user.
groupadd $GROUP_NAME >> /dev/null 2>&1
useradd $USER_NAME -g $GROUP_NAME >> /dev/null 2>&1
# id -u $USER_NAME &>/dev/null || useradd $USER_NAME -g $GROUP_NAME
echo "tserver" | passwd $USER_NAME >> /dev/null 2>&1
# echo "$USER_NAME:tserver" | chpasswd

# extract tserver files to the target path.
echo "Extracting tserver files to the target path."
tar -xzf $TAR_FILE -C $TARGET_PATH

# set permission.
chown -R tserver:tserver $TARGET_PATH/tserver
chmod a+x $TARGET_PATH/tserver/tserver
chmod a+x $TARGET_PATH/tserver/tserverd

# create service start item.
if [ -f "$SERVICE_FILE" ];then
  rm $SERVICE_FILE
fi
if [ ! -f "$SERVICE_FILE" ];then
  touch $SERVICE_FILE
  echo -e "#!/bin/sh\n\
############################################################\n\
# Name       :      /etc/rc.d/init.d/tserverd\n\
# Author     :      Baowei\n\
# Version    :      1.0\n\
# Description:\n\
# Notes      :\n\
#\n\
############################################################\n\
\n\
#\n\
#\n\
# Include the functions declared in the /etc/init.d/functions\n\
# and /etc/sysconfig/rc file\n\
#\n\
\n\
. /etc/sysconfig/rc\n\
. \$rc_functions\n\
\n\
tserverd=$TARGET_PATH/tserver/tserverd\n\
\n\
case \$1 in\n\
  start)\n\
  boot_mesg \"Starting tserverd...\"\n\
  startproc tserver tserver \$tserverd -d\n\
  ;;\n\
  stop)\n\
  boot_mesg \"Stopping tserverd...\"\n\
  killproc \$tserverd\n\
  ;;\n\
  reload)\n\
  boot_mesg \"Reloading tserverd...\"\n\
  reloadproc \$tserverd\n\
  ;;\n\
  restart)\n\
  \$0 stop\n\
  /bin/sleep 1\n\
  \$0 start\n\
  ;;\n\
  status)\n\
  statusproc \$tserverd\n\
  ;;\n\
  *)\n\
  echo \"Usage: \$0 [start|stop|reload|restart|status]\"\n\
  exit 1\n\
    ;;\n\
esac\n\
\n\
# End of file\n\
" > $SERVICE_FILE
fi
chown sys:sys $SERVICE_FILE
chmod a+x $SERVICE_FILE

# auto start
ln -s $SERVICE_FILE /etc/rc.d/rc3.d/S099tserverd  # multi users
ln -s $SERVICE_FILE /etc/rc.d/rc5.d/S099tserverd  # X11 or KDE

# start tserverd
/etc/init.d/tserverd start

# check
if ps -ef | grep tserverd | grep -v grep > /dev/null; then
  echo -e "\033[32mInstall successfully.\033[0m"  # green
else
  echo -e "\033[31mFail to install!\033[0m"   # red
fi

# remove temp file.
rm -rf $TAR_FILE

exit 0
#This line must be the last line of the file
__ARCHIVE_BELOW__
