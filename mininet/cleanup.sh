#!/bin/bash

jobs -l | grep -o "[0-9]* " | xargs -E eofstr
killall myhttpd
mn -c
