#! /bin/bash
############################################################
# Name       :      tserverd make shell
# Author     :      Baowei
# Version    :      1.0
# Description:
# Notes      :        
#
############################################################

TARGET_PATH=$(pwd)
SOURCE_PATH=$TARGET_PATH
APP_NAME=tserver

for arg in $*
do
  	# echo "arg: $arg"
	if [[ "$arg" =~ ^--target-path=.* ]]; then
  		TARGET_PATH=${arg#*--target-path=}
	fi
	if [[ "$arg" =~ ^--source-path=.* ]]; then
      SOURCE_PATH=${arg#*--source-path=}
  fi
done

echo "target path: $TARGET_PATH"
echo "source path: $SOURCE_PATH"

# check
if [ ! -f "$SOURCE_PATH/tserver.install.source.sh" ];then
  echo "Can not find 'tserver.install.source.sh'!"
	exit 1
fi

if [ ! -d "$SOURCE_PATH/$APP_NAME" ];then
  echo "Can not find 'tserver path'!"
	exit 2
fi

# compress tserver files.
tar -czvf "$TARGET_PATH"/tserver.tar.gz "$SOURCE_PATH"/$APP_NAME

# combine sh file and tar file.
cat "$SOURCE_PATH"/tserver.install.source.sh "$SOURCE_PATH"/tserver.tar.gz > "$TARGET_PATH"/tserver.install.sh
