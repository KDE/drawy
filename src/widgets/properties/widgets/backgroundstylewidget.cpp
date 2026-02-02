/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "backgroundstylewidget.hpp"
#include "buttonactionswidget.hpp"
#include <QHBoxLayout>
BackgroundStyleWidget::BackgroundStyleWidget(QWidget *parent)
    : PropertyWidget(parent)
{
    m_widget = new QWidget{parent};

    auto layout{new QHBoxLayout(m_widget)};
    layout->setContentsMargins({});
    m_widget->hide();
}

QString BackgroundStyleWidget::name() const
{
    return tr("Background Style");
}

const Property BackgroundStyleWidget::value() const
{
    return {};
}

#include "moc_backgroundstylewidget.cpp"
