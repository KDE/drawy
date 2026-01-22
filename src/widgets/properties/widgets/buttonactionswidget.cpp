/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "buttonactionswidget.hpp"
using namespace Qt::Literals::StringLiterals;

ButtonActionsWidget::ButtonActionsWidget(QWidget *parent)
    : QToolButton(parent)
{
    setProperty("class", u"drawyPropertyBarActionButton"_s);
}

ButtonActionsWidget::~ButtonActionsWidget() = default;

#include "moc_buttonactionswidget.cpp"
