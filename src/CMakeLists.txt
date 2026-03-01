# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause
set(DRAWY_LIB_SOVERSION "0")
add_subdirectory(gui)
add_subdirectory(widgets)
add_subdirectory(apps)
add_subdirectory(icons)
add_subdirectory(data)
add_subdirectory(plugins)

# Set bundle properties for macOS / iOS.
if(APPLE)
    set_target_properties(
        drawy
        PROPERTIES
            MACOSX_BUNDLE_DISPLAY_NAME
                "Drawy"
            MACOSX_BUNDLE_BUNDLE_NAME
                "Drawy"
            MACOSX_BUNDLE_LONG_VERSION_STRING
                "Drawy ${PROJECT_VERSION}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING
                "${PROJECT_VERSION}"
            MACOSX_BUNDLE_BUNDLE_VERSION
                "${PROJECT_VERSION}"
            MACOSX_BUNDLE_GUI_IDENTIFIER
                "org.kde.drawy"
            MACOSX_BUNDLE_COPYRIGHT
                "2025-2026 The Drawy Developers"
    )
endif()
