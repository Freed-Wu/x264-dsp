cmake_minimum_required(VERSION 3.10)
if(DEFINED ENV{GITHUB_REF_NAME})
  set(VERSION $ENV{GITHUB_REF_NAME})
else()
  set(VERSION 0.0.0.0)
endif()
# https://gitlab.kitware.com/cmake/cmake/-/merge_requests/9811
project(
  x264-dsp
  VERSION ${VERSION}
  DESCRIPTION x264-dsp
  HOMEPAGE_URL https://github.com/Freed-Wu/x264-dsp
  LANGUAGES C ASM_LINEAR)
file(GLOB SRC *.c common/*.c encoder/*.c)
add_executable(x264 ${SRC} ${DSP_SRC})
install(TARGETS x264 RUNTIME)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include(CheckLibraryExists)
check_library_exists(m logf "" HAVE_LIB_M)
if(HAVE_LIB_M)
  target_link_libraries(x264 PUBLIC m)
endif()

set(_DOC_INPUT_FILENAME
    "input filename"
    CACHE STRING "")
set(INPUT_FILENAME
    "352x288.yuv"
    CACHE STRING ${_DOC_INPUT_FILENAME})

set(_DOC_BIN2C
    "use bin2c"
    CACHE STRING "")
option(BIN2C ${_DOC_BIN2C})
if(BIN2C)
  find_program(bin2c REQUIRED NAMES bin2c)
  if(NOT EXISTS ${INPUT_FILENAME})
    message(FATAL_ERROR "${INPUT_FILENAME} doesn't exist!")
  endif()
  add_custom_target(yuv.h ALL COMMAND bin2c yuv < ${INPUT_FILENAME} >
    ${CMAKE_BINARY_DIR}/yuv.h)
  add_dependencies(x264 yuv.h)
endif()

set(_DOC_DRY_RUN
    "do not write any file"
    CACHE STRING "")
option(DRY_RUN ${_DOC_DRY_RUN})

set(_DOC_HAVE_TIC6X
    "enable TI C6X asm"
    CACHE STRING "")
option(HAVE_TIC6X ${_DOC_HAVE_TIC6X} ON)

set(_DOC_DOWNSAMPLE
    "downsample from 720p to 360p, 1, 2 means bilinear, bicubic"
    CACHE STRING "")
set(DOWNSAMPLE
    0
    CACHE STRING ${_DOC_DOWNSAMPLE})
if(NOT (DOWNSAMPLE GREATER_EQUAL 0 AND DOWNSAMPLE LESS 3))
  message(FATAL_ERROR "${DOWNSAMPLE} is an invalid DOWNSAMPLE!")
endif()

set(_DOC_PADDING
    "padding method, 1..3 means edge, reflect, symmetric"
    CACHE STRING "")
set(PADDING
    3
    CACHE STRING ${_DOC_PADDING})
if(NOT (PADDING GREATER 0 AND PADDING LESS 4))
  message(FATAL_ERROR "${PADDING} is an invalid PADDING!")
endif()

set(_DOC_SCALE
    "SCALE scale, a positive number"
    CACHE STRING "")
set(SCALE
    2
    CACHE STRING ${_DOC_SCALE})
if(SCALE LESS_EQUAL 0)
  message(FATAL_ERROR "${SCALE} is an invalid SCALE!")
endif()

set(_DOC_X264_BIT_DEPTH
    "bit depth, can be 8 or 10"
    CACHE STRING "")
set(X264_BIT_DEPTH
    8
    CACHE STRING ${_DOC_X264_BIT_DEPTH})
set(_LIST_X264_BIT_DEPTH 8 10)
if(NOT (X264_BIT_DEPTH IN_LIST _LIST_X264_BIT_DEPTH))
  message(FATAL_ERROR "${X264_BIT_DEPTH} is an invalid X264_BIT_DEPTH!")
endif()

set(_DOC_X264_CHROMA_FORMAT
    "chroma format, 0..3 means 400, 420, 422, 444"
    CACHE STRING "")
set(X264_CHROMA_FORMAT
    1
    CACHE STRING ${_DOC_X264_CHROMA_FORMAT})
if(NOT (X264_CHROMA_FORMAT GREATER_EQUAL 0 AND X264_CHROMA_FORMAT LESS 4))
  message(FATAL_ERROR "${X264_CHROMA_FORMAT} is an invalid X264_CHROMA_FORMAT!")
endif()

set(_DOC_X264_LOG_LEVEL
    "log level, 0..3 means error, warning, info, debug"
    CACHE STRING "")
set(X264_LOG_LEVEL
    2
    CACHE STRING ${_DOC_X264_LOG_LEVEL})
if(NOT (X264_LOG_LEVEL GREATER_EQUAL 0 AND X264_LOG_LEVEL LESS 4))
  message(FATAL_ERROR "${X264_LOG_LEVEL} is an invalid X264_LOG_LEVEL!")
endif()

configure_file(cmake/config.h.in config.h)

include(CTest)
add_subdirectory(tests EXCLUDE_FROM_ALL)

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_ARCHIVE_THREADS 0)
set(CPACK_THREADS 0)
