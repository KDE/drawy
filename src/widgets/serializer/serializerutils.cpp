/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "serializerutils.hpp"
#include <QJsonObject>
#include <QPointF>
using namespace Qt::Literals::StringLiterals;
int SerializerUtils::version()
{
    return 1;
}

QJsonObject SerializerUtils::toJson(const QPointF &point)
{
    QJsonObject result{};
    result[u"x"_s] = QJsonValue(point.x());
    result[u"y"_s] = QJsonValue(point.y());

    return result;
}
