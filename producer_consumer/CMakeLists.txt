cmake_minimum_required(VERSION 3.5)
project(producer_consumer)
enable_testing()

set(CMAKE_CXX_STANDARD 14)

set(ROOT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../..)

add_definitions(-DDEBUG -D_CONSOLE -DNATIVE_LITTLE_ENDIAN)

if (WIN32)
elseif (APPLE)
    set(CMAKE_BUILD_TYPE Debug)
elseif (UNIX)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

file(GLOB LOCAL_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
)


set(SRC_FILES
	${LOCAL_SRC}
)

list(REMOVE_DUPLICATES SRC_FILES)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ROOT_SRC}
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(Threads REQUIRED)

# target_link_libraries(${PROJECT_NAME} curl)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32)
else (WIN32)
    target_link_libraries(${PROJECT_NAME} Threads::Threads)
endif (WIN32)

enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME})
