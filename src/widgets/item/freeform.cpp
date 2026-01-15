// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "freeform.hpp"

#include <QDateTime>
#include <QJsonObject>
#include <qnamespace.h>

#include "common/constants.hpp"
#include "common/utils/math.hpp"
#include "common/utils/freehand.hpp"
#include "item/itemutils.hpp"
#include "serializer/freeformdeserializer.hpp"
#include "serializer/freeformserializer.hpp"

FreeformItem::FreeformItem()
{
    m_properties[Property::Type::StrokeWidth] = Property{1, Property::Type::StrokeWidth};
    m_properties[Property::Type::StrokeColor] = Property{QColor(Qt::black), Property::Type::StrokeColor};
    m_properties[Property::Type::Opacity] = Property{Common::maxItemOpacity, Property::Type::Opacity};
    // Laurent not supported yet
    // m_properties[Property::Type::StrokeStyle] = Property{ItemUtils::convertItemStrokeTypeEnumToString(Item::StrokeType::Solid), Property::Type::StrokeStyle};
}

int FreeformItem::minPointDistance()
{
    return 0;
}

void FreeformItem::addPoint(const QPointF &point, const qreal pressure, bool optimize)
{
    QPointF newPoint{point};
    // if (optimize) {
    //     newPoint = optimizePoint(point);
    // }
    const double x = newPoint.x(), y = newPoint.y();

    m_boundingBox = m_boundingBox.normalized();
    const double topLeftX{m_boundingBox.topLeft().x()}, topLeftY{m_boundingBox.topLeft().y()};
    const double bottomRightX{m_boundingBox.bottomRight().x()}, bottomRightY{m_boundingBox.bottomRight().y()};
    const int mg{property(Property::Type::StrokeWidth).value<int>()};

    if (m_points.size() <= 1) {
        m_boundingBox.setTopLeft({x - mg, y - mg});
        m_boundingBox.setBottomRight({x + mg, y + mg});
    } else {
        m_boundingBox.setLeft(std::min(topLeftX, x - mg));
        m_boundingBox.setTop(std::min(topLeftY, y - mg));
        m_boundingBox.setRight(std::max(bottomRightX, x + mg));
        m_boundingBox.setBottom(std::max(bottomRightY, y + mg));
    }

    m_points.push_back(newPoint);
    m_pressures.push_back(pressure);
    setDirty(true);
}

bool FreeformItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect))
        return false;

    qsizetype pointsSize{m_points.size()};
    if (pointsSize == 1) {
        return rect.contains(m_points[0]);
    }

    QPointF p{rect.topLeft()};
    QPointF q{rect.topRight()};
    QPointF r{rect.bottomRight()};
    QPointF s{rect.bottomLeft()};

    for (qsizetype idx{0}; idx < pointsSize - 1; idx++) {
        QLine l{m_points[idx].toPoint(), m_points[idx + 1].toPoint()};

        if (Common::Utils::Math::intersects(l, QLineF{p, q}) || Common::Utils::Math::intersects(l, QLineF{q, r})
            || Common::Utils::Math::intersects(l, QLineF{r, s}) || Common::Utils::Math::intersects(l, QLineF{s, q}) || rect.contains(m_points[idx].toPoint())
            || rect.contains(m_points[idx + 1].toPoint()))
            return true;
    }

    return false;
}

bool FreeformItem::intersects(const QLineF &line)
{
    qsizetype pointSize{m_points.size()};
    for (qsizetype index{1}; index < pointSize; index++) {
        if (Common::Utils::Math::intersects(QLineF{m_points[index - 1], m_points[index]}, line)) {
            return true;
        }
    }
    return false;
}

void FreeformItem::draw(QPainter &painter, const QPointF &offset)
{
    QPen pen;

    QColor color{property(Property::Type::StrokeColor).value<QColor>()};
    const int alpha{property(Property::Type::Opacity).value<int>()};
    color.setAlpha(alpha);

    // Laurent we can't support it at the moment:
    // pen.setStyle(ItemUtils::convertItemStrokeTypeStringToPenStyle(property(Property::Type::StrokeStyle).value<QString>()));
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(property(Property::Type::StrokeWidth).value<int>());
    pen.setColor(color);

    painter.setPen(pen);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    drawItem(painter, offset);
}

void FreeformItem::erase(QPainter &painter, const QPointF &offset) const {
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(boundingBox().translated(-offset), Qt::transparent);
}

QPointF FreeformItem::optimizePoint(const QPointF &newPoint)
{
    m_currentWindow.push_back(newPoint);
    m_currentWindowSum += newPoint;

    if (m_currentWindow.size() > m_bufferSize) {
        m_currentWindowSum -= m_currentWindow.front();
        m_currentWindow.pop_front();
    }

    return m_currentWindowSum / static_cast<qreal>(m_currentWindow.size());
}

void FreeformItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    const QList<QPointF> points = Common::Utils::Freehand::getStrokePoints(m_points);

    painter.save();
    painter.translate(-offset);
    painter.drawPolyline(points);
    painter.restore();
}

qsizetype FreeformItem::size() const
{
    return m_points.size();
}

void FreeformItem::translate(const QPointF &amount)
{
    for (QPointF &point : m_points) {
        point += amount;
    }

    m_boundingBox.translate(amount);
}

Item::Type FreeformItem::type() const
{
    return Item::Type::Freeform;
}

const QList<QPointF> &FreeformItem::points() const
{
    return m_points;
}

const QList<qreal> &FreeformItem::pressures() const
{
    return m_pressures;
}

QJsonObject FreeformItem::serialize() const
{
    const FreeFormSerializer serialize(this);
    return serialize.serialize();
}

void FreeformItem::deserialize(const QJsonObject &obj)
{
    FreeformDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

bool FreeformItem::needsCaching() const
{
    return true;
}

QDebug operator<<(QDebug d, const FreeformItem &t)
{
    d.space() << "points:" << t.points();
    d.space() << "pressures:" << t.pressures();
    d.space() << "Item: " << static_cast<const Item &>(t);
    return d;
}
