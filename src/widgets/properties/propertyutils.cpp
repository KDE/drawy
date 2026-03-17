/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "propertyutils.hpp"

QList<QColor> PropertyUtils::strokeDefaultColors()
{
    const QList<QColor> colors{QColor{255, 255, 255}, QColor{0, 0, 0}, QColor{255, 53, 71}, QColor{255, 187, 51}, QColor{51, 181, 229}};
    return colors;
}

QList<QColor> PropertyUtils::backgroundDefaultColors()
{
    const QList<QColor> colors{QColor{QColor{255, 255, 255}}, QColor{0, 0, 0}, QColor{255, 53, 71}, QColor{255, 187, 51}, QColor{51, 181, 229}};
    return colors;
}
