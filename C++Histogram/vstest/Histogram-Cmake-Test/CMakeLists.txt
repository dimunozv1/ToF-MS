cmake_minimum_required(VERSION 3.16)
project(Histogram LANGUAGES CXX)


option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(CMAKE_BUILD_TYPE Debug)
# Copy the font file to the build directory
configure_file("Roboto-Regular.ttf" "${CMAKE_SOURCE_DIR}/Roboto-Regular.ttf" COPYONLY)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(Histogram main-parallel.cpp)
target_link_libraries(Histogram PRIVATE sfml-graphics)
target_compile_features(Histogram PRIVATE cxx_std_17)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
target_include_directories(Histogram PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}  # Include the current source directory
)
if(WIN32)
    add_custom_command(
        TARGET Histogram
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Histogram>
        VERBATIM)
endif()

install(TARGETS Histogram)