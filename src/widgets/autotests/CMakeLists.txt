# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

macro(add_drawy_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.h TEST_NAME ${_name}
        LINK_LIBRARIES Qt::Test libdrawywidgets
    )
endmacro()

add_drawy_unittest(iconmanagertest.cpp)
add_drawy_unittest(drawycommandlineparsertest.cpp)
add_drawy_unittest(propertytest.cpp)
add_drawy_unittest(itemtest.cpp)
