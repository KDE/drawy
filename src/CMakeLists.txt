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
        ${PROJECT_NAME}
        PROPERTIES
            MACOSX_BUNDLE_GUI_IDENTIFIER
                com.example.${PROJECT_NAME}
            MACOSX_BUNDLE_BUNDLE_VERSION
                ${PROJECT_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING
                "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
            MACOSX_BUNDLE
                TRUE
    )
endif()
