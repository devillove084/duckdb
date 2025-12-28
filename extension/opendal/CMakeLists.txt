cmake_minimum_required(VERSION 3.5...3.29)
project(OpenDALExtension)

include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include(ExternalProject)

set(OPENDAL_SRC_DIR "${CMAKE_BINARY_DIR}/third_party/opendal")
set(OPENDAL_BUILD_DIR "${CMAKE_BINARY_DIR}/third_party/opendal_build")
set(OPENDAL_HEADER_DIR "${CMAKE_BINARY_DIR}/third_party/opendal_build/include")

set(OPENDAL_CPP_STATIC "${OPENDAL_BUILD_DIR}/libopendal_cpp.a")
set(OPENDAL_RUST_STATIC "${OPENDAL_SRC_DIR}/bindings/cpp/target/release/libopendal_cpp.a")

set(OPENDAL_CXXBRIDGE_DIR "${OPENDAL_SRC_DIR}/bindings/cpp/target/cxxbridge/opendal-cpp/src")
set(OPENDAL_CXX_ASYNC_RUNTIME_CC "${OPENDAL_CXXBRIDGE_DIR}/async.rs.cc")
set(OPENDAL_CXX_RUNTIME_CC "${OPENDAL_CXXBRIDGE_DIR}/lib.rs.cc")

ExternalProject_Add(
  opendal_kernel
  GIT_REPOSITORY "https://github.com/devillove084/incubator-opendal.git"
  GIT_TAG "fix/reexport_cpp_header"
  SOURCE_DIR "${OPENDAL_SRC_DIR}"
  SOURCE_SUBDIR "bindings/cpp"
  BINARY_DIR "${OPENDAL_BUILD_DIR}"
  CMAKE_ARGS
  -DOPENDAL_ENABLE_ASYNC=ON
  -DCMAKE_BUILD_TYPE=Release
  INSTALL_COMMAND ""

  BUILD_BYPRODUCTS
  "${OPENDAL_CPP_STATIC}"
  "${OPENDAL_RUST_STATIC}"
  "${OPENDAL_CXX_RUNTIME_CC}"
  "${OPENDAL_CXX_ASYNC_RUNTIME_CC}"
)

set(EXTENSION_SOURCES
  opendal_extension.cpp
  opendal_filesystem.cpp
  opendal_scan_function.cpp
)

include_directories("${OPENDAL_SRC_DIR}/bindings/cpp/target/cxxbridge")
include_directories("${OPENDAL_SRC_DIR}/bindings/cpp/include")
include_directories(
  ${OPENDAL_HEADER_DIR}
  ${OPENDAL_CXXBRIDGE_DIR}
)


set(PARAMETERS "-warnings")
build_static_extension(opendal ${EXTENSION_SOURCES})
build_loadable_extension(opendal ${PARAMETERS} ${EXTENSION_SOURCES})

set(_ASYNC_DEFS "${OPENDAL_SRC_DIR}/bindings/cpp/include/async_defs.hpp")

add_dependencies(opendal_extension opendal_kernel)
add_dependencies(opendal_loadable_extension opendal_kernel)

set_property(TARGET opendal_extension APPEND PROPERTY COMPILE_OPTIONS "-include" "${_ASYNC_DEFS}")
set_property(TARGET opendal_loadable_extension APPEND PROPERTY COMPILE_OPTIONS "-include" "${_ASYNC_DEFS}")


target_sources(opendal_extension PRIVATE "${OPENDAL_CXX_RUNTIME_CC}")
target_sources(opendal_loadable_extension PRIVATE "${OPENDAL_CXX_RUNTIME_CC}")

target_sources(opendal_extension PRIVATE "${OPENDAL_CXX_ASYNC_RUNTIME_CC}")
target_sources(opendal_loadable_extension PRIVATE "${OPENDAL_ASYNC_CXX_RUNTIME_CC}")


find_package(Threads REQUIRED)

target_link_libraries(opendal_extension
  "${OPENDAL_CPP_STATIC}"
  -Wl,--whole-archive "${OPENDAL_RUST_STATIC}" -Wl,--no-whole-archive
  Threads::Threads
  ${CMAKE_DL_LIBS}
)

target_link_libraries(opendal_loadable_extension
  "${OPENDAL_CPP_STATIC}"
  -Wl,--whole-archive "${OPENDAL_RUST_STATIC}" -Wl,--no-whole-archive
  Threads::Threads
  ${CMAKE_DL_LIBS}
)

install(
  TARGETS opendal_extension
  EXPORT "${DUCKDB_EXPORT_SET}"
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
)
