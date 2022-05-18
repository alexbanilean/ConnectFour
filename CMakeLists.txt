cmake_minimum_required(VERSION 3.21)
project(ConnectFour)

set(CMAKE_CXX_STANDARD 17)

add_executable(ConnectFour src/main.cpp src/game.cpp headers/game.h src/gamescreen.cpp headers/gamescreen.h src/piece.cpp headers/piece.h)

include_directories(headers d:/SFML/include)

set(SFML_ROOT d:/SFML)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics)

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(ConnectFour ${SFML_LIBRARIES} ${SFML_DEPENDECIES})
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Copy dlls to build
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../BIN/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()