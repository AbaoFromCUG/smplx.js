cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(smpl.js LANGUAGES CXX)

# ---- Dependencies ----
# Add C++ dependencies through CPM.cmake. See https://github.com/TheLartians/CPM.cmake for more info.
include(cmake/CPM.cmake)

# Update transitive dependencies to more recent versions (this has to be done before adding derived projects)
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.6.0")
CPMAddPackage("gh:TheLartians/Glue@1.5.1")

# Format.cmake is used to run clang-format
CPMAddPackage("gh:TheLartians/Format.cmake@1.8.1")

# EmGlue is used to create the TypeScript declarations and the JavaScript bindings
CPMAddPackage("gh:TheLartians/EmGlue@0.6.1")

CPMAddPackage(
    NAME Eigen
    VERSION 3.3.9
    URL https://gitlab.com/libeigen/eigen/-/archive/3.3.9/eigen-3.3.9.tar.gz # Eigen's CMakelists are not intended for library use
    DOWNLOAD_ONLY YES
)
if(Eigen_ADDED)
    add_library(Eigen3::Eigen INTERFACE IMPORTED)
    target_include_directories(Eigen3::Eigen INTERFACE ${Eigen_SOURCE_DIR})
endif()
CPMAddPackage(
    NAME zlib
    GITHUB_REPOSITORY madler/zlib
    GIT_TAG v1.3.1
    OPTIONS "ZLIB_BUILD_EXAMPLES OFF RENAME_ZCONF OFF"
)

if(zlib_ADDED)
    set_target_properties(zlib PROPERTIES PREFIX "_")
    add_library(ZLIB::ZLIB ALIAS zlibstatic)
endif()

CPMAddPackage(
    NAME smplxpp
    GITHUB_REPOSITORY AbaoFromCUG/smplxpp
    OPTIONS "SMPLX_BUILD_EXAMPLE OFF" "SMPLX_BUILD_VIEWER OFF" "SMPLX_USE_SYSTEM_EIGEN ON"
    GIT_TAG 47e871cbf7950f81fc51e6066755ee53fef96674
)

execute_process(COMMAND em++ --cflags OUTPUT_VARIABLE EM_CFLAGS)

# ---- Create library ----

add_executable(smplx src/binding.cpp)

target_link_options(
    smplx
    PRIVATE
    -sMODULARIZE
    # -sFORCE_FILESYSTEM
    -sALLOW_MEMORY_GROWTH
    "-sEXPORTED_RUNTIME_METHODS=['FS']"
    --bind
    --emit-tsd
    $<TARGET_FILE_DIR:smplx>/smplx.d.ts
)
include_directories(${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
target_include_directories(smplx PRIVATE "${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES}")
set_target_properties(
    smplx
    PROPERTIES CXX_STANDARD 17
               OUTPUT_NAME smplx
               COMPILE_FLAGS "${EM_CFLAGS}"
)
target_link_libraries(smplx PRIVATE smplxpp embind)

add_custom_command(
    TARGET smplx
    COMMENT "Copy wasm module"
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_LIST_DIR}/dist/
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/smplx.js ${CMAKE_CURRENT_LIST_DIR}/dist/index.js
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/smplx.d.ts ${CMAKE_CURRENT_LIST_DIR}/dist/index.d.ts
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/smplx.wasm ${CMAKE_CURRENT_LIST_DIR}/dist/
)
