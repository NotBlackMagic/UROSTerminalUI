# This file was automatically generated and updated by RASC

#source directories
file(GLOB_RECURSE Source_Files 
"${CMAKE_CURRENT_SOURCE_DIR}/ra/*.c"
"${CMAKE_CURRENT_SOURCE_DIR}/ra/*.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/ra_gen/*.c"
"${CMAKE_CURRENT_SOURCE_DIR}/ra_gen/*.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


SET(ALL_FILES ${Source_Files})

add_executable(${PROJECT_NAME}.elf ${ALL_FILES})

target_include_directories(${PROJECT_NAME}.elf
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/arm/CMSIS_5/CMSIS/Core/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/fsp/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/fsp/inc/api
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/fsp/inc/instances
    ${CMAKE_CURRENT_SOURCE_DIR}/ra/tes/dave2d/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ra_cfg/fsp_cfg
    ${CMAKE_CURRENT_SOURCE_DIR}/ra_cfg/fsp_cfg/bsp
    ${CMAKE_CURRENT_SOURCE_DIR}/ra_gen
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

