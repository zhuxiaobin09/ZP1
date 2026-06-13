add_library(host_dds h_dds_handler.cpp)
add_library(host::dds ALIAS host_dds)

target_include_directories(host_dds
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(host_dds
    PUBLIC
        zp1::log
        zp1::idl
)

target_compile_features(host_dds PUBLIC cxx_std_20)
