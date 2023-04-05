cmake_minimum_required(VERSION 3.5)

project(wxWidgets_Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    app.h app.cpp
)

if (WIN32)
    set(wxWidgets_ROOT_DIR $ENV{WXWIN})
    set(wxWidgets_LIB_DIR  $ENV{WXWIN}/lib/vc_x64_lib)
    set(wxWidgets_USE_LIBS ON)
endif()

find_package(wxWidgets REQUIRED COMPONENTS net core base)

if (wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})

    message("S.O. : " ${CMAKE_SYSTEM_NAME})
    if (WIN32)
        message("Root Dir: " ${wxWidgets_ROOT_DIR} )
        message("Lib dir : " ${wxWidgets_LIB_DIR} )
        add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
    else(LINUX)
        message("wxWidgets package found!")
        add_executable(${PROJECT_NAME} ${SOURCES})
    endif()

    target_link_libraries (${PROJECT_NAME} PUBLIC ${wxWidgets_LIBRARIES})

    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
    message("Cmake completed successfully!")
else()
    message(FATAL_ERROR "wxWidgets not found!")
endif()
