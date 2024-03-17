file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.c")
add_executable(Main ${SOURCE_FILES})

if(WIN32)
    target_link_libraries(Main wsock32 ws2_32)
endif()