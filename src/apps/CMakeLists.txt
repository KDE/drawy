# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

add_executable(
    drawy
    main.cpp
    drawy.qrc
)

target_link_libraries(
    drawy
    KF6::Crash
    KF6::CoreAddons
    KF6::IconThemes
    KF6::ConfigWidgets
    Qt::Widgets
    libdrawywidgets
    drawyconfig
)

install(
    TARGETS
        drawy
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)
if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()
