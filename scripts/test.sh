#!/bin/bash

[[ -d scripts ]] || { echo >&2 "please run from top-level dir of project" && exit 1; }

read -rp "is \`lnk\` already running in the background? (y/n) [y]: " answer
if [[ "$answer" == "n" ]]; then
	echo >&2 "please run from top-level dir of project"
	exit 1
fi

if ! command -v ab &>/dev/null; then
	echo >&2 "ab not found; please install ApacheBench"
	exit 1
fi

echo "a;b" >lnk.cfg
ab -n 20000 -c 5000 "http://localhost:8080/a"
