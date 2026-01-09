// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "window.hpp"

#include <QButtonGroup>
#include <QFile>
#include <QFontDatabase>
#include <QShortcut>

#include "boardlayout.hpp"
#include "canvas/canvas.hpp"
#include "components/actionbar.hpp"
#include "components/propertybar.hpp"
#include "components/toolbar.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "controller/controller.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_debug.h"
#include "keybindings/actionmanager.hpp"
#include <KMessageBox>
using namespace Qt::Literals::StringLiterals;
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    applyCustomStyles();

    BoardLayout *layout{new BoardLayout(this)};
    Controller *controller{new Controller(this)};
    ApplicationContext *context{ApplicationContext::instance()};

    RenderingContext &renderingContext{context->renderingContext()};
    UIContext &uiContext{context->uiContext()};

    renderingContext.canvas().setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->setMargins(10);
    layout->setLeftWidget(&uiContext.propertyBar());
    layout->setTopWidget(&uiContext.toolBar());
    layout->setBottomWidget(&uiContext.actionBar());
    layout->setCentralWidget(&renderingContext.canvas());

    connect(&renderingContext.canvas(), &Canvas::mousePressed, controller, &Controller::mousePressed);
    connect(&renderingContext.canvas(), &Canvas::mouseMoved, controller, &Controller::mouseMoved);
    connect(&renderingContext.canvas(), &Canvas::mouseReleased, controller, &Controller::mouseReleased);
    connect(&renderingContext.canvas(), &Canvas::keyPressed, controller, &Controller::keyPressed);
    connect(&renderingContext.canvas(), &Canvas::keyReleased, controller, &Controller::keyReleased);

    connect(&renderingContext.canvas(), &Canvas::tablet, controller, &Controller::tablet);
    connect(&renderingContext.canvas(), &Canvas::wheel, controller, &Controller::wheel);
    connect(&renderingContext.canvas(), &Canvas::leave, controller, &Controller::leave);

    applyCustomStyles();
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (!ApplicationContext::instance()->spatialContext().quadtree().getAllItems().isEmpty()) {
        if (KMessageBox::ButtonCode::PrimaryAction
            == KMessageBox::questionTwoActions(this, tr("Do you want to close?"), tr("Close"), KStandardGuiItem::ok(), KStandardGuiItem::cancel())) {
            e->accept();
        } else {
            e->ignore();
        }
        return;
    } else {
        e->accept();
    }
}

void MainWindow::applyCustomStyles()
{
    QFile file(u":/styles/style.qss"_s);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        const QString qss = stream.readAll();
        setStyleSheet(qss);
    } else {
        qCWarning(DRAWY_LOG) << "Failed to load stylesheet.";
    }

    int fontID = QFontDatabase::addApplicationFont(u":/fonts/FuzzyBubbles.ttf"_s);
    if (fontID == -1) {
        qCWarning(DRAWY_LOG) << "Failed to load font: FuzzyBubbles";
    }

    fontID = QFontDatabase::addApplicationFont(u":/fonts/Inter.ttf"_s);
    if (fontID == -1) {
        qCWarning(DRAWY_LOG) << "Failed to load font: Inter";
    }
}

void MainWindow::viewFullScreen(bool fullScreen)
{
    if (fullScreen) {
        setWindowState(windowState() | Qt::WindowFullScreen);
    } else {
        setWindowState(windowState() & ~Qt::WindowFullScreen);
    }
}

void MainWindow::loadFile(const QString &fileName)
{
    ActionManager &actionManager{ApplicationContext::instance()->uiContext().actionManager()};
    actionManager.loadFile(fileName);
}

#include "moc_window.cpp"
