#!/bin/bash

g++ -o ./app ./src/main.cpp -larqanore -Iinclude/ -Llib -Wl,-rpath='$ORIGIN:$ORIGIN/lib'
