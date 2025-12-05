// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "window.hpp"

#include <QButtonGroup>
#include <QFile>
#include <QFontDatabase>
#include <QShortcut>

#include "../canvas/canvas.hpp"
#include "../components/actionbar.hpp"
#include "../components/propertybar.hpp"
#include "../components/toolbar.hpp"
#include "../context/applicationcontext.hpp"
#include "../context/renderingcontext.hpp"
#include "../context/uicontext.hpp"
#include "../controller/controller.hpp"
#include "boardlayout.hpp"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    this->m_applyCustomStyles();

    BoardLayout *layout{new BoardLayout(this)};
    Controller *controller{new Controller(this)};
    ApplicationContext *context{ApplicationContext::instance()};

    RenderingContext &renderingContext{context->renderingContext()};
    UIContext &uiContext{context->uiContext()};

    renderingContext.canvas().setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->setLayout(layout);

    layout->setMargins(10);
    layout->setLeftWidget(&uiContext.propertyBar());
    layout->setTopWidget(&uiContext.toolBar());
    layout->setBottomWidget(&uiContext.actionBar());
    layout->setCentralWidget(&renderingContext.canvas());

    QObject::connect(&renderingContext.canvas(),
                     &Canvas::mousePressed,
                     controller,
                     &Controller::mousePressed);
    QObject::connect(&renderingContext.canvas(),
                     &Canvas::mouseMoved,
                     controller,
                     &Controller::mouseMoved);
    QObject::connect(&renderingContext.canvas(),
                     &Canvas::mouseReleased,
                     controller,
                     &Controller::mouseReleased);
    QObject::connect(&renderingContext.canvas(),
                     &Canvas::keyPressed,
                     controller,
                     &Controller::keyPressed);
    QObject::connect(&renderingContext.canvas(),
                     &Canvas::keyReleased,
                     controller,
                     &Controller::keyReleased);

    QObject::connect(&renderingContext.canvas(), &Canvas::tablet, controller, &Controller::tablet);
    QObject::connect(&renderingContext.canvas(), &Canvas::wheel, controller, &Controller::wheel);
    QObject::connect(&renderingContext.canvas(), &Canvas::leave, controller, &Controller::leave);

    m_applyCustomStyles();
}

MainWindow::~MainWindow() {
}

void MainWindow::m_applyCustomStyles() {
    QFile file(":/styles/style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString qss = stream.readAll();
        this->setStyleSheet(qss);
    } else {
        qWarning() << "Failed to load stylesheet.";
    }

    int fontID = QFontDatabase::addApplicationFont(":/fonts/FuzzyBubbles.ttf");
    if (fontID == -1) {
        qWarning() << "Failed to load font: FuzzyBubbles";
    }

    fontID = QFontDatabase::addApplicationFont(":/fonts/Inter.ttf");
    if (fontID == -1) {
        qWarning() << "Failed to load font: Inter";
    }
}

#include "moc_window.cpp"
