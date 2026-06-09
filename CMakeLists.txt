cmake_minimum_required (VERSION 3.26)

project (oop2_project)
set (MY_AUTHORS "YOUR-NAME-HERE")

include (cmake/CompilerSettings.cmake)

add_executable (${CMAKE_PROJECT_NAME})

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address>)
if (NOT MSVC)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address>)
endif()

add_subdirectory (include)
add_subdirectory (src)
add_subdirectory (resources)

include (cmake/Zip.cmake)

# Use shared SFML libraries on all platforms
set (SFML_STATIC_LIBRARIES OFF)

# Windows: point CMake to the local SFML installation
if (WIN32)
    set (SFML_LOCATION "C:/SFML/SFML-3.0.0")
    set (SFML_DIR "${SFML_LOCATION}/lib/cmake/SFML")
endif()

# Graphics is required on all platforms
find_package (SFML 3.0 COMPONENTS Graphics REQUIRED)
target_link_libraries (${CMAKE_PROJECT_NAME} SFML::Graphics)

# Main and Audio are Windows-only (Mac gets them via the system/Homebrew SFML)
if (WIN32)
    find_package (SFML 3.0 COMPONENTS Main Audio REQUIRED)
    target_link_libraries (${CMAKE_PROJECT_NAME} SFML::Main SFML::Audio)

    # Copy DLLs to output directory — kept inside WIN32 guard because
    # $<TARGET_RUNTIME_DLLS:...> expands to empty on macOS and breaks the build
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}>
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
            COMMAND_EXPAND_LISTS
    )
endif()

include (cmake/SFML.cmake)