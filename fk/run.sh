#!/bin/sh

root_path=`pwd`
#export LD_LIBRARY_PATH=/usr/local/lib
#echo library path: ${root_path}/protobuflib/linux

./blade-bin/routersvr/routersvr -D --conf_dir ${root_path}/conf/
echo 'starting routersvr..........'

./blade-bin/connsvr/connsvr -D --conf_dir ${root_path}/conf/
echo 'starting connsvr..........'

