// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawygui_export.h"

#include "item.hpp"

#include <QSizeF>

class QPixmap;

class LIBDRAWYGUI_EXPORT ImageItem : public Item
{
public:
    ImageItem();
    ~ImageItem() override = default;

    void draw(QPainter &painter, const QPointF &offset) override;

    void commitTransformation() override;

    [[nodiscard]] bool intersects(const QRectF &rect) override;

    [[nodiscard]] Item::FormType formType() const override;

    [[nodiscard]] QJsonObject serialize(int zorder) const override;
    void deserialize(const QJsonObject &obj) override;

    [[nodiscard]] const QPixmap &pixmap() const;
    void setPixmap(QPixmap pixmap);

    QSizeF pastedSize() const;

    void setBox(QRectF box);

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

private:
    QPixmap m_pixmap;
};
