#!/bin/bash

if [ $# == 1 ]; then
	if [ ! -d $1 ]; then
		mkdir -p $1
	fi
	repository=$(pwd)
	cd $1
	if [ ! -d ./include/merlin-operational-research ]; then
		mkdir -p include/merlin-operational-research
	fi
	if [ ! -d ./lib ]; then
		mkdir lib
	fi

	if [ -d "${repository}/include" ] && [ -f "${repository}/include/merlin_a_star.hpp" ]; then
		cp "${repository}/include/merlin_a_star.hpp" ./include/merlin-operational-research/
	else
		echo "[Error] Sources not found - The installation script must be called from the repository (cd to Merlin-operational-research/)."
	fi
	if [ -d "${repository}/src" ] && [ -f "${repository}/src/merlin_a_star.cpp" ]; then
		g++ -shared -fPIC -Wall -Wextra -pedantic -std=c++17 "${repository}/src/merlin_a_star.cpp" -I./include/ -o libmerlin_a_star.so
		mv libmerlin_a_star.so ./lib/
	else
		echo "[Error] Sources not found - The installation script must be called from the repository (cd to Merlin-operational-research/)."
	fi
else
	echo "[Error] Expected 1 argument ($# given) - (Only) the installation directory is required."
fi

