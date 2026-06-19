cmake_minimum_required(VERSION 4.1)



project("practice_9" CXX)


find_package(fmt CONFIG REQUIRED)
find_package(cereal CONFIG REQUIRED)

add_executable("${PROJECT_NAME}")



target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_23)

target_sources("${PROJECT_NAME}" PRIVATE "main.cpp")

target_link_libraries("${PROJECT_NAME}" PRIVATE cereal::cereal)