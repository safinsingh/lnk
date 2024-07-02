#!/bin/bash

[[ -d scripts ]] || { echo >&2 "please run from top-level dir of project" && exit 1; }

./build.sh && ./build/lnk
