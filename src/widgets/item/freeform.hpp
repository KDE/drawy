// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "libdrawywidgets_private_export.h"
#include <deque>
#include <memory>

#include "item.hpp"

class LIBDRAWYWIDGETS_TESTS_EXPORT FreeformItem : public Item, public std::enable_shared_from_this<FreeformItem>
{
public:
    FreeformItem();
    ~FreeformItem() override = default;
    FreeformItem(const FreeformItem &freeform);

    [[nodiscard]] static int minPointDistance();

    void draw(QPainter &painter, const QPointF &offset) override;
    void quickDraw(QPainter &painter, const QPointF &offset) const;

    [[nodiscard]] bool intersects(const QRectF &rect) override;
    [[nodiscard]] bool intersects(const QLineF &rect) override;

    void translate(const QPointF &amount) override;

    [[nodiscard]] QList<std::shared_ptr<Item>> split() const;
    [[nodiscard]] qsizetype size() const;
    [[nodiscard]] int maxSize() const;

    virtual void addPoint(const QPointF &point, const qreal pressure, bool optimize = true);

    [[nodiscard]] Item::Type type() const override;

    const QList<QPointF> &points() const;
    const QList<qreal> &pressures() const;

    [[nodiscard]] QJsonObject serialize() const override;
    void deserialize(const QJsonObject &obj) override;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;
    QList<QPointF> m_points{};
    QList<qreal> m_pressures{};

private:
    [[nodiscard]] LIBDRAWYWIDGETS_NO_EXPORT QPointF optimizePoint(const QPointF &newPoint);
    std::deque<QPointF> m_currentWindow;
    QPointF m_currentWindowSum{0, 0};
    int m_bufferSize{7};
};
LIBDRAWYWIDGETS_EXPORT QDebug operator<<(QDebug d, const FreeformItem &t);
