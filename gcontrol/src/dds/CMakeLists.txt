add_library(gcontrol_dds g_dds_handler.cpp)
add_library(gcontrol::dds ALIAS gcontrol_dds)

target_include_directories(gcontrol_dds
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(gcontrol_dds
    PUBLIC
        zp1::log
        zp1::idl
)

target_compile_features(gcontrol_dds PUBLIC cxx_std_20)
