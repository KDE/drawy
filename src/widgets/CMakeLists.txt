# SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

add_library(libdrawywidgets)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

target_sources(
    libdrawywidgets
    PRIVATE
        item/itemcache/itemcache.cpp
        item/itemcache/itemcache.hpp
        canvas/canvas.cpp
        canvas/canvas.hpp
        command/commandhistory.cpp
        command/commandhistory.hpp
        command/deselectcommand.cpp
        command/deselectcommand.hpp
        command/groupcommand.cpp
        command/groupcommand.hpp
        command/insertitemcommand.cpp
        command/insertitemcommand.hpp
        command/itemcommand.cpp
        command/itemcommand.hpp
        command/moveitemcommand.cpp
        command/moveitemcommand.hpp
        command/resizeitemcommand.cpp
        command/resizeitemcommand.hpp
        command/lineresizeitemcommand.cpp
        command/lineresizeitemcommand.hpp
        command/rotateitemcommand.cpp
        command/rotateitemcommand.hpp
        command/removeitemcommand.cpp
        command/removeitemcommand.hpp
        command/selectcommand.cpp
        command/selectcommand.hpp
        command/ungroupcommand.cpp
        command/ungroupcommand.hpp
        command/updatepropertycommand.cpp
        command/updatepropertycommand.hpp
        command/alignitemcommand.hpp
        command/alignitemcommand.cpp
        command/lockcommand.hpp
        command/lockcommand.cpp
        command/zordercommand.hpp
        command/zordercommand.cpp
        command/commandbase.hpp
        command/commandbase.cpp
        components/bottomleftwidgets.cpp
        components/bottomleftwidgets.hpp
        components/propertybar.cpp
        components/propertybar.hpp
        components/toolbar.cpp
        components/toolbar.hpp
        components/frame.cpp
        components/frame.hpp
        components/clickablelabel.cpp
        components/clickablelabel.hpp
        components/topwidgets.cpp
        components/topwidgets.hpp
        components/topleftwidgets.cpp
        components/topleftwidgets.hpp
        components/toolbuttonselectplugin.hpp
        components/toolbuttonselectplugin.cpp
        components/flowlayout.hpp
        components/flowlayout.cpp
        components/toolbuttonspluginwidget.hpp
        components/toolbuttonspluginwidget.cpp
        components/toolbuttonplugin.hpp
        components/toolbuttonplugin.cpp
        context/applicationcontext.cpp
        context/applicationcontext.hpp
        context/coordinatetransformer.cpp
        context/coordinatetransformer.hpp
        context/renderingcontext.cpp
        context/renderingcontext.hpp
        context/selectioncontext.cpp
        context/selectioncontext.hpp
        context/spatialcontext.cpp
        context/spatialcontext.hpp
        context/uicontext.cpp
        context/uicontext.hpp
        context/aboutdata.hpp
        context/aboutdata.cpp
        context/helpmenu.hpp
        context/helpmenu.cpp
        controller/controller.cpp
        controller/controller.hpp
        data-structures/cachegrid.cpp
        data-structures/cachegrid.hpp
        data-structures/orderedlist.cpp
        data-structures/orderedlist.hpp
        data-structures/quadtree.cpp
        data-structures/quadtree.hpp
        data-structures/quadtree.ipp
        event/event.cpp
        event/event.hpp
        keybindings/actionmanager.cpp
        keybindings/actionmanager.hpp
        keybindings/keybindmanager.cpp
        keybindings/keybindmanager.hpp
        properties/widgets/actionswidget.cpp
        properties/widgets/actionswidget.hpp
        properties/widgets/erasersizewidget.cpp
        properties/widgets/erasersizewidget.hpp
        properties/widgets/arrowtypewidget.cpp
        properties/widgets/arrowtypewidget.hpp
        properties/widgets/startarrowtypewidget.cpp
        properties/widgets/startarrowtypewidget.hpp
        properties/widgets/endarrowtypewidget.cpp
        properties/widgets/endarrowtypewidget.hpp
        properties/widgets/fontsizewidget.cpp
        properties/widgets/fontsizewidget.hpp
        properties/widgets/propertymanager.cpp
        properties/widgets/propertymanager.hpp
        properties/widgets/propertywidget.cpp
        properties/widgets/propertywidget.hpp
        properties/widgets/backgroundcolorwidget.cpp
        properties/widgets/backgroundcolorwidget.hpp
        properties/widgets/strokecolorwidget.cpp
        properties/widgets/strokecolorwidget.hpp
        properties/widgets/colorwidgetbase.cpp
        properties/widgets/colorwidgetbase.hpp
        properties/widgets/strokewidthwidget.cpp
        properties/widgets/strokewidthwidget.hpp
        properties/widgets/buttonactionswidget.hpp
        properties/widgets/buttonactionswidget.cpp
        properties/widgets/stokestylewidget.hpp
        properties/widgets/stokestylewidget.cpp
        properties/widgets/opacitywidget.hpp
        properties/widgets/opacitywidget.cpp
        properties/widgets/alignmentwidget.hpp
        properties/widgets/alignmentwidget.cpp
        properties/widgets/zorderwidget.hpp
        properties/widgets/zorderwidget.cpp
        properties/widgets/backgroundstylewidget.cpp
        properties/widgets/backgroundstylewidget.hpp
        properties/widgets/backgroundstyleactionswidget.h
        properties/widgets/backgroundstyleactionswidget.cpp
        properties/widgets/strokestyleactionswidget.hpp
        properties/widgets/strokestyleactionswidget.cpp
        properties/widgets/cornerrectangletypewidget.hpp
        properties/widgets/cornerrectangletypewidget.cpp
        tools/arrowtool.cpp
        tools/arrowtool.hpp
        tools/drawingtool.cpp
        tools/drawingtool.hpp
        tools/ellipsetool.cpp
        tools/ellipsetool.hpp
        tools/erasertool.cpp
        tools/erasertool.hpp
        tools/freeformtool.cpp
        tools/freeformtool.hpp
        tools/linetool.cpp
        tools/linetool.hpp
        tools/movetool.cpp
        tools/movetool.hpp
        tools/linedrawingtool.cpp
        tools/linedrawingtool.hpp
        tools/polygondrawingtool.cpp
        tools/polygondrawingtool.hpp
        tools/rectangletool.cpp
        tools/rectangletool.hpp
        tools/diamondtool.cpp
        tools/diamondtool.hpp
        tools/selectiontool/selectiontool.cpp
        tools/selectiontool/selectiontool.hpp
        tools/texttool.cpp
        tools/texttool.hpp
        tools/tool.cpp
        tools/tool.hpp
        tools/customtool.hpp
        tools/customtool.cpp
        tools/imagetool.cpp
        tools/imagetool.hpp
        transformhandler/transformhandler.hpp
        transformhandler/transformhandler.cpp
        transformhandler/linemovetransformhandler.hpp
        transformhandler/linemovetransformhandler.cpp
        transformhandler/lineresizetransformhandler.hpp
        transformhandler/lineresizetransformhandler.cpp
        transformhandler/movetransformhandler.hpp
        transformhandler/movetransformhandler.cpp
        transformhandler/resizetransformhandler.hpp
        transformhandler/resizetransformhandler.cpp
        transformhandler/rotatetransformhandler.hpp
        transformhandler/rotatetransformhandler.cpp
        window/boardlayout.cpp
        window/boardlayout.hpp
        window/window.cpp
        window/window.hpp
        resources/res.qrc
        drawycommandlineparser.hpp
        drawycommandlineparser.cpp
        jobs/serializejob.hpp
        jobs/serializejob.cpp
        jobs/saveasjob.cpp
        jobs/saveasjob.hpp
        jobs/deserializejob.hpp
        jobs/deserializejob.cpp
        jobs/loadjob.hpp
        jobs/loadjob.cpp
        jobs/autosavejob.hpp
        jobs/autosavejob.cpp
        jobs/autosavejobutil.hpp
        jobs/autosavejobutil.cpp
        jobs/restoreautosavejob.hpp
        jobs/restoreautosavejob.cpp
        jobs/loadjobutil.hpp
        jobs/loadjobutil.cpp
        dialog/configuresettingsdialog.cpp
        dialog/configuresettingsdialog.hpp
        dialog/configgroupinterface.cpp
        dialog/configgroupinterface.hpp
        dialog/general/configuregeneralwidget.hpp
        dialog/general/configuregeneralwidget.cpp
        dialog/general/autosavewidget.hpp
        dialog/general/autosavewidget.cpp
        dialog/general/canvascolorwidget.hpp
        dialog/general/canvascolorwidget.cpp
        dialog/general/defaultcolorswidget.hpp
        dialog/general/defaultcolorswidget.cpp
        dialog/misc/configuremiscwidget.hpp
        dialog/misc/configuremiscwidget.cpp
        dialog/misc/powermanagerwidget.hpp
        dialog/misc/powermanagerwidget.cpp
        dialog/input/configureinputwidget.hpp
        dialog/input/configureinputwidget.cpp
        dialog/input/freehandsettingswidget.hpp
        dialog/input/freehandsettingswidget.cpp
        dialog/input/selectionsettingswidget.hpp
        dialog/input/selectionsettingswidget.cpp
        iconmanager/iconmanager.hpp
        iconmanager/iconmanager.cpp
        mime/drawymimehandler.cpp
        mime/drawymimehandler.hpp
        mime/textmimehandler.cpp
        mime/textmimehandler.hpp
        mime/svgmimehandler.cpp
        mime/svgmimehandler.hpp
        mime/imagemimehandler.cpp
        mime/imagemimehandler.hpp
        mime/rendermimehandler.cpp
        mime/rendermimehandler.hpp
        mime/mimehandler.cpp
        mime/mimehandler.hpp
        mime/mimemanager.cpp
        mime/mimemanager.hpp
        debug/debugdialog.hpp
        debug/debugdialog.cpp
        libdrawywidgets_private_export.h
        common/renderitems.cpp
        common/renderitems.hpp
        customelementswidget/customelementswidget.hpp
        customelementswidget/customelementswidget.cpp
)

