// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "window.hpp"

#include <QFile>
#include <QFileInfo>
#include <QFontDatabase>
#include <QShortcut>
#include <QWindow>

#include <KActionCollection>
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KStandardAction>
#include <KStandardActions>
#include <KWindowConfig>

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

#if WITH_DBUS
#include "dbus/managepowermanagement.hpp"
#endif
#if HAVE_KUSERFEEDBACK
#include "userfeedback/userfeedbackmanager.hpp"
#include <KUserFeedback/NotificationPopup>
#include <KUserFeedback/Provider>
#endif
using namespace Qt::Literals::StringLiterals;

namespace
{
const char myConfigGroupName[] = "MainWindow";
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , mApplicationContext(new ApplicationContext(this))
#if WITH_DBUS
    , mManagePowerManagement(new ManagePowerManagement(this))
#endif
{
    loadCustomFonts();
    m_autoSaveJob = new AutoSaveJob{mApplicationContext, this};
    auto layout{new BoardLayout(this)};
    auto controller{new Controller(mApplicationContext, this)};

    auto renderingContext{mApplicationContext->renderingContext()};
    auto uiContext{mApplicationContext->uiContext()};

    connect(this, &MainWindow::paletteChanged, uiContext, &UIContext::slotThemeChanged);
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
    actionCollection->associateWidget(this);

    readConfig();
#if HAVE_KUSERFEEDBACK
    auto userFeedBackNotificationPopup = new KUserFeedback::NotificationPopup(this);
    userFeedBackNotificationPopup->setFeedbackProvider(UserFeedBackManager::self()->userFeedbackProvider());
#endif
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
        writeConfig();
        e->accept();
        return;
    }

    if (mApplicationContext->currentFileModified()) {
        if (mApplicationContext->uiContext()->actionManager()->confirmSaveAfterModification()) {
            writeConfig();
            e->accept();
        } else {
            e->ignore();
        }
        return;
    }

    writeConfig();
    e->accept();
}

void MainWindow::activeDebug()
{
    mApplicationContext->setDebug(true);
}

void MainWindow::viewFullScreen(bool fullScreen)
{
#if WITH_DBUS
    if (DrawyGlobalConfig::self()->enabledInFullScreen()) {
        if (fullScreen) {
            mManagePowerManagement->inhibitPowerManagement();
        } else {
            mManagePowerManagement->allowPowerManagement();
        }
    }
#endif
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

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::PaletteChange) {
        Q_EMIT paletteChanged();
    }
}

void MainWindow::readConfig()
{
    create(); // ensure a window is created
    if (!windowHandle()) {
        return;
    }

    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584

    if (group.readEntry("FullScreen", false)) {
        viewFullScreen(true);
    }
}

void MainWindow::writeConfig() const
{
    if (!windowHandle()) {
        return;
    }

    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigGroupName));
    group.writeEntry("FullScreen", isFullScreen());
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_window.cpp"
