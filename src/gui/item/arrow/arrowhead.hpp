// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QPainter>
#include <QPainterPath>

class ArrowHead
{
    Q_GADGET
public:
    enum class Type {
        None,
        Open,
        Unfilled,
        Filled,
        Circle,
        Diamond
    };

    Q_ENUM(Type);

public:
    ArrowHead(const QPointF &start, const QPointF &end);
    virtual ~ArrowHead();

    void setBounds(const QPointF &start, const QPointF &end);

    virtual QPainterPath getPath() = 0;
    virtual bool isFilled() const = 0;
    virtual Type type() const = 0;

public:
    static std::shared_ptr<ArrowHead> create(ArrowHead::Type arrowType, const QPointF &start, const QPointF &end);

protected:
    QPointF m_start;
    QPointF m_end;
    QPainterPath m_path;
};
