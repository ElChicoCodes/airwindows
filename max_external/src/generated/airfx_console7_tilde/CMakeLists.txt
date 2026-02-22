include(${MAX_SDK_BASE_DIR}/script/max-pretarget.cmake)

include_directories(
    "${MAX_SDK_INCLUDES}"
    "${MAX_SDK_MSP_INCLUDES}"
    "${MAX_SDK_JIT_INCLUDES}"
)

add_library(
    ${PROJECT_NAME}
    MODULE
    "airfx_console7_tilde.cpp"
    "Console7ChannelEngine.hpp"
)

set(${PROJECT_NAME}_EXTERN_OUTPUT_NAME "airfx.console7~" CACHE STRING "" FORCE)
mark_as_advanced(${PROJECT_NAME}_EXTERN_OUTPUT_NAME)

include(${MAX_SDK_BASE_DIR}/script/max-posttarget.cmake)
