cmake_minimum_required(VERSION 3.2)

find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_compile_options(-std=c++11)

file(GLOB unittest_list unittest/*cpp)
foreach(f ${unittest_list})
    get_filename_component(exe ${f} NAME_WE)
    add_executable(${exe} ${f})
endforeach()

