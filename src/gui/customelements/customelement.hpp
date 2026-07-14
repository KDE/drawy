/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "item/item.hpp"
#include "libdrawygui_export.h"
#include <QString>
#include <memory>
class QDebug;
class QJsonObject;
class LIBDRAWYGUI_EXPORT CustomElement
{
public:
    CustomElement();
    ~CustomElement();

    void load(const QJsonObject &obj);
    [[nodiscard]] QJsonObject save() const;

    [[nodiscard]] qint64 timeStamp() const;

    [[nodiscard]] std::shared_ptr<Item> item() const;
    void setItem(const std::shared_ptr<Item> &newItem);

    [[nodiscard]] bool isValid() const;

private:
    qint64 mTimeStamp = -1;
    std::shared_ptr<Item> mItem;
};
LIBDRAWYGUI_EXPORT QDebug operator<<(QDebug d, const CustomElement &t);
