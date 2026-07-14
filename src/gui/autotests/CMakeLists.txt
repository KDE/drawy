# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_definitions(-DDRAWY_GUI_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
add_definitions(-DDRAWY_GUI_BINARY_DATA_DIR="${CMAKE_CURRENT_BINARY_DIR}/data")

if(ENABLE_PCH)
    file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp CONTENT "/*empty file*/")
    file(
        GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libdrawygui_pch.h
        CONTENT
            "#pragma once\n
        #include <QObject>\n
        #include <QTest>\n
        #include <QJsonDocument>\n
        #include <QJsonObject>\n
        #include <QSignalSpy>\n"
    )
    add_library(libdrawygui_test_pch STATIC ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp)
    target_precompile_headers(libdrawygui_test_pch PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/libdrawygui_pch.h)
    target_link_libraries(
        libdrawygui_test_pch
        libdrawygui
        drawyguihelper
        Qt::Test
    )
endif()

add_library(drawyguihelper STATIC)
target_sources(
    drawyguihelper
    PRIVATE
        drawy_autotest_helper.cpp
        drawy_autotest_helper.hpp
)
target_link_libraries(
    drawyguihelper
    Qt::Test
    Qt::Core
)

macro(add_drawy_gui_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.hpp TEST_NAME ${_name}
        LINK_LIBRARIES Qt::Test libdrawygui drawyguihelper Qt::Widgets
    )
endmacro()

add_drawy_gui_unittest(propertytest.cpp)
add_drawy_gui_unittest(itemutilstest.cpp)
add_drawy_gui_unittest(arrowitemtest.cpp)
add_drawy_gui_unittest(ellipseitemtest.cpp)
add_drawy_gui_unittest(freeformitemtest.cpp)
add_drawy_gui_unittest(groupitemtest.cpp)
add_drawy_gui_unittest(lineitemtest.cpp)
add_drawy_gui_unittest(polygonitemtest.cpp)
add_drawy_gui_unittest(rectangleitemtest.cpp)
add_drawy_gui_unittest(textitemtest.cpp)
add_drawy_gui_unittest(arrowutilstest.cpp)
add_drawy_gui_unittest(diamonditemtest.cpp)
add_drawy_gui_unittest(pluginformutilstest.cpp)
add_drawy_gui_unittest(imageitemtest.cpp)
add_drawy_gui_unittest(customelementsmanagertest.cpp)
add_drawy_gui_unittest(customelementsutilstest.cpp)
add_drawy_gui_unittest(customelementtest.cpp)
