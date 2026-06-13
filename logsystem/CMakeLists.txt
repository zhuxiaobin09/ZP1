add_library(zp1_log logger.cpp)
add_library(zp1::log ALIAS zp1_log)

target_include_directories(zp1_log
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(zp1_log PUBLIC cxx_std_20)
