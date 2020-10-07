cmake_minimum_required(VERSION 3.10)

project(hello)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup(TARGETS)

find_package(LeapSDK 4.1.0 PATHS "${LeapSDK_DIR}/lib/cmake")

add_executable(glutsample GLUTSample.cpp ExampleConnection.c)

target_include_directories(glutsample PUBLIC ${Leap_ROOT_DIR}/include)

target_link_libraries(glutsample PRIVATE LeapSDK::LeapC)
target_link_libraries(glutsample PRIVATE CONAN_PKG::opencv)
target_link_libraries(glutsample PRIVATE CONAN_PKG::freeglut CONAN_PKG::glew CONAN_PKG::glm)

add_custom_command(TARGET glutsample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_PROPERTY:LeapSDK::LeapC,IMPORTED_LOCATION_RELEASE> 
                                                  $<TARGET_FILE_DIR:glutsample>)