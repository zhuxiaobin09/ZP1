add_library(gcontrol_http_config_routes
    gcontrol_route.cpp
    host_route.cpp
    log_route.cpp
)
add_library(gcontrol::http_config_routes ALIAS gcontrol_http_config_routes)

target_include_directories(gcontrol_http_config_routes
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../..
)

target_link_libraries(gcontrol_http_config_routes
    PUBLIC
        gcontrol::http_common
)

target_compile_features(gcontrol_http_config_routes PUBLIC cxx_std_20)
