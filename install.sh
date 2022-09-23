#!/bin/bash

if [ $# == 1 ]; then
	if [ ! -d $1 ]; then
		mkdir -p $1
	fi
	repository=$(pwd)
	cd $1
	if [ ! -d ./include ]; then
		mkdir -p include
	fi
	if [ ! -d ./lib ]; then
		mkdir lib
	fi

	if [ -d "${repository}/include" ] && [ -f "${repository}/include/merlin_operational_research" ]; then
		cp "${repository}/include/merlin_"*.hpp ./include/
		cp "${repository}/include/merlin_operational_research" ./include/
	else
		echo "[Error] Sources not found - The installation script must be called from the repository (cd to Merlin-operational-research/)."
	fi
	if [ -d "${repository}/src" ]; then
		g++ -shared -fPIC -Wall -Wextra -pedantic -std=c++17 -O2 "${repository}/src/merlin_"*.cpp -I./include/ -o libmerlin_operational_research.so
		mv libmerlin_operational_research.so ./lib/
	else
		echo "[Error] Sources not found - The installation script must be called from the repository (cd to Merlin-operational-research/)."
	fi
else
	echo "[Error] Expected 1 argument ($# given) - (Only) the installation directory is required."
fi

