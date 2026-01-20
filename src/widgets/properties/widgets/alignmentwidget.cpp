/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "alignmentwidget.hpp"
#include "buttonactionswidget.hpp"
#include "context/applicationcontext.hpp"
#include "context/uicontext.hpp"
#include "iconmanager/iconmanager.hpp"
#include <QHBoxLayout>
AlignmentWidget::AlignmentWidget(QWidget *parent)
    : PropertyWidget(parent)
{
    m_widget = new QWidget{parent};

    auto layout{new QHBoxLayout(m_widget)};
    layout->setContentsMargins({});
    m_widget->hide();
}

QString AlignmentWidget::name() const
{
    return tr("Alignment");
}

const Property AlignmentWidget::value() const
{
    return {};
}

#include "moc_alignmentwidget.cpp"
