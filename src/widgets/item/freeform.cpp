// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "freeform.hpp"

#include <QDateTime>
#include <QJsonObject>
#include <qnamespace.h>

#include "common/constants.hpp"
#include "common/utils/freehand.hpp"
#include "common/utils/math.hpp"
#include "item/itemutils.hpp"
#include "serializer/freeformdeserializer.hpp"
#include "serializer/freeformserializer.hpp"

FreeformItem::FreeformItem()
{
    m_properties[Property::Type::StrokeWidth] = Property{1, Property::Type::StrokeWidth};
    m_properties[Property::Type::StrokeColor] = Property{QColor(Qt::black), Property::Type::StrokeColor};
    m_properties[Property::Type::Opacity] = Property{Common::maxItemOpacity, Property::Type::Opacity};
    // Laurent not supported yet
    // m_properties[Property::Type::StrokeStyle] =
    // Property{ItemUtils::convertItemStrokeTypeEnumToString(Item::StrokeType::Solid),
    // Property::Type::StrokeStyle};
}

int FreeformItem::minPointDistance()
{
    return 0;
}

void FreeformItem::addPoint(const QPointF &point, const qreal pressure)
{
    QPointF newPoint{point};
    m_points.push_back(newPoint);
    m_pressures.push_back(pressure);

    using namespace Common::Utils::Freehand;
    m_path = getStrokePath(getStrokePolygon(getStrokePoints(m_points, m_pressures, m_simulatePressure)));
    m_boundingBox = m_path.boundingRect().normalized();

    setDirty(true);
}

bool FreeformItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect))
        return false;

    return m_path.intersects(rect);
}

bool FreeformItem::intersects(const QLineF &line)
{
    QPainterPath linePath{};
    linePath.moveTo(line.p1());
    linePath.lineTo(line.p2());

    return m_path.intersects(linePath);
}

void FreeformItem::draw(QPainter &painter, const QPointF &offset)
{
    QColor color{property(Property::Type::StrokeColor).value<QColor>()};
    const int alpha{property(Property::Type::Opacity).value<int>()};
    color.setAlpha(alpha);

    // Laurent we can't support it at the moment:
    // pen.setStyle(ItemUtils::convertItemStrokeTypeStringToPenStyle(property(Property::Type::StrokeStyle).value<QString>()));

    // We'll be drawing a polygon. We don't want it to have an outline.
    QPen pen{};
    pen.setWidth(0);
    pen.setColor(Qt::transparent);

    painter.setBrush(color);
    painter.setPen(pen);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    drawItem(painter, offset);
}

void FreeformItem::erase(QPainter &painter, const QPointF &offset) const
{
    painter.save();
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(boundingBox().translated(-offset), Qt::transparent);
    painter.restore();
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
    painter.save();
    painter.translate(-offset);
    painter.drawPath(m_path);

    // UNCOMMENT TO SEE THE POLYGON'S STRUCTURE
    // QFont ft{};
    // ft.setPixelSize(1);
    // painter.setFont(ft);

    // QPen pen; pen.setWidthF(0.25); pen.setCapStyle(Qt::RoundCap); pen.setColor(Qt::red);
    // painter.setPen(pen);
    // for (auto &pt : polygon) {
    //     painter.drawPoint(pt);
    // }

    // i = 0;
    // painter.setPen(Qt::blue);
    // for (auto &pt : points) {
    //     painter.drawText(pt.point, QString::asprintf("%d", i++));
    // }

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

    using namespace Common::Utils::Freehand;
    m_path = getStrokePath(getStrokePolygon(getStrokePoints(m_points, m_pressures, m_simulatePressure)));

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

bool FreeformItem::isPressureSimulated() const
{
    return m_simulatePressure;
}

void FreeformItem::setSimulatePressure(bool value)
{
    m_simulatePressure = value;
}

QDebug operator<<(QDebug d, const FreeformItem &t)
{
    d.space() << "points:" << t.points();
    d.space() << "pressures:" << t.pressures();
    d.space() << "Item: " << static_cast<const Item &>(t);
    return d;
}
