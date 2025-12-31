# SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
#
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.16)

project(drawy VERSION 0.1 LANGUAGES CXX)

# Enable automatic handling of UI, MOC, and RCC files.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(KF_MIN_VERSION "6.19.0")
set(QT_REQUIRED_VERSION "6.9.0")
find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
include(ECMDeprecationSettings)
# TODO activate it when all code will be ported include(KDECompilerSettings NO_POLICY_SCOPE)
# TODO activate it kde_enable_exceptions()
include(KDEClangFormat)
include(KDEGitCommitHooks)
include(ECMPoQmTools)
include(GNUInstallDirs)
include(ECMQtDeclareLoggingCategory)
include(ECMPoQmTools)
include(KDEInstallDirs)
include(FeatureSummary)

ecm_set_disabled_deprecation_versions(QT 6.11.0  KF 6.21.0)

# Find Qt6
find_package(Qt6 ${QT_REQUIRED_VERSION} REQUIRED
    COMPONENTS
        Widgets
        LinguistTools
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS Crash)

file(
    GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES
    *.cpp
    *.hpp
)

kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

# ZSTD for compression
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZSTD REQUIRED libzstd)

# Combine translation files with the source files.
set(PROJECT_SOURCES
    src/canvas/canvas.cpp
    src/canvas/canvas.hpp
    src/command/commandhistory.cpp
    src/command/commandhistory.hpp
    src/command/command.hpp
    src/command/deselectcommand.cpp
    src/command/deselectcommand.hpp
    src/command/groupcommand.cpp
    src/command/groupcommand.hpp
    src/command/insertitemcommand.cpp
    src/command/insertitemcommand.hpp
    src/command/itemcommand.cpp
    src/command/itemcommand.hpp
    src/command/moveitemcommand.cpp
    src/command/moveitemcommand.hpp
    src/command/removeitemcommand.cpp
    src/command/removeitemcommand.hpp
    src/command/selectcommand.cpp
    src/command/selectcommand.hpp
    src/command/ungroupcommand.cpp
    src/command/ungroupcommand.hpp
    src/command/updatepropertycommand.cpp
    src/command/updatepropertycommand.hpp
    src/common/constants.hpp
    src/common/renderitems.cpp
    src/common/renderitems.hpp
    src/common/utils/compression.cpp
    src/common/utils/compression.hpp
    src/common/utils/math.hpp
    src/components/actionbar.cpp
    src/components/actionbar.hpp
    src/components/propertybar.cpp
    src/components/propertybar.hpp
    src/components/toolbar.cpp
    src/components/toolbar.hpp
    src/context/applicationcontext.cpp
    src/context/applicationcontext.hpp
    src/context/coordinatetransformer.cpp
    src/context/coordinatetransformer.hpp
    src/context/renderingcontext.cpp
    src/context/renderingcontext.hpp
    src/context/selectioncontext.cpp
    src/context/selectioncontext.hpp
    src/context/spatialcontext.cpp
    src/context/spatialcontext.hpp
    src/context/uicontext.cpp
    src/context/uicontext.hpp
    src/controller/controller.cpp
    src/controller/controller.hpp
    src/data-structures/cachegrid.cpp
    src/data-structures/cachegrid.hpp
    src/data-structures/orderedlist.cpp
    src/data-structures/orderedlist.hpp
    src/data-structures/quadtree.cpp
    src/data-structures/quadtree.hpp
    src/data-structures/quadtree.ipp
    src/event/event.cpp
    src/event/event.hpp
    src/iconmanager/iconmanager.cpp
    src/iconmanager/iconmanager.hpp
    src/item/arrow.cpp
    src/item/arrow.hpp
    src/item/ellipse.cpp
    src/item/ellipse.hpp
    src/item/factory/arrowfactory.cpp
    src/item/factory/arrowfactory.hpp
    src/item/factory/ellipsefactory.cpp
    src/item/factory/ellipsefactory.hpp
    src/item/factory/freeformfactory.cpp
    src/item/factory/freeformfactory.hpp
    src/item/factory/itemfactory.cpp
    src/item/factory/itemfactory.hpp
    src/item/factory/linefactory.cpp
    src/item/factory/linefactory.hpp
    src/item/factory/rectanglefactory.cpp
    src/item/factory/rectanglefactory.hpp
    src/item/factory/textfactory.cpp
    src/item/factory/textfactory.hpp
    src/item/freeform.cpp
    src/item/freeform.hpp
    src/item/group.cpp
    src/item/group.hpp
    src/item/item.cpp
    src/item/item.hpp
    src/item/line.cpp
    src/item/line.hpp
    src/item/polygon.cpp
    src/item/polygon.hpp
    src/item/rectangle.cpp
    src/item/rectangle.hpp
    src/item/text.cpp
    src/item/text.hpp
    src/keybindings/action.cpp
    src/keybindings/action.hpp
    src/keybindings/actionmanager.cpp
    src/keybindings/actionmanager.hpp
    src/keybindings/keybindmanager.cpp
    src/keybindings/keybindmanager.hpp
    src/properties/property.cpp
    src/properties/property.hpp
    src/properties/widgets/actionswidget.cpp
    src/properties/widgets/actionswidget.hpp
    src/properties/widgets/erasersizewidget.cpp
    src/properties/widgets/erasersizewidget.hpp
    src/properties/widgets/fontsizewidget.cpp
    src/properties/widgets/fontsizewidget.hpp
    src/properties/widgets/propertymanager.cpp
    src/properties/widgets/propertymanager.hpp
    src/properties/widgets/propertywidget.cpp
    src/properties/widgets/propertywidget.hpp
    src/properties/widgets/strokecolorwidget.cpp
    src/properties/widgets/strokecolorwidget.hpp
    src/properties/widgets/strokewidthwidget.cpp
    src/properties/widgets/strokewidthwidget.hpp
    src/serializer/loader.cpp
    src/serializer/loader.hpp
    src/serializer/serializer.cpp
    src/serializer/serializer.hpp
    src/tools/arrowtool.cpp
    src/tools/arrowtool.hpp
    src/tools/drawingtool.cpp
    src/tools/drawingtool.hpp
    src/tools/ellipsetool.cpp
    src/tools/ellipsetool.hpp
    src/tools/erasertool.cpp
    src/tools/erasertool.hpp
    src/tools/freeformtool.cpp
    src/tools/freeformtool.hpp
    src/tools/linetool.cpp
    src/tools/linetool.hpp
    src/tools/movetool.cpp
    src/tools/movetool.hpp
    src/tools/polygondrawingtool.cpp
    src/tools/polygondrawingtool.hpp
    src/tools/rectangletool.cpp
    src/tools/rectangletool.hpp
    src/tools/selectiontool/selectiontool.cpp
    src/tools/selectiontool/selectiontool.hpp
    src/tools/selectiontool/selectiontoolmovestate.cpp
    src/tools/selectiontool/selectiontoolmovestate.hpp
    src/tools/selectiontool/selectiontoolselectstate.cpp
    src/tools/selectiontool/selectiontoolselectstate.hpp
    src/tools/selectiontool/selectiontoolstate.hpp
    src/tools/texttool.cpp
    src/tools/texttool.hpp
    src/tools/tool.cpp
    src/tools/tool.hpp
    src/window/boardlayout.cpp
    src/window/boardlayout.hpp
    src/window/window.cpp
    src/window/window.hpp
    src/main.cpp
    src/resources/res.qrc
    src/resources/style.qss
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
ecm_create_qm_loader(${PROJECT_NAME} drawy_qt)
add_executable(${PROJECT_NAME})
ecm_qt_declare_logging_category(${PROJECT_NAME} HEADER drawy_debug.h IDENTIFIER DRAWY_LOG
    CATEGORY_NAME org.kde.drawy DESCRIPTION "drawy" EXPORT DRAWY
)
target_include_directories(${PROJECT_NAME} PRIVATE ${ZSTD_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ZSTD_LINK_LIBRARIES})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE KF6::Crash)

# Set bundle properties for macOS / iOS.
if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
        MACOSX_BUNDLE TRUE
    )
endif()

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

install(
    TARGETS
        ${PROJECT_NAME}
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

if (UNIX AND NOT APPLE)
  install(PROGRAMS deploy/linux/org.kde.drawy.desktop DESTINATION ${KDE_INSTALL_APPDIR})
  install(FILES assets/logo.svg DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/icons/hicolor/scalable/apps RENAME org.kde.drawy.svg)
  install(FILES deploy/linux/org.kde.drawy.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
endif()

ecm_qt_install_logging_categories(
    EXPORT DRAWY
    FILE drawy.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)

ecm_install_po_files_as_qm(poqm)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
