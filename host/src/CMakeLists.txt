add_subdirectory(dds)

add_executable(host main.cpp)

target_include_directories(host
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(host
    PRIVATE
        host::dds
)

target_compile_features(host PRIVATE cxx_std_20)
