cmake_minimum_required(VERSION 4.2)
project(AVToolkit C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(ADW REQUIRED libadwaita-1)

set(CMAKE_C_STANDARD 11)

add_executable(AVToolkit src/main.c)

target_include_directories(AVToolkit PRIVATE ${GTK4_INCLUDE_DIRS} ${ADW_INCLUDE_DIRS})
target_link_libraries(AVToolkit PRIVATE ${GTK4_LIBRARIES} ${ADW_LIBRARIES})
target_compile_options(AVToolkit PRIVATE ${GTK4_CFLAGS_OTHER} ${ADW_CFLAGS_OTHER})

target_link_directories(AVToolkit PRIVATE ${GTK4_LIBRARY_DIRS})

