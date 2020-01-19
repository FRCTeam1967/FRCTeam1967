cmake_minimum_required(VERSION 2.8)
project(cscore)

set (WITHOUT_JAVA true)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++1y -Wformat=2 -Wall -Wextra -Werror -pedantic -Wno-unused-parameter")

# This must be a macro(), as inside a function string() can only
# update variables in the function scope.
macro(fix_default_compiler_settings_)
  if (MSVC)
    # For MSVC, CMake sets certain flags to defaults we want to override.
    # This replacement code is taken from sample in the CMake Wiki at
    # http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace.
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      if (NOT BUILD_SHARED_LIBS AND NOT gtest_force_shared_crt)
        # When Google Test is built as a shared library, it should also use
        # shared runtime libraries.  Otherwise, it may end up with multiple
        # copies of runtime library data in different modules, resulting in
        # hard-to-find crashes. When it is built as a static library, it is
        # preferable to use CRT as static libraries, as we don't have to rely
        # on CRT DLLs being available. CMake always defaults to using shared
        # CRT libraries, so we override that default here.
        string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
      endif()
    endforeach()
  endif()
endmacro()
fix_default_compiler_settings_()

file(GLOB SRC_FILES src/main/native/cpp/*.cpp)
LIST(APPEND ALL_SRC_FILES ${SRC_FILES})

file(GLOB SRC_FILES wpiutil/src/main/native/cpp/support/*.cpp)
LIST(APPEND ALL_SRC_FILES ${SRC_FILES})

file(GLOB SRC_FILES wpiutil/src/main/native/cpp/llvm/*.cpp)
LIST(APPEND ALL_SRC_FILES ${SRC_FILES})

include_directories(src/main/native/include src/main/native/cpp)
include_directories(java/arm-linuxsrc/main/native/include src/main/native/cpp)
include_directories(/usr/include/opencv4)

include_directories(wpiutil/include)

add_library(cscore SHARED ${ALL_SRC_FILES})
set_target_properties(cscore PROPERTIES VERSION 1.0.0 SOVERSION 1)
target_link_libraries(cscore)

INSTALL(TARGETS cscore
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib COMPONENT lib)
INSTALL(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT headers)

# We need thread support
find_package(Threads REQUIRED)

