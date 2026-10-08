/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "pageswidget.hpp"
#include "keybindings/actionmanager.hpp"
#include "pageslistview.hpp"
#include <KLocalizedString>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
PagesWidget::PagesWidget(ActionManager *actionManager, QAbstractItemModel *model, QWidget *parent)
    : QWidget{parent}
    , mListView(new PagesListView(actionManager, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    auto headerLayout = new QHBoxLayout;
    headerLayout->setObjectName(u"headerLayout"_s);
    mainLayout->addLayout(headerLayout);

    auto label = new QLabel(i18nc("@label", "Pages"), this);
    label->setObjectName(u"label"_s);
    headerLayout->addWidget(label);
    headerLayout->addStretch();

    auto newPageButton = new QToolButton(this);
    newPageButton->setObjectName(u"newPageButton"_s);
    newPageButton->setAutoRaise(true);
    newPageButton->setDefaultAction(actionManager->action(ActionManager::Action::NewPage));
    headerLayout->addWidget(newPageButton);

    mListView->setObjectName(u"mListView"_s);
    mListView->setModel(model);
    mainLayout->addWidget(mListView);
    setFocusProxy(mListView);
}

PagesWidget::~PagesWidget() = default;

#include "moc_pageswidget.cpp"
