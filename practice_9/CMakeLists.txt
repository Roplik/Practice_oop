cmake_minimum_required(VERSION 4.1)



project("practice_9" CXX)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable("${PROJECT_NAME}")

target_link_libraries("${PROJECT_NAME}" PRIVATE unofficial::sqlite3::sqlite3 fmt::fmt)

target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_23)

target_sources("${PROJECT_NAME}" PRIVATE "main.cpp"
PRIVATE FILE_SET "modules" TYPE CXX_MODULES FILES
    "tools.cppm")