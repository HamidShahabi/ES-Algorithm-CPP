cmake_minimum_required(VERSION "3.10.2")
project(final_project CXX)

set(SYSTEMC_PATH /usr/local/systemc-2.3.3)

include_directories(${SYSTEMC_PATH}/include)

find_library(systemc systemc ${SYSTEMC_PATH}/lib-linux64)

link_directories(${SYSTEMC_PATH}/lib-linux64)

add_executable(
    final_project
    main_systemc.cc
    ga_module.hh
    population_updator_module.hh
)

target_link_libraries(final_project PUBLIC systemc)