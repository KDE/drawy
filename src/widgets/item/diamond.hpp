/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "libdrawywidgets_private_export.h"

#include "polygon.hpp"

class LIBDRAWYWIDGETS_TESTS_EXPORT DiamondItem : public PolygonItem
{
public:
    DiamondItem();
    ~DiamondItem() override = default;

    [[nodiscard]] bool intersects(const QRectF &rect) override;
    [[nodiscard]] bool intersects(const QLineF &rect) override;

    [[nodiscard]] Item::Type type() const override;

    [[nodiscard]] QJsonObject serialize() const override;
    void deserialize(const QJsonObject &obj) override;

    [[nodiscard]] bool operator==(const DiamondItem &other) const;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;
};
