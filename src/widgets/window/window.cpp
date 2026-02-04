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
#include "jobs/saveasjob.hpp"
#include "keybindings/actionmanager.hpp"
#include "keybindings/keybindmanager.hpp"
#include <KMessageBox>
#include <QDir>
#include <QMenu>

using namespace Qt::Literals::StringLiterals;
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_autoSaveJob{new AutoSaveJob{this}}
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

    auto restoreAutoSaveJob = new RestoreAutoSaveJob(context, this);
    restoreAutoSaveJob->setParentWidget(this);
    restoreAutoSaveJob->start();

    AutoSaveJobUtil::createAutoSaveStandardPath();
    m_autoSaveJob->start();

    auto actionCollection{context->uiContext()->keybindManager()->actionCollection()};
    QAction *fullScreenAction = KStandardAction::fullScreen(nullptr, nullptr, this, actionCollection);
    fullScreenAction->setChecked(isFullScreen());
    connect(fullScreenAction, &QAction::toggled, this, &MainWindow::viewFullScreen);

    KStandardActions::quit(this, &MainWindow::close, actionCollection);
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
    auto context{ApplicationContext::instance()};
    if (DrawyGlobalConfig::self()->autoSaveEnabled()) {
        m_autoSaveJob->saveFile();
        e->accept();
        return;
    }

    if (context->currentFileModified()) {
        if (context->uiContext()->actionManager()->confirmSaveAfterModification()) {
            e->accept();
        } else {
            e->ignore();
        }
        return;
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

#include "moc_window.cpp"
