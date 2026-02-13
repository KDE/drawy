/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include "item/polygon.hpp"
#include "libstandardformplugin_export.h"
#include "pluginform/pluginform.hpp"
class QPainterPath;
class LIBSTANDARDFORMPLUGIN_EXPORT TriangleItem : public PolygonItem
{
public:
    TriangleItem();
    ~TriangleItem() override = default;

    [[nodiscard]] bool intersects(const QRectF &rect) override;

    [[nodiscard]] Item::FormType formType() const override;

    [[nodiscard]] QJsonObject serialize(int zorder) const override;
    void deserialize(const QJsonObject &obj) override;

    [[nodiscard]] bool operator==(const TriangleItem &other) const;

    [[nodiscard]] static PluginForm::PluginFormInfo pluginFormInfo();

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

    [[nodiscard]] QPainterPath getPath() const;
};
