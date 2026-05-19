cmake_minimum_required(VERSION 3.28)
project(PlantsVsZombies LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

#set(JSON_BuildTests OFF CACHE INTERNAL "")
#set(JSON_Install OFF CACHE INTERNAL "")

FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)


file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(PlantsVsZombies ${SOURCES})

add_custom_command(TARGET PlantsVsZombies POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:PlantsVsZombies>/assets
)

add_custom_command(TARGET PlantsVsZombies POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/storage
    $<TARGET_FILE_DIR:PlantsVsZombies>/storage
)

target_compile_options(PlantsVsZombies PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Wpedantic
    $<$<CONFIG:DEBUG>:-g -fsanitize=address>
    $<$<CONFIG:RELEASE>:-O2>
)
target_link_options(PlantsVsZombies PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address>)

target_link_libraries(PlantsVsZombies PRIVATE
SFML::Graphics
SFML::Audio
nlohmann_json::nlohmann_json)
target_include_directories(PlantsVsZombies PRIVATE include)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PlantsVsZombies)
