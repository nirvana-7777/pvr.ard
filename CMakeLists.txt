cmake_minimum_required(VERSION 3.5)
project(pvr.ard)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

find_package(Kodi REQUIRED)
find_package(RapidJSON 1.0.2 REQUIRED)

include_directories(${KODI_INCLUDE_DIR}/.. # Hack way with "/..", need bigger Kodi cmake rework to match right include ways
                    ${RAPIDJSON_INCLUDE_DIRS}
                    )

set(ARD_SOURCES
  src/http/Curl.cpp
  src/http/HttpClient.cpp
  src/Utils.cpp
  src/Settings.cpp
  src/PVRARD.cpp
)

set(ARD_HEADERS
  src/http/Curl.h
  src/http/HttpClient.h
  src/Utils.h
  src/Settings.h
  src/PVRARD.h
)

addon_version(pvr.ard ARD)

build_addon(pvr.ard ARD DEPLIBS)

include(CPack)
