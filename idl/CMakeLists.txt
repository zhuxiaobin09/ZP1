set(ZP1_IDL_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

idlcxx_generate(
    TARGET zp1_idl_types
    FILES GControlMessage.idl
    BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_DIR ${ZP1_IDL_GENERATED_DIR}
)

add_library(zp1_idl STATIC ${ZP1_IDL_GENERATED_DIR}/GControlMessage.cpp)
add_library(zp1::idl ALIAS zp1_idl)

add_dependencies(zp1_idl zp1_idl_types_generate)

target_link_libraries(zp1_idl
    PUBLIC
        CycloneDDS-CXX::ddscxx
        zp1_idl_types
)

target_include_directories(zp1_idl
    PUBLIC
        ${ZP1_IDL_GENERATED_DIR}
)

target_compile_features(zp1_idl PUBLIC cxx_std_20)
