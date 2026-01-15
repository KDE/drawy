# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_definitions(-DDRAWY_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
add_definitions(-DDRAWY_BINARY_DATA_DIR="${CMAKE_CURRENT_BINARY_DIR}/data")

if(ENABLE_PCH)
    file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp CONTENT "/*empty file*/")
    file(
        GENERATE OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/libdrawywidgets_pch.h
        CONTENT
            "#pragma once\n
        #include <QObject>\n
        #include <QTest>\n
        #include <QJsonDocument>\n
        #include <QJsonObject>\n
        #include <QSignalSpy>\n"
    )
    add_library(libdrawywidgets_test_pch STATIC ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp)
    target_precompile_headers(libdrawywidgets_test_pch PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/libdrawywidgets_pch.h)
    target_link_libraries(
        libdrawywidgets_test_pch
        libdrawywidgets
        drawywidgetshelper
        Qt::Test
    )
endif()

add_library(drawywidgetshelper STATIC)
target_sources(
    drawywidgetshelper
    PRIVATE
        drawy_autotest_helper.cpp
        drawy_autotest_helper.hpp
)
target_link_libraries(
    drawywidgetshelper
    Qt::Test
    Qt::Core
)

macro(add_drawy_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.hpp TEST_NAME ${_name}
        LINK_LIBRARIES Qt::Test libdrawywidgets drawywidgetshelper
    )
endmacro()

add_drawy_unittest(iconmanagertest.cpp)
add_drawy_unittest(drawycommandlineparsertest.cpp)
add_drawy_unittest(propertytest.cpp)
add_drawy_unittest(itemutilstest.cpp)
add_drawy_unittest(saveasjobtest.cpp)
add_drawy_unittest(serializejobtest.cpp)
add_drawy_unittest(arrowitemtest.cpp)
add_drawy_unittest(ellipseitemtest.cpp)
add_drawy_unittest(freeformitemtest.cpp)
add_drawy_unittest(groupitemtest.cpp)
add_drawy_unittest(lineitemtest.cpp)
add_drawy_unittest(polygonitemtest.cpp)
add_drawy_unittest(rectangleitemtest.cpp)
add_drawy_unittest(textitemtest.cpp)
add_drawy_unittest(buttonactionswidgettest.cpp)
add_drawy_unittest(stokestylewidgettest.cpp)
add_drawy_unittest(arrowutilstest.cpp)
add_drawy_unittest(opacitywidgettest.cpp)
add_drawy_unittest(propertymanagertest.cpp)
add_drawy_unittest(rectangletooltest.cpp)
add_drawy_unittest(arrowtooltest.cpp)
add_drawy_unittest(ellipsetooltest.cpp)
add_drawy_unittest(linetooltest.cpp)
