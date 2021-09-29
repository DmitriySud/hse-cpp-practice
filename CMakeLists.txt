project(hse_practice)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ADVICE "Better use \"make cmake-<hw-name>\" or \"make cmake-all\" to build all projects")

add_subdirectory(third_party/googletest)

if (NOT DEFINED TARGET_PROJECTS)
  message(FATAL_ERROR "Don't use cmake explicitly.\n"${ADVICE})
elseif(${TARGET_PROJECTS} STREQUAL "!!!!")
  message(FATAL_ERROR "Something went wrong... Maybe you run \"make cmake\". It is "
    " internal command.\n" ${ADVICE})
else()
  message("Cmake project : " ${TARGET_PROJECTS})
  add_subdirectory(${TARGET_PROJECTS})
endif()

