# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

macro(add_standardformplugin_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.hpp TEST_NAME ${_name}
        LINK_LIBRARIES Qt::Test libstandardformplugin
    )
endmacro()

add_standardformplugin_unittest(standardpluginutilstest.cpp)
add_standardformplugin_unittest(xboxitemtest.cpp)
add_standardformplugin_unittest(triangleitemtest.cpp)
