cmake_minimum_required(VERSION 3.17)
project(test-multiprocess)

add_compile_options(-std=gnu++11)
add_compile_options(-Wdeprecated-declarations)
SET(CMAKE_BUILD_TYPE Release CACHE STRING
	"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
#	FORCE
	)

set(srcs_main "test-multiprocess.cpp")

add_executable(test-multiprocess ${srcs_main})

set (srcs_slave "test-slave.cpp")

add_executable(test-slave ${srcs_slave})
