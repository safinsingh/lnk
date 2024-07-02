#!/bin/bash

[[ -d scripts ]] || { echo >&2 "please run from top-level dir of project" && exit 1; }

mkdir -p build
(
	cd build
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
	cmake --build .
)
