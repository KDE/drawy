// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawygui_export.h"

#include "polygon.hpp"

class LIBDRAWYGUI_EXPORT RectangleItem : public PolygonItem
{
public:
    RectangleItem();
    ~RectangleItem() override = default;

    [[nodiscard]] bool intersects(const QRectF &rect) override;

    [[nodiscard]] Item::FormType formType() const override;

    [[nodiscard]] QJsonObject serialize(int zorder) const override;
    void deserialize(const QJsonObject &obj) override;

    [[nodiscard]] bool operator==(const RectangleItem &other) const;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;
};
