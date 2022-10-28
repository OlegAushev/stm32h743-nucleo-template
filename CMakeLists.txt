# STM32 Minimal CMake project for C/C++ projects
cmake_minimum_required(VERSION 3.12)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(gcc-arm-none-eabi)

project(stm32h743-nucleo-template VERSION 0.1)
enable_language(C CXX ASM)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/check-git)
include(CheckGit)
CheckGitSetup()

################################################################################
# Microcontroller Information
set(mcu_devboard STM32H7xx_Nucleo)
set(mcu_family STM32H7xx)
set(mcu_model STM32H743xx)
set(cpu_parameters
	-mcpu=cortex-m7
	-mthumb
	-mfpu=fpv5-sp-d16
	-mfloat-abi=hard)

set(mcu_startup_script ${PROJECT_SOURCE_DIR}/src/startup_stm32h743xx.s)
set(mcu_linker_script ${PROJECT_SOURCE_DIR}/STM32H743ZITx_FLASH.ld)

################################################################################
# Headers and Sources
set(stm32cubemx_incdirs
	${PROJECT_SOURCE_DIR}/drivers/${mcu_family}_HAL_Driver
	${PROJECT_SOURCE_DIR}/drivers/${mcu_family}_HAL_Driver/Inc
	${PROJECT_SOURCE_DIR}/drivers/${mcu_family}_HAL_Driver/Inc/Legacy
	${PROJECT_SOURCE_DIR}/drivers/CMSIS/Device/ST/${mcu_family}/Include
	${PROJECT_SOURCE_DIR}/drivers/CMSIS/Include
	${PROJECT_SOURCE_DIR}/drivers/CMSIS/DSP/Include
	${PROJECT_SOURCE_DIR}/drivers/BSP/Components
	${PROJECT_SOURCE_DIR}/drivers/BSP/${mcu_devboard}
)

set(project_incdirs
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

file(GLOB_RECURSE stm32cubemx_sources CONFIGURE_DEPENDS
	${PROJECT_SOURCE_DIR}/drivers/${mcu_family}_HAL_Driver/*.c
	${PROJECT_SOURCE_DIR}/drivers/BSP/*.c
)

file(GLOB_RECURSE project_sources CONFIGURE_DEPENDS
	${PROJECT_SOURCE_DIR}/include/*.c
	${PROJECT_SOURCE_DIR}/include/*.cpp
	${PROJECT_SOURCE_DIR}/src/*.c
	${PROJECT_SOURCE_DIR}/src/*.cpp
)

################################################################################
# Executable
set(executable ${CMAKE_PROJECT_NAME})

add_executable(${executable}
	${stm32cubemx_sources}
	${project_sources}
	${mcu_startup_script}
	${CMAKE_BINARY_DIR}/generated/git_version.cpp
)

target_compile_definitions(${executable} PRIVATE
	$<$<CONFIG:Debug>:DEBUG>
	$<$<CONFIG:Debug>:USE_FULL_ASSERT>
	${mcu_model}
	USE_HAL_DRIVER
)

target_include_directories(${executable} SYSTEM PRIVATE
	${stm32cubemx_incdirs}
)

target_include_directories(${executable} PRIVATE
	${project_incdirs}
)

add_dependencies(${executable} git-version)

################################################################################
# Compiler Options and Features
target_compile_options(${executable} PRIVATE
	${cpu_parameters}
	-Wall
	-Wextra
	-Wpedantic
	-Wshadow
	-Wdouble-promotion
	-Wformat=2 -Wformat-truncation
	-Wundef
	-fno-common
	-Wno-unused-parameter
	$<$<COMPILE_LANGUAGE:CXX>:
		-Wconversion
		-Wno-volatile
		-Wold-style-cast
		-Wuseless-cast
		-Wsuggest-override>
	$<$<CONFIG:Debug>:-O0 -g3 -ggdb>
	$<$<CONFIG:Release>:-O3 -g0>
)

target_compile_features(${executable} PRIVATE
	c_std_11
	cxx_std_20
)

################################################################################
# Linker Options
target_link_options(${executable} PRIVATE
	-T${mcu_linker_script}
	${cpu_parameters}
	-Wl,-Map=${CMAKE_PROJECT_NAME}.map
	--specs=nosys.specs
	-Wl,--start-group
	-lc
	-lm
	-lstdc++
	-Wl,--end-group
	-Wl,--print-memory-usage
	-u_printf_float			# allows to use printf with float
)

target_link_directories(${executable} PRIVATE
	${PROJECT_SOURCE_DIR}/drivers/CMSIS/DSP/Lib/GCC
)

target_link_libraries(${executable}
	libarm_cortexM7lfsp_math.a
)

################################################################################
# Misc Options and Post-build Tasks
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_custom_command(TARGET ${executable} POST_BUILD
	COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${executable}>
	COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${executable}> ${executable}.hex
	COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${executable}> ${executable}.bin
)

if (${DUMP_ASM})
	add_custom_command(TARGET ${executable} POST_BUILD
		COMMAND ${CMAKE_OBJDUMP} -D $<TARGET_FILE:${executable}> > ${executable}.s)
endif()


