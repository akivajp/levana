#!/bin/sh

PWD=`pwd`
DIR=`dirname $0`

if [ $PWD = "/" ]
then
  PWD=""
fi
if [ $DIR = "." ]
then
  DIR=""
fi

echo $0 > ~/Downloads/out.txt
echo $1 >> ~/Downloads/out.txt
echo $2 >> ~/Downloads/out.txt
echo $3 >> ~/Downloads/out.txt

BIN_DIR=${PWD}${DIR}
CON_DIR=`dirname $BIN_DIR`
APP_DIR=`dirname $CON_DIR`
WORK_DIR=`dirname $APP_DIR`
cd $BIN_DIR
export LUA_PATH=${BIN_DIR}/?.lua
./levana
#./levana $BIN_DIR/entry.lua
exit 0

