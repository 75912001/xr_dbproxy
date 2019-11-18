#!/bin/bash
pid=`ps -ef | grep "xr_dbproxy.exe" | awk '{print $2}'`
kill -TERM $pid
pkill -9 xr_dbproxy.exe
