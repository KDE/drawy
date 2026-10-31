/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "pageswidget.hpp"
#include "common/constants.hpp"
#include "keybindings/actionmanager.hpp"
#include "pageslistview.hpp"
#include <KLocalizedString>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
PagesWidget::PagesWidget(ActionManager *actionManager, QAbstractItemModel *model, QWidget *parent)
    : QWidget{parent}
    , mListView(new PagesListView(actionManager, this))
    , mNewPageButton(new QToolButton(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mListView->setObjectName(u"mListView"_s);
    mListView->setModel(model);
    mainLayout->addWidget(mListView);

    const int iconWidth{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    const QSize iconSize{iconWidth, iconWidth};

    mNewPageButton->setAutoRaise(true);
    mNewPageButton->setIconSize(iconSize);
    mNewPageButton->setDefaultAction(actionManager->action(ActionManager::Action::NewPage));
    mNewPageButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mNewPageButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mNewPageButton->setMinimumHeight(mNewPageButton->sizeHint().height());
    mainLayout->addWidget(mNewPageButton);

    setFocusProxy(mListView);
}

PagesWidget::~PagesWidget() = default;

QSize PagesWidget::sizeHint() const
{
    const int listHeight{mListView->sizeHint().height()};
    const int buttonHeight{mNewPageButton ? mNewPageButton->sizeHint().height() : 0};
    const int spacing{layout() ? layout()->spacing() : 0};
    return {Common::pagesPopupWidth, listHeight + spacing + buttonHeight};
}

#include "moc_pageswidget.cpp"
