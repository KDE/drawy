// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "item/itemutils.hpp"
#include "jobs/loadjob.hpp"
#include "tools/tool.hpp"
#include <KStandardAction>
#include <KStandardActions>
#include <QAction>
#include <QObject>
#include <functional>
class KActionCollection;

class ApplicationContext;
class ActionManager : public QObject
{
    Q_OBJECT
public:
    // All non standard actions (not available in KStandardAction)
    enum class Action : uint8_t {
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
        SwitchToMoveTool,
        Clear,
        SendToBack,
        SendBackward,
        BringForward,
        BringToFront,
        Debug,
    };

    Q_ENUM(Action)

    enum class Choice {
        Save,
        Discard,
        Cancel
    };

    Q_ENUM(Choice)

    explicit ActionManager(KActionCollection *actionCollection, ApplicationContext *context);

    [[nodiscard]] QAction *action(Action type) const;
    [[nodiscard]] QString actionName(Action type) const;
    [[nodiscard]] QAction *action(KStandardActions::StandardAction standardAction) const;
    [[nodiscard]] QString actionName(KStandardActions::StandardAction standardAction) const;
    [[nodiscard]] QAction *action(KStandardAction::StandardAction standardAction) const;
    [[nodiscard]] QString actionName(KStandardAction::StandardAction standardAction) const;

    void zoomIn();
    void zoomOut();
    void zoomReset();
    void undo();
    void redo();
    void copy();
    void paste();
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

    void switchToTool(Tool::Type type);
    void alignItems(ItemUtils::AlignType alignType);

    void clear();

    // SLOTS
    void slotUpdateZoomButtons();
    void slotUpdateHistoryButtons();
    void slotUpdateZorderAndGroupButtons();

private:
    void slotDebug();
    void openRecentFile(const QUrl &url);
    void zorderMove(ItemUtils::ZorderMove move);
    void slotLoadDone(const LoadJob::LoadInfo &info);
    void configureShortcuts();
    QAction *createAction(const Action &actionType, const QString &title, const QList<QKeySequence> &keys);
    QAction *createToolAction(const Action &actionType, const QString &title, const QList<QKeySequence> &keys, Tool::Type toolType);

    // This will allow us to create actions and connect them to slots directly :)
    template<typename Receiver, typename Func>
    QAction *createAction(const Action &actionType, const QString &title, const QList<QKeySequence> &keys, Receiver *recvr, Func &&slot)
    {
        auto result{createAction(actionType, title, keys)};
        if constexpr (std::is_invocable_v<Func, Receiver *>) {
            connect(result, &QAction::triggered, this, [recvr, f = std::forward<Func>(slot)](bool) mutable {
                std::invoke(f, recvr);
            });
        } else {
            // it means it's not a member function
            connect(result, &QAction::triggered, this, [f = std::forward<Func>(slot)](bool) mutable {
                std::invoke(f);
            });
        }

        return result;
    }

    ApplicationContext *const m_context;
    KActionCollection *const m_actionCollection;
    KRecentFilesAction *const m_recentFiles;
};
