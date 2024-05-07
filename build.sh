#!/bin/bash

g++ -o ./app ./src/arqsocket.cpp ./src/arqmessage.cpp ./src/app.cpp -Iinclude -larqanore -Wl,-rpath='$ORIGIN:$ORIGIN/lib'
