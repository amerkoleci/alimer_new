# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb")

# ShaderMakeBlob
add_subdirectory(ShaderMakeBlob)

# fmt
add_subdirectory(fmt EXCLUDE_FROM_ALL)
set_property(TARGET fmt PROPERTY FOLDER "ThirdParty")
install (DIRECTORY fmt/include/fmt DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/ThirdParty)

# spdlog
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Use external fmt library instead of bundled" FORCE)
set(SPDLOG_USE_STD_FORMAT OFF CACHE BOOL "Use external fmt library instead of bundled" FORCE)
if (NOT ALIMER_EXCEPTIONS_ENABLED)
    set(SPDLOG_NO_EXCEPTIONS ON CACHE BOOL "Compile with -fno-exceptions. Call abort() on any spdlog exceptions" FORCE)
endif()

add_subdirectory(spdlog EXCLUDE_FROM_ALL)
set_property(TARGET spdlog PROPERTY FOLDER "ThirdParty")
install (DIRECTORY spdlog/include/spdlog DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/ThirdParty)

# rapidjson
add_library(rapidjson INTERFACE)
set(RAPIDJSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include)
target_include_directories(rapidjson INTERFACE ${RAPIDJSON_INCLUDE_DIR})
install (DIRECTORY ${RAPIDJSON_INCLUDE_DIR}/rapidjson
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/ThirdParty
         FILES_MATCHING PATTERN *.h
)

# unordered_dense
add_subdirectory(unordered_dense)
set_property(TARGET unordered_dense PROPERTY FOLDER "ThirdParty")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/unordered_dense/include/ankerl
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/ThirdParty
)

# lz4
add_subdirectory(lz4)
set_property(TARGET lz4 PROPERTY FOLDER "ThirdParty")

# SDL3
if (NOT (EMSCRIPTEN OR ANDROID OR WINDOWS_STORE))
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_LIBC ON CACHE BOOL "" FORCE)
    set(SDL_TIMERS ON CACHE BOOL "" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
    set(SDL_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
    set(SDL_DISABLE_INSTALL_DOCS ON CACHE BOOL "" FORCE)
    # Disable everything we don't care about.
    set(SDL_ATOMIC OFF CACHE BOOL "" FORCE)
    set(SDL_AUDIO OFF CACHE BOOL "" FORCE)
    set(SDL_RENDER OFF CACHE BOOL "" FORCE)
    set(SDL_HAPTIC OFF CACHE BOOL "" FORCE)
    set(SDL_POWER OFF CACHE BOOL "" FORCE)
    set(SDL_FILE OFF CACHE BOOL "" FORCE)
    set(SDL_CPUINFO OFF CACHE BOOL "" FORCE)
    set(SDL_FILESYSTEM OFF CACHE BOOL "" FORCE)
    set(SDL_SENSOR OFF CACHE BOOL "" FORCE)
    set(SDL_LOCALE OFF CACHE BOOL "" FORCE)
    set(SDL_MISC OFF CACHE BOOL "" FORCE)
    set(SDL_CAMERA OFF CACHE BOOL "" FORCE)
    set(SDL_GPU OFF CACHE BOOL "" FORCE)
    set(SDL_DIALOG OFF CACHE BOOL "" FORCE)
    add_subdirectory(SDL EXCLUDE_FROM_ALL)
    set_property(TARGET "SDL3-static" PROPERTY FOLDER "ThirdParty")
    if (SDL_SHARED)
        set_property(TARGET "SDL_uclibc" PROPERTY FOLDER "ThirdParty")
    endif ()
endif()

# miniaudio
add_library(miniaudio INTERFACE)
target_include_directories(miniaudio INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/miniaudio")
target_sources(miniaudio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/miniaudio/miniaudio.h)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/miniaudio
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/ThirdParty
)

if (ALIMER_TESTS)
    add_library(doctest INTERFACE)
    set(DOCTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doctest)
    target_sources(doctest INTERFACE 
        ${DOCTEST_INCLUDE_DIR}/doctest.h
    )
    target_include_directories(doctest SYSTEM INTERFACE ${DOCTEST_INCLUDE_DIR})
endif ()
