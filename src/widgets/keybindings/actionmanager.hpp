// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "item/itemutils.hpp"
#include "jobs/loadjob.hpp"
#include "tools/tool.hpp"
#include <KStandardActions>
#include <QAction>
#include <QObject>
#include <functional>

class ApplicationContext;
class ActionManager : public QObject
{
    Q_OBJECT
public:
    enum class AlignType : uint8_t {
        Unknown = 0,
        AlignLeft,
        CentralHorizontal,
        AlignRight,
        AlignTop,
        CentralVertical,
        AlignBottom,
    };

    Q_ENUM(AlignType)

    // All non standard actions (not available in KStandardAction)
    enum class Action {
        GroupItems,
        UngroupItems,
        DeleteSelection,
        ExportAsSVG,
        SwitchToSelectionTool,
        SwitchToFreeformTool,
        SwitchToRectangleTool,
        SwitchToEllipseTool,
        SwitchToLineTool,
        SwitchToArrowTool,
        SwitchToTextTool,
        SwitchToEraserTool,
        SwitchToMoveTool
    };

    Q_ENUM(Action)

    enum class Choice {
        Save,
        Discard,
        Cancel
    };

    Q_ENUM(Choice)

    explicit ActionManager(ApplicationContext *context);

    [[nodiscard]] QAction *action(Action type) const;
    [[nodiscard]] QString actionName(Action type) const;
    [[nodiscard]] QAction *action(KStandardActions::StandardAction standardAction) const;
    [[nodiscard]] QString actionName(KStandardActions::StandardAction standardAction) const;

    void zoomIn();
    void zoomOut();
    void undo();
    void redo();
    void switchToMoveTool();
    void selectAll();
    void deleteSelection();
    void groupItems();
    void ungroupItems();
    void newFile();
    void saveAsNewFile();
    void saveCurrentFile();
    void openFile();
    void loadFile(const QString &fileName);
    bool confirmSaveAfterModification(); // to confirm if you want to save the file quitting/opening a new file
    void exportToSvg();
    void configureSettings();
    void showHamburgerMenu();

    void switchToTool(Tool::Type type);
    void alignItems(ItemUtils::AlignType alignType);

    void clear();

private:
    void slotLoadDone(const LoadJob::LoadInfo &info);
    QAction *createAction(const Action &actionType, const QString &title, const QList<QKeySequence> &keys);
    QAction *createToolAction(const Action &actionType, const QString &title, const QList<QKeySequence> &keys, Tool::Type toolType);

    // This will allow us to create actions and connect them to slots directly :)
    template<typename Receiver, typename Func>
    QAction *createAction(const Action &actionType, const QString &title, const QList<QKeySequence> &keys, Receiver *recvr, Func &&slot)
    {
        auto result{createAction(actionType, title, keys)};
        connect(result, &QAction::triggered, this, [recvr, f = std::forward<Func>(slot)](bool) mutable {
            if constexpr (std::is_invocable_v<Func, Receiver *>) {
                std::invoke(f, recvr);
            } else {
                // it means it's not a member function
                std::invoke(f);
            }
        });
        ;

        return result;
    }

    ApplicationContext *const m_context;
};
