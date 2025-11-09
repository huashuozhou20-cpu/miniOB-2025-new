file(GLOB_RECURSE OBLSM_SOURCES "*.cpp")
file(GLOB CLIUTIL_SOURCES "client/cliutil/*.cpp")
list(FILTER OBLSM_SOURCES EXCLUDE REGEX "client/.*")
list(FILTER OBLSM_SOURCES EXCLUDE REGEX "benchmarks/.*")

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
add_library(oblsm STATIC ${OBLSM_SOURCES})
message("OBLSM_SOURCES: ${OBLSM_SOURCES}")

# Try to find jsoncpp library
find_library(JSONCPP_LIB NAMES jsoncpp libjsoncpp.a
  HINTS 
    ${PROJECT_SOURCE_DIR}/deps/3rd/jsoncpp/build/lib
    ${PROJECT_SOURCE_DIR}/deps/3rd/usr/lib 
    /usr/lib 
    /usr/local/lib
)
if(NOT JSONCPP_LIB)
  if(EXISTS ${PROJECT_SOURCE_DIR}/deps/3rd/jsoncpp/build/lib/libjsoncpp.a)
    set(JSONCPP_LIB ${PROJECT_SOURCE_DIR}/deps/3rd/jsoncpp/build/lib/libjsoncpp.a)
  else()
    set(JSONCPP_LIB jsoncpp)
  endif()
endif()

# Find jsoncpp headers
find_path(JSONCPP_INCLUDE_DIR json/json.h
  HINTS
    ${PROJECT_SOURCE_DIR}/deps/3rd/jsoncpp/include
    ${PROJECT_SOURCE_DIR}/deps/3rd/usr/include
    ${PROJECT_SOURCE_DIR}/deps/3rd/usr/include/jsoncpp
    ${PROJECT_SOURCE_DIR}/deps/3rd/usr/local/include
    /usr/include/jsoncpp
    /usr/local/include
    /usr/include
)
if(JSONCPP_INCLUDE_DIR)
  target_include_directories(oblsm PRIVATE ${JSONCPP_INCLUDE_DIR})
  message(STATUS "Found jsoncpp headers at: ${JSONCPP_INCLUDE_DIR}")
else()
  message(WARNING "jsoncpp headers not found, trying to continue without it")
endif()
TARGET_LINK_LIBRARIES(oblsm common pthread ${JSONCPP_LIB})

ADD_EXECUTABLE(oblsm_cli ${CLIUTIL_SOURCES} client/ob_lsm_client.cpp)

TARGET_LINK_LIBRARIES(oblsm_cli oblsm)

ADD_EXECUTABLE(oblsm_bench benchmarks/ob_lsm_bench.cpp)
TARGET_LINK_LIBRARIES(oblsm_bench oblsm)
