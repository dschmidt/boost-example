project(boost-example)
cmake_minimum_required(VERSION 2.8)

include( GNUInstallDirs )
find_package(Boost COMPONENTS system)
find_package(Threads)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)


# link to boost
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${Boost_INCLUDE_DIR}
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
