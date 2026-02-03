# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

add_executable(
    Drawy
    main.cpp
    drawy.qrc
)
ecm_create_qm_loader(Drawy drawy_qt)

target_link_libraries(
    Drawy
    KF6::Crash
    KF6::CoreAddons
    KF6::IconThemes
    Qt::Widgets
    libdrawywidgets
)

install(
    TARGETS
        Drawy
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)
