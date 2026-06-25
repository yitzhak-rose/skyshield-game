file (GLOB_RECURSE GAME_RESOURCE_FILES CONFIGURE_DEPENDS
    LIST_DIRECTORIES false
    RELATIVE ${CMAKE_CURRENT_LIST_DIR}
    data/*
    fonts/*
    images/*
    sounds/*
)

foreach (GAME_RESOURCE_FILE ${GAME_RESOURCE_FILES})
    configure_file (${GAME_RESOURCE_FILE} ${CMAKE_BINARY_DIR}/${GAME_RESOURCE_FILE} COPYONLY)
endforeach ()

find_package (SFML 3.0 COMPONENTS Audio REQUIRED)
target_link_libraries (${CMAKE_PROJECT_NAME} SFML::Audio)
