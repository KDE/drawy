// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "window.hpp"

#include <QButtonGroup>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QShortcut>

#include <KActionCollection>
#include <KStandardAction>
#include <KStandardActions>

#include "boardlayout.hpp"
#include "canvas/canvas.hpp"
#include "common/constants.hpp"
#include "components/actionbar.hpp"
#include "components/propertybar.hpp"
#include "components/toolbar.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "controller/controller.hpp"
#include "data-structures/quadtree.hpp"
#include "dialog/configuresettingsdialog.hpp"
#include "drawy_debug.h"
#include "jobs/autosavejob.hpp"
#include "jobs/autosavejobutil.hpp"
#include "jobs/restoreautosavejob.hpp"
#include "jobs/saveasjob.hpp"
#include "keybindings/actionmanager.hpp"
#include "serializer/serializerutils.hpp"
#include <KMessageBox>
#include <QDir>
#include <QMenu>
using namespace Qt::Literals::StringLiterals;
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    loadCustomFonts();
    auto layout{new BoardLayout(this)};
    auto controller{new Controller(this)};
    ApplicationContext *context{ApplicationContext::instance()};

    auto renderingContext{context->renderingContext()};
    auto uiContext{context->uiContext()};

    renderingContext->canvas()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->setMargins(10);
    layout->setLeftWidget(uiContext->propertyBar());
    layout->setTopWidget(uiContext->toolBar());
    layout->setBottomWidget(uiContext->actionBar());
    layout->setCentralWidget(renderingContext->canvas());

    connect(renderingContext->canvas(), &Canvas::mousePressed, controller, &Controller::mousePressed);
    connect(renderingContext->canvas(), &Canvas::mouseMoved, controller, &Controller::mouseMoved);
    connect(renderingContext->canvas(), &Canvas::mouseReleased, controller, &Controller::mouseReleased);
    connect(renderingContext->canvas(), &Canvas::keyPressed, controller, &Controller::keyPressed);
    connect(renderingContext->canvas(), &Canvas::keyReleased, controller, &Controller::keyReleased);

    connect(renderingContext->canvas(), &Canvas::tablet, controller, &Controller::tablet);
    connect(renderingContext->canvas(), &Canvas::wheel, controller, &Controller::wheel);
    connect(renderingContext->canvas(), &Canvas::leave, controller, &Controller::leave);
    connect(renderingContext->canvas(), &Canvas::customContextMenuRequested, this, &MainWindow::contextMenuRequested);

    auto restoreAutoSaveJob = new RestoreAutoSaveJob(context, this);
    restoreAutoSaveJob->setParentWidget(this);
    connect(restoreAutoSaveJob, &RestoreAutoSaveJob::restoreDone, this, [this, context]() {
        AutoSaveJobUtil::createAutoSaveStandardPath();
        // Autosave
        auto autoSaveJob = new AutoSaveJob(context, this);
        autoSaveJob->start();
    });
    restoreAutoSaveJob->start();
    setupAction();
}

MainWindow::~MainWindow() = default;

