include(FetchContent)

FetchContent_Declare(
    cpp_httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.18.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(cpp_httplib)

add_library(gcontrol_http g_http_handler.cpp)
add_library(gcontrol::http ALIAS gcontrol_http)

target_include_directories(gcontrol_http
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(gcontrol_http
    PUBLIC
        zp1::log
        httplib::httplib
)

target_compile_features(gcontrol_http PUBLIC cxx_std_20)
