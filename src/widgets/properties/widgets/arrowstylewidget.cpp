/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "arrowstylewidget.hpp"
#include "buttonactionswidget.hpp"
#include "context/applicationcontext.hpp"
#include "context/uicontext.hpp"
#include "iconmanager/iconmanager.hpp"
#include <QHBoxLayout>
ArrowStyleWidget::ArrowStyleWidget(QWidget *parent)
    : PropertyWidget(parent)
{
    m_widget = new QWidget{parent};

    auto layout{new QHBoxLayout(m_widget)};
    layout->setContentsMargins({});
    m_widget->hide();
}

QString ArrowStyleWidget::name() const
{
    return tr("Arrow");
}

const Property ArrowStyleWidget::value() const
{
    return {};
}

#include "moc_arrowstylewidget.cpp"
