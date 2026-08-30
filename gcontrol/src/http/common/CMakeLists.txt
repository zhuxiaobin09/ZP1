add_library(gcontrol_http_common config_file_util.cpp)
add_library(gcontrol::http_common ALIAS gcontrol_http_common)

target_include_directories(gcontrol_http_common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(gcontrol_http_common
    PUBLIC
        zp1::log
        httplib::httplib
)

target_compile_features(gcontrol_http_common PUBLIC cxx_std_20)