if(USE_DBUS)
    target_sources(
        libdrawywidgets
        PRIVATE
            dbus/managepowermanagement.hpp
            dbus/managepowermanagement.cpp
    )
endif()

if(TARGET KF6::UserFeedbackWidgets)
    target_sources(
        libdrawywidgets
        PRIVATE
            userfeedback/userfeedbackmanager.cpp
            userfeedback/userfeedbackmanager.hpp
            userfeedback/drawyuserfeedbackprovider.cpp
            userfeedback/drawyuserfeedbackprovider.hpp
            dialog/userfeedback/configureuserfeedbackwidget.cpp
            dialog/userfeedback/configureuserfeedbackwidget.hpp
    )
endif()

if(TARGET KF6::UserFeedbackWidgets)
    target_link_libraries(libdrawywidgets PUBLIC KF6::UserFeedbackWidgets)
endif()

if(TARGET KF6::TextAddonsWidgets)
    target_link_libraries(libdrawywidgets PUBLIC KF6::TextAddonsWidgets)
endif()

# precompiled headers support
if(ENABLE_PCH)
    target_precompile_headers(
        libdrawywidgets
        PRIVATE
            <QObject>
            <QDebug>
    )
endif()

generate_export_header(libdrawywidgets BASE_NAME libdrawywidgets)

