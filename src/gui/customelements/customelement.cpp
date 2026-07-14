/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "customelement.hpp"
#include "serializer/deserializeutils.hpp"
#include <QDateTime>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
CustomElement::CustomElement()
    : mTimeStamp(QDateTime::currentSecsSinceEpoch())
{
}

CustomElement::~CustomElement() = default;

bool CustomElement::isValid() const
{
    return mItem && mTimeStamp != -1;
}

void CustomElement::load(const QJsonObject &obj)
{
    mTimeStamp = obj["timestamp"_L1].toInteger(-1);
    mItem = DeserializeUtils::deserializeItem(obj);
}

QJsonObject CustomElement::save() const
{
    QJsonObject obj = mItem->serialize(-1);
    obj["timestamp"_L1] = mTimeStamp;
    return obj;
}

qint64 CustomElement::timeStamp() const
{
    return mTimeStamp;
}

std::shared_ptr<Item> CustomElement::item() const
{
    return mItem;
}

void CustomElement::setItem(const std::shared_ptr<Item> &newItem)
{
    mItem = newItem;
}

QDebug operator<<(QDebug d, const CustomElement &t)
{
    d.space() << "timestamp:" << t.timeStamp();
    d.space() << "item:" << *t.item();
    return d;
}
