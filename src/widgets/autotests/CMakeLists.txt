# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

macro(add_drawy_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.hpp TEST_NAME ${_name}
        LINK_LIBRARIES Qt::Test libdrawywidgets
    )
endmacro()

add_drawy_unittest(iconmanagertest.cpp)
add_drawy_unittest(drawycommandlineparsertest.cpp)
add_drawy_unittest(propertytest.cpp)
add_drawy_unittest(itemtest.cpp)
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
