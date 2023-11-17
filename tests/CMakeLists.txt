include("../cmake/GoogleTest.cmake")

add_executable(module_test
test.cpp
)

target_compile_features(module_test PUBLIC cxx_std_20)

target_link_libraries(module_test gtest module)