void MainWindow::loadCustomFonts()
{
    int fontID = QFontDatabase::addApplicationFont(u":/fonts/FuzzyBubbles.ttf"_s);
    if (fontID == -1) {
        qCWarning(DRAWY_LOG) << "Failed to load font: FuzzyBubbles";
    }

    fontID = QFontDatabase::addApplicationFont(u":/fonts/Inter.ttf"_s);
    if (fontID == -1) {
        qCWarning(DRAWY_LOG) << "Failed to load font: Inter";
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (!m_forceClose) {
        if (!ApplicationContext::instance()->spatialContext()->quadtree().getAllItems().isEmpty()) {
            const int choice = KMessageBox::questionTwoActionsCancel(this,
                                                                     tr("Do you want to save file?"),
                                                                     tr("Close"),
                                                                     KGuiItem(tr("Save And Close")),
                                                                     KGuiItem(tr("Ignore")),
                                                                     KStandardGuiItem::cancel());
            if (choice == KMessageBox::Cancel) {
                e->ignore();
            } else if (choice == KMessageBox::ButtonCode::PrimaryAction) {
                ApplicationContext *context{ApplicationContext::instance()};
                QString fileName = context->currentFileName();
                if (fileName.isEmpty()) {
                    const QDir homeDir{QDir::home()};
                    QString text = QObject::tr("Untitled.%1").arg(Common::drawyFileExt);
                    const QString defaultFilePath = homeDir.filePath(text);
                    text = QObject::tr("Drawy (*.%1)").arg(Common::drawyFileExt);
                    fileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save File"), defaultFilePath, text);
                    if (fileName.isEmpty()) {
                        e->ignore();
                        return;
                    }
                }
                m_forceClose = true;
                auto job = new SaveAsJob(this);
                const SaveAsJob::SaveAsInfo info{
                    .filePath = fileName,
                    .offsetPos = context->spatialContext()->offsetPos(),
                    .zoomFactor = context->renderingContext()->zoomFactor(),
                    .items = context->spatialContext()->quadtree().getAllItems(),
                };
                job->setSaveAsInfo(info);
                connect(job, &SaveAsJob::saveFileDone, this, [fileName, this](const QJsonObject &obj) {
                    SerializerUtils::saveInFile(obj, fileName);
                    qDebug() << " save done ";
                    AutoSaveJobUtil::removeAutoSaveFile();
                    close();
                });
                job->start();
            } else {
                e->accept();
            }
            return;
        } else {
            e->accept();
        }
    } else {
        e->accept();
    }
}

void MainWindow::viewFullScreen(bool fullScreen)
{
    KToggleFullScreenAction::setFullScreen(this, fullScreen);
}

void MainWindow::loadFile(const QString &fileName)
{
    ApplicationContext *context{ApplicationContext::instance()};
    auto actionManager{context->uiContext()->actionManager()};
    actionManager->loadFile(fileName);
}

void MainWindow::contextMenuRequested([[maybe_unused]] const QPoint &pos)
{
    auto menu = new QMenu(this);

    menu->addAction(mFullScreenAction);
    menu->addSeparator();
    menu->addAction(mConfigureSettingsAction);
    menu->addSeparator();
    menu->addAction(mQuitAction);
    menu->exec(QCursor::pos());
    delete menu;
}

void MainWindow::configureSettings()
{
    ConfigureSettingsDialog dlg(this);
    dlg.exec();
}

void MainWindow::setupAction()
{
    ApplicationContext *context{ApplicationContext::instance()};
    auto actionManager{context->uiContext()->actionManager()};

    mActionCollection = new KActionCollection(this);

    mFullScreenAction = KStandardAction::fullScreen(nullptr, nullptr, this, mActionCollection);
    mFullScreenAction->setChecked(isFullScreen());
    connect(mFullScreenAction, &QAction::toggled, this, &MainWindow::viewFullScreen);
    mConfigureSettingsAction = KStandardActions::preferences(this, &MainWindow::configureSettings, mActionCollection);
    mQuitAction = KStandardActions::quit(this, &MainWindow::close, mActionCollection);

    mSaveAction = KStandardAction::save(actionManager, &ActionManager::saveToFile, mActionCollection);

    mUndoAction = KStandardAction::undo(actionManager, &ActionManager::undo, mActionCollection);
    mRedoAction = KStandardAction::redo(actionManager, &ActionManager::redo, mActionCollection);

    mZoomInAction = KStandardAction::zoomIn(actionManager, &ActionManager::zoomIn, mActionCollection);
    mZoomOutAction = KStandardAction::zoomIn(actionManager, &ActionManager::zoomOut, mActionCollection);

    mLoadAction = KStandardAction::open(actionManager, &ActionManager::loadFromFile, mActionCollection);
    mSelectAllAction = KStandardAction::selectAll(actionManager, &ActionManager::selectAll, mActionCollection);

    createToolAction(u"freeform_tool"_s,
                     tr("Freeform Tool"),
                     Tool::Type::Freeform,
                     {QKeySequence(QKeyCombination(Qt::Key_P)), QKeySequence(QKeyCombination(Qt::Key_B))});
    createToolAction(u"eraser_tool"_s, tr("Eraser Tool"), Tool::Type::Eraser, {QKeySequence(QKeyCombination(Qt::Key_E))});
    createToolAction(u"selection_tool"_s, tr("Selection Tool"), Tool::Type::Selection, {QKeySequence(QKeyCombination(Qt::Key_S))});
    createToolAction(u"rectangle_tool"_s, tr("Rectangle Tool"), Tool::Type::Rectangle, {QKeySequence(QKeyCombination(Qt::Key_R))});
    createToolAction(u"ellipse_tool"_s, tr("Ellipse Tool"), Tool::Type::Ellipse, {QKeySequence(QKeyCombination(Qt::Key_O))});
    createToolAction(u"line_tool"_s, tr("Line Tool"), Tool::Type::Line, {QKeySequence(QKeyCombination(Qt::Key_L))});
    createToolAction(u"text_tool"_s, tr("Text Tool"), Tool::Type::Text, {QKeySequence(QKeyCombination(Qt::Key_T))});
    createToolAction(u"arrow_tool"_s, tr("Arrow Tool"), Tool::Type::Text, {QKeySequence(QKeyCombination(Qt::Key_A))});

    QAction *act = createAction(u"move_tool"_s, tr("Move Tool"), {QKeySequence(QKeyCombination(Qt::Key_M))});
    connect(act, &QAction::triggered, actionManager, [actionManager]() {
        actionManager->switchToMoveTool();
    });
    act = createAction(u"group_element"_s, tr("Group Elements"), {QKeySequence(QKeyCombination(Qt::Key_G))});
    connect(act, &QAction::triggered, actionManager, [actionManager]() {
        actionManager->groupItems();
    });
    act = createAction(u"ungroup_element"_s, tr("Ungroup Elements"), {QKeySequence(QKeyCombination(Qt::CTRL | Qt::SHIFT, Qt::Key_G))});
    connect(act, &QAction::triggered, actionManager, [actionManager]() {
        actionManager->ungroupItems();
    });
    act = createAction(u"delete_selected_items"_s, tr("Delete"), {QKeySequence::Delete});
    connect(act, &QAction::triggered, actionManager, [actionManager]() {
        actionManager->deleteSelection();
    });
    mActionCollection->associateWidget(this);
    mActionCollection->readSettings();
}

QAction *MainWindow::createAction(const QString &actionName, const QString &title, const QList<QKeySequence> &keys)
{
    auto act = new QAction(title, mActionCollection);
    mActionCollection->addAction(actionName, act);
    mActionCollection->setDefaultShortcuts(act, keys);
    act->setShortcuts(keys);
    return act;
}

void MainWindow::createToolAction(const QString &actionName, const QString &title, Tool::Type type, const QList<QKeySequence> &keys)
{
    ApplicationContext *context{ApplicationContext::instance()};
    auto actionManager{context->uiContext()->actionManager()};

    auto act = new QAction(title, mActionCollection);
    mActionCollection->addAction(actionName, act);
    mActionCollection->setDefaultShortcuts(act, keys);
    act->setShortcuts(keys);
    connect(act, &QAction::triggered, actionManager, [type, actionManager]() {
        actionManager->switchToTool(type);
    });
}

#include "moc_window.cpp"
