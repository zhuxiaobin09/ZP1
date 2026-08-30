add_subdirectory(dds)
add_subdirectory(http)

add_executable(gcontrol main.cpp)

target_include_directories(gcontrol
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(gcontrol
    PRIVATE
        gcontrol::dds
        gcontrol::http
)

target_compile_features(gcontrol PRIVATE cxx_std_20)
