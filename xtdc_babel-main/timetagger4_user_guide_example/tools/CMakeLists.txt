cmake_minimum_required(VERSION 3.16)
set(CRONO_TARGET_NAME "ejectutable")
project(${CRONO_TARGET_NAME})
set(OUTPUT_DIR "C:/Users/diana//OneDrive/Documentos/Documentos/documentos_2024/Fisica/Tesis/Code/ToF-MS/Run_TOF-MS_programs/")
option(BUILD_SHARED_LIBS "Build shared libraries" OFF) #SFML
set(CMAKE_BUILD_TYPE Debug)
# _____________________________________________________________________________
# Build Windows/Linux `xtdc4_ugex.exe/xtdc4_ugex` executable, 
# for (Debug/Release) configurations.
# _____________________________________________________________________________

# General Validations and Configurations ______________________________________
# cd indirection from /tools to the project source code, "." if no shift
set(PROJ_SRC_INDIR ../..)  

# Get the platform and architecure ____________________________________________
# Check supported architecture, and set `CRONO_GEN_ARCH` to `x86_64` for all
# variations of `x64` platforms, and to `x86` for all variations of `x86` 
# platforms.
# Set `CRONO_GEN_PLATFORM` to either `windows` or `linux`
SET(CRONO_GEN_PLATFORM "windows")
IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CRONO_GEN_PLATFORM "linux")
ENDIF()

list(APPEND x64_archs Win64 AMD64 x86_64)
IF(CMAKE_SYSTEM_PROCESSOR IN_LIST x64_archs)
        set(CRONO_GEN_ARCH "x86_64")
ENDIF()


# Include directories paths ___________________________________________________
set(CRONO_DEP_PKG_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_SRC_INDIR}/include)
include_directories(${CRONO_TARGET_NAME} PRIVATE  ${CRONO_DEP_PKG_INC_DIR})

# Link to xtdc4_driver library ________________________________________
set(CRONO_DEP_PKG_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_SRC_INDIR}/lib/x64)
link_directories(${CRONO_DEP_PKG_LIB_DIR})

# Set output directory_________________________________________________________
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

# Validate that directories exist _____________________________________________
IF(     NOT EXISTS "${CRONO_DEP_PKG_LIB_DIR}" 
     OR NOT EXISTS "${CRONO_DEP_PKG_INC_DIR}"
) 
        message(FATAL_ERROR 
                "Crono: Please copy driver installation folder <TimeTagger4/driver> to the project root.")
ENDIF()
IF(     NOT EXISTS "${OUTPUT_DIR}" 
) 
        message(FATAL_ERROR 
                "Output folder was not found")
ENDIF()
IF(     NOT EXISTS "${PROJ_SRC_INDIR}/src/Roboto-Regular.ttf" 
     
) 
        message(FATAL_ERROR 
                "Font: Please copy Roboto-Regular.ttf to the source directory.")
ENDIF()               
# Add the target  _____________________________________________________________
add_executable(${CRONO_TARGET_NAME} 
        ${PROJ_SRC_INDIR}/src/TOF_Time_Tagger.cpp)

# Windows-specific Configurations _____________________________________________
IF ( CRONO_GEN_PLATFORM  STREQUAL "windows")
    add_library(xtdc4_driver_64 SHARED IMPORTED GLOBAL)
    set_target_properties(xtdc4_driver_64 PROPERTIES IMPORTED_IMPLIB 
            xtdc4_driver_64.lib)  
    target_link_libraries(${CRONO_TARGET_NAME} PUBLIC xtdc4_driver_64) #This used to be plain signature just in case. 
    
    # Add compilation flags for Windows
    target_compile_options(${CRONO_TARGET_NAME} PRIVATE -Wall -g)  # Add -Wall and -g flags for warnings and debugging information
        #---------------------------------------SFML---------------------------------
    include(FetchContent)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
    FetchContent_MakeAvailable(SFML)
    target_link_libraries(${CRONO_TARGET_NAME} PRIVATE sfml-graphics)
    target_compile_features(${CRONO_TARGET_NAME} PRIVATE cxx_std_17)
    

    if(WIN32)
    add_custom_command(
        TARGET ${CRONO_TARGET_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${CRONO_TARGET_NAME}>
        VERBATIM)
    endif()
    install(TARGETS ${CRONO_TARGET_NAME})

# Linux-specific Configurations _______________________________________________
ELSEIF (CRONO_GEN_PLATFORM STREQUAL "linux") 
    # Compilation options _____________________________________________________
    add_compile_options(-fPIC -Wall $<$<CONFIG:Debug>:-g>)
    add_compile_definitions($<$<CONFIG:Debug>:CRONO_DEBUG_ENABLED>)

    # Link to xtdc4_driver library ____________________________________________
    target_link_libraries(${CRONO_TARGET_NAME} libxtdc4_driver.a)
ENDIF()
