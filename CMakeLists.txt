# SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
#
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(DRAWY_VERSION "0.1")
project(drawy VERSION ${DRAWY_VERSION})

set(DRAWY_LIB_VERSION "${DRAWY_VERSION}")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(KF_MIN_VERSION "6.19.0")
set(QT_REQUIRED_VERSION "6.9.0")
find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDEClangFormat)
include(KDEGitCommitHooks)
include(ECMQtDeclareLoggingCategory)
include(ECMPoQmTools)
include(KDEInstallDirs)
include(FeatureSummary)
include(ECMInstallIcons)
include(GenerateExportHeader)
include(ECMAddAppIcon)
include(KDECMakeSettings)
include(ECMAddTests)
include(ECMDeprecationSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
kde_enable_exceptions()

ecm_set_disabled_deprecation_versions(QT 6.11.0  KF 6.21.0)

# Find Qt6
find_package(Qt6 ${QT_REQUIRED_VERSION} REQUIRED
    COMPONENTS
        Widgets
)
if(BUILD_TESTING)
    find_package(Qt6 ${QT_REQUIRED_VERSION} REQUIRED COMPONENTS Test)
    add_definitions(-DBUILD_TESTING)
endif()
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS Crash)
option(
    USE_UNITY_CMAKE_SUPPORT
    "Use UNITY cmake support (speedup compile time)"
    OFF
) 

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if(USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()

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

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Not setting for GNU due to too many warnings related to private members of base classes or around lambdas
    # see e.g. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56556 or https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79328
    string(APPEND CMAKE_CXX_FLAGS " -Wshadow")
endif()


include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(BUILD_TESTING)
    find_package(Qt6 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Test)
    add_definitions(-DBUILD_TESTING)
endif()

add_subdirectory(src)


ecm_qt_install_logging_categories(
    EXPORT DRAWY
    FILE drawy.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)

ecm_install_po_files_as_qm(poqm)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
