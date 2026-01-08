# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-2-Clause
set(icons
    16-apps-drawy.png
    32-apps-drawy.png
    48-apps-drawy.png
    32-mimetypes-application-x-drawy.png
    64-mimetypes-application-x-drawy.png
)

set(all_icons ${icons})

ecm_install_icons(ICONS ${all_icons}
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)
