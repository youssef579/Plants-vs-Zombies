cmake_minimum_required(VERSION 3.28)
project(PlantsVsZombies LANGUAGES CXX)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

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

add_executable(main ${SOURCES})

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:main>/assets
)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/storage
    $<TARGET_FILE_DIR:main>/storage
)

target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE
SFML::Graphics
SFML::Audio
nlohmann_json::nlohmann_json)
target_include_directories(main PRIVATE include)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
