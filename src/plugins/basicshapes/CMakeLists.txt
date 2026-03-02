# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

add_library(libbasicshapesformplugin)
target_sources(
    libbasicshapesformplugin
    PRIVATE
        triangle/triangle.hpp
        triangle/triangle.cpp
        triangle/trianglefactory.cpp
        triangle/trianglefactory.hpp
        triangle/triangledeserializer.cpp
        triangle/triangledeserializer.hpp
        triangle/triangleserializer.cpp
        triangle/triangleserializer.hpp
        xbox/xboxdeserializer.cpp
        xbox/xboxdeserializer.hpp
        xbox/xboxserializer.cpp
        xbox/xboxserializer.hpp
        xbox/xboxfactory.cpp
        xbox/xboxfactory.hpp
        xbox/xbox.cpp
        xbox/xbox.hpp
        basicshapespluginutils.hpp
        basicshapespluginutils.cpp
)
generate_export_header(libbasicshapesformplugin BASE_NAME libbasicshapesformplugin)
ecm_qt_declare_logging_category(libbasicshapesformplugin HEADER basicshapesformplugin_debug.h IDENTIFIER BASICSHAPES_FORM_LOG CATEGORY_NAME org.kde.drawy.basicshapesformplugin
    DESCRIPTION "Drawy basic shapes form plugin" EXPORT DRAWY
)

target_link_libraries(libbasicshapesformplugin libdrawygui)

set_target_properties(
    libbasicshapesformplugin
    PROPERTIES
        OUTPUT_NAME
            standardformplugin
        VERSION
            ${DRAWY_LIB_VERSION}
        SOVERSION
            ${DRAWY_LIB_SOVERSION}
)

install(
    TARGETS
        libbasicshapesformplugin
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
    LIBRARY
        NAMELINK_SKIP
)

###

kcoreaddons_add_plugin(basicshapesform_plugin INSTALL_NAMESPACE drawypluginforms)

target_sources(
    basicshapesform_plugin
    PRIVATE
        basicshapesplugin.hpp
        basicshapesplugin.cpp
)
target_link_libraries(basicshapesform_plugin libbasicshapesformplugin)

###
if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()
