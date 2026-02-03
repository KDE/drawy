// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "topleftwidgets.hpp"
#include "context/applicationcontext.hpp"
#include "context/uicontext.hpp"
#include "frame.hpp"
#include "keybindings/actionmanager.hpp"
#include <QHBoxLayout>
#include <QStyle>
#include <QToolButton>

using namespace Qt::StringLiterals;
TopLeftWidgets::TopLeftWidgets(QWidget *parent)
    : QWidget{parent}
    , m_layout{new QHBoxLayout{this}}
{
    m_layout->setContentsMargins(0, 0, 0, 0);

    auto context{ApplicationContext::instance()};

    // menu button
    auto menuFrame{new Frame{this}};
    auto menuFrameLayout = new QHBoxLayout{menuFrame};

    const int iconWidth{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    const QSize iconSize{iconWidth, iconWidth};

    auto menuButton{new QToolButton{menuFrame}};
    menuButton->setIcon(QIcon::fromTheme(u"application-menu"_s));
    menuButton->setAutoRaise(true);
    menuButton->setIconSize(iconSize);
    menuButton->setToolTip(tr("Menu"));

    // separator
    auto vLine{new QFrame{menuFrame}};
    vLine->setFrameShape(QFrame::VLine);
    vLine->setFixedHeight(menuButton->height());

    // save controls
    auto saveButton{new QToolButton{menuFrame}};
    saveButton->setIcon(QIcon::fromTheme(u"document-save"_s));
    saveButton->setAutoRaise(true);
    saveButton->setIconSize(iconSize);
    saveButton->setToolTip(tr("Save"));

    connect(saveButton, &QToolButton::clicked, this, [context]() {
        auto actionManager{context->uiContext()->actionManager()};
        actionManager->saveToFile();
    });

    auto loadButton{new QToolButton{menuFrame}};
    loadButton->setIcon(QIcon::fromTheme(u"document-open"_s));
    loadButton->setAutoRaise(true);
    loadButton->setIconSize(iconSize);
    loadButton->setToolTip(tr("Load"));

    connect(loadButton, &QToolButton::clicked, this, [context]() {
        auto actionManager{context->uiContext()->actionManager()};
        actionManager->loadFromFile();
    });

    menuFrameLayout->addWidget(menuButton);
    menuFrameLayout->addWidget(vLine);
    menuFrameLayout->addWidget(saveButton);
    menuFrameLayout->addWidget(loadButton);

    m_layout->addWidget(menuFrame);
}
