#!/bin/sh

root_path=`pwd`
#export LD_LIBRARY_PATH=/usr/local/lib
#echo library path: ${root_path}/protobuflib/linux

echo "root_path:${root_path}"

svr_array=(routersvr connsvr)

if [ $# == 1 ];then
   echo "starting $1.........."
   ./blade-bin/$1/$1 -D --conf_dir ${root_path}/conf/
else
   for item in ${svr_array[@]}
   do
     echo "starting $item.........."
     ./blade-bin/$item/$item -D --conf_dir ${root_path}/conf/
   done
fi

