/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "customelementswidget.hpp"
#include "customelements/customelementsmanager.hpp"
#include "customelements/customelementsmodel.hpp"
#include <QListView>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
CustomElementsWidget::CustomElementsWidget(CustomElementsManager *manager, QWidget *parent)
    : QWidget{parent}
    , mListView(new QListView(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mListView->setObjectName(u"mListView"_s);
    if (manager) {
        mListView->setModel(manager->customElementsModel());
    }
    mainLayout->addWidget(mListView);
}

CustomElementsWidget::~CustomElementsWidget() = default;
#include "moc_customelementswidget.cpp"
