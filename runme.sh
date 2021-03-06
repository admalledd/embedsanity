#!/bin/bash
set -e
RESET_SEQ="\033[0m"

BLACK="\033[1;30m"
RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
BLUE="\033[1;34m"
MAGENTA="\033[1;35m"
CYAN="\033[1;36m"
WHITE="\033[1;37m"

BOLD_SEQ="\033[1m"


export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/admalledd/dev/pypy/builds/pypy-py3k-64bit-2014-10-31/bin:/usr/lib/jvm/java-7-openjdk-amd64/jre/lib/amd64/server

#default: recompile
echo -e "${RED}recompiling...${RESET_SEQ}"
(rm ./bin/embsanity.bin || true) #ignore failing rm
(rm ./c_src/bin/debug/* ||true)  #ignore failing rm
(rm ./hs_err_pid* ||true)        #ignore failing rm

cd c_src
make -j8 debug V=true
make install #install on its own step due to out-of-order building above
cd ..
echo -e "${RED}compile bootstraps... (java,mono,etc)${RESET_SEQ}"
cd data
make debug V=true
cd ..

echo -e "${RED}recompile done! launch follows:${RESET_SEQ}"
echo -e "\n\n\n"


#gdb -tui ./bin/embsanity.bin $1

./bin/embsanity.bin $1

#if valgrinding:

#valgrind --leak-check=full --num-callers=500 ./bin/embsanity.bin $1
