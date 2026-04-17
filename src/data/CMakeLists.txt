# SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-2-Clause
if(UNIX AND NOT APPLE)
    install(PROGRAMS org.kde.drawy.desktop DESTINATION ${KDE_INSTALL_APPDIR})
    install(FILES org.kde.drawy.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
    install(FILES application-x-drawy.xml DESTINATION ${KDE_INSTALL_MIMEDIR})
    update_xdg_mimetypes(${KDE_INSTALL_MIMEDIR})
endif()
