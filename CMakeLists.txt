project(ceph-mesos)
cmake_minimum_required(VERSION 2.8.0)
enable_testing()

find_package(Protobuf 2.5.0 EXACT REQUIRED)
find_package(Boost REQUIRED)

find_library(
    GFLAGS_LIB
    gflags
    DOC "The commandline flags library used within Google"
)

if (GFLAGS_LIB MATCHES GFLAGS_LIB-NOTFOUND)
    MESSAGE(FATAL_ERROR "gflags not found")
endif()

find_library(
    GLOG_LIB
    glog
    DOC "Logging library for C++"
)

if (GLOG_LIB MATCHES GLOG_LIB-NOTFOUND)
    MESSAGE(FATAL_ERROR "glog not found")
endif()


find_library(
    YAMLCPP_LIB
    yaml-cpp
    DOC "A YAML parser and emitter in C++"
)

if (YAMLCPP_LIB MATCHES YAMLCPP_LIB-NOTFOUND)
    MESSAGE(FATAL_ERROR "YAML-CPP not found")
endif()

find_library(
    JSONCPP_LIB
    jsoncpp
    DOC "A C++ library for interacting with JSON"
)

if (JSONCPP_LIB MATCHES JSONCPP_LIB-NOTFOUND)
    MESSAGE(FATAL_ERROR "JSONCPP not found")
endif()

find_library(
    MICROHTTPD_LIB
    microhttpd
    DOC "A C library to make it easy to run an HTTP server"
)

if (MICROHTTPD_LIB MATCHES MICROHTTPD_LIB-NOTFOUND)
    MESSAGE(FATAL_ERROR "GNU libmicrohttpd not found")
endif()

find_library(
    MESOS_LIB
    mesos
    DOC "A distributed systems kernel"
)

if (MESOS_LIB MATCHES MESOS_LIB-NOTFOUND)
    MESSAGE(FATAL_ERROR "Mesos not found")
endif()

find_library(
    GTEST_LIB
    gtest
    DOC "Google's framework for writing C++ tests"
)

if (GTEST_LIB MATCHES GTEST_LIB-NOTFOUND)
    MESSAGE(FATAL_ERROR "Google testing Framework not found")
endif()

find_library(
    PTHREAD_LIB
    pthread
    DOC "threading library"
)

if (PTHREAD_LIB MATCHES PTHREAD_LIB-NOTFOUND)
    MESSAGE(FATAL_ERROR "pthread not found")
endif()

find_library(
    RT_LIB
    rt
    DOC "rt library"
)

if (RT_LIB MATCHES RT_LIB-NOTFOUND)
    MESSAGE(FATAL_ERROR "rt not found")

endif()

# CXX Flags
add_definitions(-std=c++11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pedantic")

set(CEPH_MESOS_SCHEDULER_SOURCES
    src/common/Config.cpp
    src/scheduler/main.cpp
    src/httpserver/FileServer.cpp
    src/httpserver/RestServer.cpp
    src/scheduler/CephScheduler.cpp
    src/scheduler/EventLoop.cpp
    src/state/StateMachine.cpp
)

set(CEPH_MESOS_EXECUTOR_SOURCES
    src/executor/CephExecutor.cpp
    src/httpserver/FileServer.cpp
)

set(CEPH_MESOS_TEST_SOURCES
    src/tests/main.cpp
    src/tests/ConfigTest.cpp
    src/common/Config.cpp
    /usr/src/gmock/gmock-all.cc
)
set(CEPH_MESOS_SCHEDULER_TEST_SOURCES
    src/tests/main.cpp
    src/tests/CephSchedulerAgentTest.cpp
    src/common/Config.cpp
    src/scheduler/EventLoop.cpp
    src/state/StateMachine.cpp
    /usr/src/gmock/gmock-all.cc
)

add_executable(ceph-mesos ${CEPH_MESOS_SCHEDULER_SOURCES})
target_include_directories(ceph-mesos PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(ceph-mesos
    ${GFLAGS_LIB}
    ${YAMLCPP_LIB}
    ${MICROHTTPD_LIB}
    ${MESOS_LIB}
    ${JSONCPP_LIB}
    ${PTHREAD_LIB}
    ${RT_LIB}
)
add_custom_command(TARGET ceph-mesos POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/etc/cephmesos
    $<TARGET_FILE_DIR:ceph-mesos>
)

add_executable(ceph-mesos-executor ${CEPH_MESOS_EXECUTOR_SOURCES})
target_include_directories(ceph-mesos-executor PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ceph-mesos-executor
    ${MESOS_LIB}
    ${MICROHTTPD_LIB}
)

add_executable(ceph-mesos-tests ${CEPH_MESOS_TEST_SOURCES})
target_include_directories(ceph-mesos-tests PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)
add_executable(ceph-mesos-scheduler-tests ${CEPH_MESOS_SCHEDULER_TEST_SOURCES})
target_include_directories(ceph-mesos-scheduler-tests PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(ceph-mesos-tests
    ${GTEST_LIB}
    ${PTHREAD_LIB}
    ${GFLAGS_LIB}
    ${YAMLCPP_LIB}
)
target_link_libraries(ceph-mesos-scheduler-tests
    ${GTEST_LIB}
    ${GFLAGS_LIB}
    ${YAMLCPP_LIB}
    ${MESOS_LIB}
    ${JSONCPP_LIB}
    ${PTHREAD_LIB}
    ${RT_LIB}
)

add_test(config-tests ceph-mesos-tests)
add_test(scheduler-tests ceph-mesos-scheduler-tests)
