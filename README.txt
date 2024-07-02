lnk - fast [link] shortener
===========================

`lnk` is a fast (see `scripts/test.sh`; ~15k req/sec on my 5800x) link shortener written in pure C99.

`lnk` is multithreaded and uses a thread-safe queue to handle incoming requests and distribute them
among threads.

`lnk` uses non-blocking sockets and monitors them with an edge-triggered epoll(7) interface.

`lnk` supports configuration in a `lnk.cfg` file with the format as follows: `a;https://google.com`
will redirect /a --> google.com.

`lnk` uses my hashmap implementation (https://github.com/safinsingh/maps) to store configuration,
so please ensure submodules are cloned before building.

`lnk` is built with CMake; you can automate the build process by running `./scripts/{build,run}.sh`
from the root of the project.

`lnk` includes an optimization script to test different numbers of threads and lengths of the epoll
event buffer; you can run it with `python3 scripts/optimize.py`
