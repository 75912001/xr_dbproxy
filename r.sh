#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib
./k.sh
sleep 2
./xr_dbproxy.exe ./config.ini