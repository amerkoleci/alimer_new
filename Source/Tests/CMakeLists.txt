if (NOT ALIMER_TESTS)
    return ()
endif ()

file (GLOB_RECURSE TEST_SOURCE_CODE RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.cpp)

group_sources()

add_executable(Alimer.Tests ${TEST_SOURCE_CODE})
target_link_libraries(Alimer.Tests PRIVATE Alimer doctest)
set_property(TARGET Alimer.Tests PROPERTY FOLDER "Tests")
