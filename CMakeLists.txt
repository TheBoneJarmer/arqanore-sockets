cmake_minimum_required(VERSION 3.28)
project(sockets)

set(CMAKE_CXX_STANDARD 14)

if (WIN32)
	add_compile_definitions(_WIN32)
    include_directories("C:/Include")
    link_directories("C:/Lib")
	link_libraries(wsock32 ws2_32)
endif (WIN32)

add_library(arqanore-sockets include/arqanore/arqsocket.h src/arqsocket.cpp src/arqmessage.cpp include/arqanore/arqmessage.h)
target_include_directories(arqanore-sockets PUBLIC include)
target_link_libraries(arqanore-sockets arqanore)

add_executable(app src/app.cpp src/arqsocket.cpp include/arqanore/arqsocket.h src/arqmessage.cpp include/arqanore/arqmessage.h)
target_include_directories(app PUBLIC include)
target_link_libraries(app arqanore)