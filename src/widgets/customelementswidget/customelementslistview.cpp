/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "customelementslistview.hpp"

CustomElementsListView::CustomElementsListView(QWidget *parent)
    : QListView(parent)
{
    setMouseTracking(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

CustomElementsListView::~CustomElementsListView() = default;

#include "moc_customelementslistview.cpp"
