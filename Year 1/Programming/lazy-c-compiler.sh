#!/bin/bash
if [ "$1" == "-h" ];
then
    echo "------------------------------------"
    echo "./compile.sh [file name] [option]"
    echo "--------+---------------------------"
    echo "   -h   |   see this message"
    echo "   -r   |   run code after compiling"
else
    gcc -c $1.c
    gcc -o $1 $1.o
    gcc -o $1 $1.c
fi

if [ "$2" == "-r" ] && [ "$1" != "-h" ];
then
    ./$1
fi

#https://github.com/TheBanditOfRed