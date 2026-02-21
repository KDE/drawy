// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "window.hpp"

#include <QFile>
#include <QFileInfo>
#include <QFontDatabase>
#include <QShortcut>

#include <KActionCollection>
#include <KLocalizedString>
#include <KStandardAction>
#include <KStandardActions>

#include "boardlayout.hpp"
#include "canvas/canvas.hpp"
#include "common/constants.hpp"
#include "components/bottomleftwidgets.hpp"
#include "components/propertybar.hpp"
#include "components/topleftwidgets.hpp"
#include "components/topwidgets.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "controller/controller.hpp"
#include "drawy_debug.h"
#include "drawyglobalconfig.h"
#include "jobs/autosavejob.hpp"
#include "jobs/autosavejobutil.hpp"
#include "jobs/restoreautosavejob.hpp"
#include "keybindings/actionmanager.hpp"
#include "keybindings/keybindmanager.hpp"

using namespace Qt::Literals::StringLiterals;
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , mApplicationContext(new ApplicationContext(this))
{
    loadCustomFonts();
    m_autoSaveJob = new AutoSaveJob{mApplicationContext, this};
    auto layout{new BoardLayout(this)};
    auto controller{new Controller(mApplicationContext, this)};

    auto renderingContext{mApplicationContext->renderingContext()};
    auto uiContext{mApplicationContext->uiContext()};

    renderingContext->canvas()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->setMargins(10);
    layout->setLeftWidget(uiContext->propertyBar());
    layout->setTopWidget(uiContext->topWidgets());
    layout->setTopLeftWidget(uiContext->topLeftWidgets());
    layout->setBottomLeftWidget(uiContext->bottomLeftWidgets());
    layout->setCentralWidget(renderingContext->canvas());

    connect(renderingContext->canvas(), &Canvas::mousePressed, controller, &Controller::mousePressed);
    connect(renderingContext->canvas(), &Canvas::mouseMoved, controller, &Controller::mouseMoved);
    connect(renderingContext->canvas(), &Canvas::mouseReleased, controller, &Controller::mouseReleased);
    connect(renderingContext->canvas(), &Canvas::keyPressed, controller, &Controller::keyPressed);
    connect(renderingContext->canvas(), &Canvas::keyReleased, controller, &Controller::keyReleased);
    connect(renderingContext->canvas(), &Canvas::tablet, controller, &Controller::tablet);
    connect(renderingContext->canvas(), &Canvas::wheel, controller, &Controller::wheel);
    connect(renderingContext->canvas(), &Canvas::leave, controller, &Controller::leave);
    connect(renderingContext->canvas(), &Canvas::dragEnter, controller, &Controller::dragEnter);
    connect(renderingContext->canvas(), &Canvas::dragMove, controller, &Controller::dragMove);
    connect(renderingContext->canvas(), &Canvas::dragLeave, controller, &Controller::dragLeave);
    connect(renderingContext->canvas(), &Canvas::drop, controller, &Controller::drop);

    auto restoreAutoSaveJob = new RestoreAutoSaveJob(mApplicationContext, this);
    restoreAutoSaveJob->setParentWidget(this);
    restoreAutoSaveJob->start();

    AutoSaveJobUtil::createAutoSaveStandardPath();
    m_autoSaveJob->start();

    auto actionCollection{mApplicationContext->uiContext()->keybindManager()->actionCollection()};
    QAction *fullScreenAction = KStandardAction::fullScreen(nullptr, nullptr, this, actionCollection);
    fullScreenAction->setChecked(isFullScreen());
    connect(fullScreenAction, &QAction::toggled, this, &MainWindow::viewFullScreen);

    KStandardActions::quit(this, &MainWindow::close, actionCollection);

    updateWindowTitle();
    connect(mApplicationContext, &ApplicationContext::currentFileStateChanged, this, &MainWindow::updateWindowTitle);
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
    if (DrawyGlobalConfig::self()->autoSaveEnabled()) {
        m_autoSaveJob->saveFile();
        e->accept();
        return;
    }

    if (mApplicationContext->currentFileModified()) {
        if (mApplicationContext->uiContext()->actionManager()->confirmSaveAfterModification()) {
            e->accept();
        } else {
            e->ignore();
        }
        return;
    } else {
        e->accept();
    }
}

void MainWindow::activeDebug()
{
    mApplicationContext->setDebug(true);
}

void MainWindow::viewFullScreen(bool fullScreen)
{
    KToggleFullScreenAction::setFullScreen(this, fullScreen);
}

void MainWindow::loadFile(const QString &fileName)
{
    auto actionManager{mApplicationContext->uiContext()->actionManager()};
    actionManager->loadFile(fileName);
}

void MainWindow::updateWindowTitle()
{
    const QFileInfo currentFileInfo(mApplicationContext->currentFileName());
    QString fileName{currentFileInfo.fileName()};

    if (mApplicationContext->currentFileModified()) {
        fileName.append(u" *"_s);
    }

    setWindowTitle(i18nc("@title:window %1 is a filename", "%1 — Drawy", fileName));
}

#include "moc_window.cpp"
