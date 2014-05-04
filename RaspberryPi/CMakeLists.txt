cmake_minimum_required (VERSION 2.6)
project (MediaControlSystem)

set(SOURCE
    src/main.cpp
    src/GPIO.cpp
    src/USARTHandler.cpp
)

add_executable(MediaControlSystem ${SOURCE})

