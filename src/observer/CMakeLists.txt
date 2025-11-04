MESSAGE(STATUS "This is CMAKE_CURRENT_SOURCE_DIR dir " ${CMAKE_CURRENT_SOURCE_DIR})

# 让源码目录在 include 搜索路径（保留你原来的）
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# -------- Libevent：优先用 vendored 前缀，其次兜底手动找库 --------
# 你的第三方在 deps/3rd/usr 下
list(PREPEND CMAKE_PREFIX_PATH
  ${PROJECT_SOURCE_DIR}/deps/3rd/usr
  ${PROJECT_SOURCE_DIR}/deps/3rd/usr/lib/cmake/Libevent
)

# 优先尝试 CONFIG 模式
find_package(Libevent CONFIG QUIET)

if (NOT Libevent_FOUND)
  message(STATUS "Libevent CONFIG not found, fallback to manual locate...")

  # 头文件
  find_path(LIBEVENT_INCLUDE_DIR event2/event.h
    HINTS ${PROJECT_SOURCE_DIR}/deps/3rd/usr/include /usr/include /usr/local/include
  )
  # 库：至少需要 core + pthreads
  find_library(LIBEVENT_CORE NAMES event_core
    HINTS ${PROJECT_SOURCE_DIR}/deps/3rd/usr/lib /usr/lib /usr/local/lib)
  find_library(LIBEVENT_PTHREADS NAMES event_pthreads
    HINTS ${PROJECT_SOURCE_DIR}/deps/3rd/usr/lib /usr/lib /usr/local/lib)

  if (NOT LIBEVENT_INCLUDE_DIR OR NOT LIBEVENT_CORE OR NOT LIBEVENT_PTHREADS)
    message(FATAL_ERROR "Libevent not found. Set CMAKE_PREFIX_PATH to deps/3rd/usr or install libevent-dev")
  endif()

  # 兜底创建 imported targets：名字与官方保持一致（注意大写 L）
  add_library(Libevent::core UNKNOWN IMPORTED)
  set_target_properties(Libevent::core PROPERTIES
    IMPORTED_LOCATION "${LIBEVENT_CORE}"
    INTERFACE_INCLUDE_DIRECTORIES "${LIBEVENT_INCLUDE_DIR}"
  )
  add_library(Libevent::pthreads UNKNOWN IMPORTED)
  set_target_properties(Libevent::pthreads PROPERTIES
    IMPORTED_LOCATION "${LIBEVENT_PTHREADS}"
    INTERFACE_INCLUDE_DIRECTORIES "${LIBEVENT_INCLUDE_DIR}"
  )
endif()

# -------- JsonCpp：优先用 vendored 静态库，找不到再退回原写法 --------
find_library(JSONCPP_LIB NAMES jsoncpp libjsoncpp.a
  HINTS ${PROJECT_SOURCE_DIR}/deps/3rd/usr/lib /usr/lib /usr/local/lib
)
if(NOT JSONCPP_LIB)
  # 兜底：保持你原有硬编码（如果系统能解析）
  set(JSONCPP_LIB libjsoncpp.a)
endif()

# -------- 源文件收集 --------
FILE(GLOB_RECURSE ALL_SRC *.cpp *.c)
SET(MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
MESSAGE("MAIN SRC: " ${MAIN_SRC})
FOREACH (F ${ALL_SRC})
  IF (NOT ${F} STREQUAL ${MAIN_SRC})
    SET(LIB_SRC ${LIB_SRC} ${F})
    MESSAGE("Use " ${F})
  ENDIF()
ENDFOREACH (F)

# -------- 输出路径 --------
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
MESSAGE("Binary directory:" ${EXECUTABLE_OUTPUT_PATH})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
MESSAGE("Archive directory:" ${LIBRARY_OUTPUT_PATH})

# -------- 先建静态库，再建可执行并链接 --------
ADD_LIBRARY(observer_static STATIC ${LIB_SRC})
# === JsonCpp 头文件路径修复（同时兼容两种目录结构） ===
target_include_directories(observer_static PRIVATE
  ${PROJECT_SOURCE_DIR}/deps/3rd/usr/local/include        # deps/3rd/usr/local/include/json/json.h
  ${PROJECT_SOURCE_DIR}/deps/3rd/usr/include              # 可能是 include/json/json.h
  ${PROJECT_SOURCE_DIR}/deps/3rd/usr/include/jsoncpp      # 也可能是 include/jsoncpp/json/json.h
  /usr/include/jsoncpp                                    # 如果系统装了 libjsoncpp-dev
)
INCLUDE (readline)
MINIOB_FIND_READLINE()
IF (HAVE_READLINE)
  TARGET_LINK_LIBRARIES(observer_static ${READLINE_LIBRARY})
  TARGET_INCLUDE_DIRECTORIES(observer_static PRIVATE ${READLINE_INCLUDE_DIR})
  ADD_DEFINITIONS(-DUSE_READLINE)
  MESSAGE ("observer_static use readline")
ELSE ()
  MESSAGE ("readline is not found")
ENDIF()

# 注意：这里用 Libevent::core / Libevent::pthreads（大写 L）
TARGET_LINK_LIBRARIES(observer_static
  common pthread dl
  Libevent::core Libevent::pthreads
  ${JSONCPP_LIB}
)

SET_TARGET_PROPERTIES(observer_static PROPERTIES OUTPUT_NAME observer)

ADD_EXECUTABLE(observer ${MAIN_SRC})
TARGET_LINK_LIBRARIES(observer observer_static)

# 安装
INSTALL(TARGETS observer observer_static
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib)

