# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

add_executable(
    drawy
    main.cpp
    drawy.qrc
)
ecm_create_qm_loader(drawy drawy_qt)

target_link_libraries(
    drawy
    KF6::Crash
    Qt::Widgets
    libdrawywidgets
    )


install(
    TARGETS
        drawy
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)