if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(
        libdrawywidgets
        PROPERTIES
            UNITY_BUILD
                ON
    )
endif()

target_link_libraries(
    libdrawywidgets
    PUBLIC
        libdrawygui
    PRIVATE
        Qt::Widgets
        KF6::WidgetsAddons
        KF6::ConfigWidgets
        KF6::XmlGui
        KF6::I18n
        KF6::SyntaxHighlighting
        drawyconfig
)

if(USE_DBUS)
    target_link_libraries(libdrawywidgets PRIVATE Qt::DBus)
endif()

# kconfig_add_kcfg_files(libdrawywidgets settings/drawyglobalconfig.kcfgc)

ecm_qt_declare_logging_category(libdrawywidgets HEADER drawy_debug.h IDENTIFIER DRAWY_LOG
    CATEGORY_NAME org.kde.drawy DESCRIPTION "drawy" EXPORT DRAWY
)

ecm_qt_declare_logging_category(libdrawywidgets HEADER drawy_command_debug.h IDENTIFIER DRAWY_COMMAND_LOG
    CATEGORY_NAME org.kde.drawy_command DESCRIPTION "drawy (command)" EXPORT DRAWY
)

ecm_qt_declare_logging_category(libdrawywidgets HEADER drawy_autosave_debug.h IDENTIFIER DRAWY_AUTOSAVE_LOG
    CATEGORY_NAME org.kde.drawy_autosave DESCRIPTION "drawy (autosave job)" EXPORT DRAWY
)

ecm_qt_declare_logging_category(libdrawywidgets HEADER drawy_powermanager_debug.h IDENTIFIER DRAWY_POWERMANAGER_LOG
    CATEGORY_NAME org.kde.drawy_powermanager DESCRIPTION "drawy (powermanager)" EXPORT DRAWY
)

ecm_qt_declare_logging_category(libdrawywidgets HEADER drawy_resize_debug.h IDENTIFIER DRAWY_RESIZE_LOG
    CATEGORY_NAME org.kde.drawy_resize DESCRIPTION "drawy (resize)" EXPORT DRAWY
)

# install(FILES settings/drawyglobalconfig.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})

set_target_properties(
    libdrawywidgets
    PROPERTIES
        OUTPUT_NAME
            drawywidgets
        VERSION
            ${DRAWY_LIB_VERSION}
        SOVERSION
            ${DRAWY_LIB_SOVERSION}
)

install(
    TARGETS
        libdrawywidgets
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
    LIBRARY
        NAMELINK_SKIP
)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()
