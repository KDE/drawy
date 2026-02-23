# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

add_library(libstandardformplugin)
target_sources(
    libstandardformplugin
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
generate_export_header(libstandardformplugin BASE_NAME libstandardformplugin)
ecm_qt_declare_logging_category(libstandardformplugin HEADER standardformplugin_debug.h IDENTIFIER STANDARD_FORM_LOG CATEGORY_NAME org.kde.drawy.standardformplugin
    DESCRIPTION "Drawy standardformplugin" EXPORT DRAWY
)

target_link_libraries(libstandardformplugin libdrawygui)

set_target_properties(
    libstandardformplugin
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
        libstandardformplugin
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
    LIBRARY
        NAMELINK_SKIP
)

###

kcoreaddons_add_plugin(standardform_plugin INSTALL_NAMESPACE drawypluginforms)

target_sources(
    standardform_plugin
    PRIVATE
        basicshapesplugin.hpp
        basicshapesplugin.cpp
)
target_link_libraries(standardform_plugin libstandardformplugin)

###
if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()
