/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "alignmentwidget.hpp"

AlignmentWidget::AlignmentWidget(QWidget *parent)
    : PropertyWidget(parent)
{
}

QString AlignmentWidget::name() const
{
    return tr("Alignment");
}

const Property AlignmentWidget::value() const
{
    return {};
}
