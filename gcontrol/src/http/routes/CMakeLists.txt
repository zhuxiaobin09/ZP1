add_subdirectory(config)

add_library(gcontrol_http_routes
    route_registry.cpp
    config/config_route_registry.cpp
)
add_library(gcontrol::http_routes ALIAS gcontrol_http_routes)

target_include_directories(gcontrol_http_routes
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(gcontrol_http_routes
    PUBLIC
        gcontrol::http_config_routes
        gcontrol::http_common
)

target_compile_features(gcontrol_http_routes PUBLIC cxx_std_20)
