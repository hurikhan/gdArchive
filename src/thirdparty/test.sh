#! /usr/bin/env bash

_CMD='./build.sh && ./clean.sh && ./build.sh && ./sweep.sh && figlet test ok'

command time --format="CPU: %P Time:%Es" bash -c "$_CMD"
