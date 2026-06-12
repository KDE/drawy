# SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
#
# SPDX-License-Identifier: BSD-2-Clause

include(GenerateExportHeader)

add_library(drawyconfig SHARED)

target_link_libraries(
    drawyconfig
    PUBLIC
        Qt6::Core
        KF6::ConfigCore
        KF6::ConfigWidgets
)

kconfig_add_kcfg_files(drawyconfig drawyglobalconfig.kcfgc)

generate_export_header(drawyconfig BASE_NAME DRAWYCONFIG)

target_include_directories(drawyconfig PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

install(FILES drawyglobalconfig.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})
