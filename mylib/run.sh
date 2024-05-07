#!/bin/bash

if [[ "${1: -2}" == ".c" ]]; then
    gcc -std=c99 -Wall -Wextra -Wshadow -O2 -DDEBUG_MODE -lm "$1" -o prog

elif [[ "${1: -4}" == ".cpp" || "${1: -4}" == ".cxx" || "${1: -3}" == ".cc" ]]; then
    g++ -std=c++11 -Wall -Wextra -Wshadow -fno-exceptions -fno-rtti -O2 -DDEBUG_MODE "$1" -o prog
else
    printf "\033[1;33mUsage:\033[0m %s <C/C++ file>" "$0"
    exit 1
fi

if [ $? -eq 0 ]; then
    printf "Done.\n\n"
else
    printf "\033[91mFailed.\033[0m\n"
    exit 1
fi

./prog

if [ $? -ne 0 ]; then
    printf "\033[91mFailed.\033[0m\n"
fi

rm prog
