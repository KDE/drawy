// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "config-drawy.hpp"
#include "item/itemutils.hpp"
#include "jobs/loadjob.hpp"
#include "tools/tool.hpp"
#include <KStandardAction>
#include <KStandardActions>
#include <QAction>
#include <QObject>
#include <functional>
#if HAVE_WHATSNEWSNGSUPPORT
#include <KAboutData>
#endif
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
        ExportAsImage,
        ExportSelectedElementsAsImage,
        SwitchToSelectionTool,
        SwitchToFreeformTool,
        SwitchToRectangleTool,
        SwitchToEllipseTool,
        SwitchToLineTool,
        SwitchToArrowTool,
        SwitchToTextTool,
        SwitchToEraserTool,
        SwitchToMoveTool,
        SwitchToImageTool,
        SwitchToDiamondTool,
        Clear,
        SendToBack,
        SendBackward,
        BringForward,
        BringToFront,
        AlignTop,
        AlignVerticalCenter,
        AlignBottom,
        AlignLeft,
        AlignHorizontalCenter,
        AlignRight,
        Debug,
        WhatsNew,
    };

    Q_ENUM(Action)

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
    [[nodiscard]] bool confirmSaveAfterModification(); // to confirm if you want to save the file quitting/opening a new file
    void exportToImage();
    void configureSettings();

    void switchToTool(Tool::Type type);
    void alignItems(ItemUtils::AlignType alignType);

    void clear();

    // SLOTS
    void slotUpdateZoomButtons();
    void slotUpdateHistoryButtons();
    void slotUpdateZorderAndGroupButtons();
#if HAVE_WHATSNEWSNGSUPPORT
    void slotShowWhatsNew();
    void setReleasesInfo(const QList<KAboutRelease> &releases);
#endif
private:
    void slotDebug();
    void openRecentFile(const QUrl &url);
    void zorderMove(ItemUtils::ZorderMove move);
    void slotLoadDone(const LoadJob::LoadInfo &info);
    void configureShortcuts();
    void exportSelectedElementsToImage();
    void exportToImageElements(const QList<std::shared_ptr<Item>> &items);
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

#if HAVE_WHATSNEWSNGSUPPORT
    QList<KAboutRelease> m_releases;
#endif

    ApplicationContext *const m_context;
    KActionCollection *const m_actionCollection;
    KRecentFilesAction *const m_recentFiles;
};
