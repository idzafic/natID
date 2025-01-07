cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME samoExe)	#Unijeti naziv solution-a

project(${PROJECT_NAME})

#dodaj sve cpp fajlove potrebne za implmentaciju
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp) 
