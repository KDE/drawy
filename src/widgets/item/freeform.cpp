// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "freeform.hpp"

#include <QDateTime>
#include <QJsonObject>

#include "common/constants.hpp"
#include "common/utils/freehand.hpp"
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
    if (m_pointBuffer.size() >= m_maxBufferSize) {
        finalizeStroke();

        m_pointBuffer.push_back(m_points.back());
        m_pressureBuffer.push_back(m_pressures.back());
    }

    m_pointBuffer.push_back(point);
    m_pressureBuffer.push_back(pressure);

    setDirty(true);
}

void FreeformItem::finalizeStroke()
{
    if (m_pointBuffer.empty())
        return;

    if (m_points.empty()) {
        m_points = m_pointBuffer;
        m_pressures = m_pressureBuffer;
    } else {
        m_points.append(m_pointBuffer.mid(1));
        m_pressures.append(m_pressureBuffer.mid(1));
    }

    m_pointBuffer.clear();
    m_pressureBuffer.clear();

    const qreal thickness{property(Property::Type::StrokeWidth).value<qreal>()};

    m_path = Common::Utils::Freehand::getStroke(m_points, m_pressures, m_simulatePressure, thickness);
    m_path.setCachingEnabled(true);

    m_boundingBox = m_path.boundingRect().normalized();
}

bool FreeformItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect))
        return false;

    return m_path.intersects(rect);
}

bool FreeformItem::intersects(const QLineF &line)
{
    QPainterPath linePath;
    linePath.moveTo(line.p1());
    linePath.lineTo(line.p2());

    return m_path.intersects(linePath);
}

void FreeformItem::draw(QPainter &painter, const QPointF &offset)
{
    finalizeStroke();

    QColor color{property(Property::Type::StrokeColor).value<QColor>()};
    const int alpha{property(Property::Type::Opacity).value<int>()};
    color.setAlpha(alpha);

    // Laurent we can't support it at the moment:
    // pen.setStyle(ItemUtils::convertItemStrokeTypeStringToPenStyle(property(Property::Type::StrokeStyle).value<QString>()));

    // We'll be drawing a polygon. We don't want it to have an outline.
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);

    drawItem(painter, offset);
}

bool FreeformItem::isBufferFull() const
{
    return m_pointBuffer.size() >= m_maxBufferSize;
}

void FreeformItem::drawBuffer(QPainter &painter, const QPointF &offset) const
{
    const qreal thickness{property(Property::Type::StrokeWidth).value<qreal>()};
    QPainterPath path{Common::Utils::Freehand::getStroke(m_pointBuffer, m_pressureBuffer, m_simulatePressure, thickness)};

    QColor color{property(Property::Type::StrokeColor).value<QColor>()};
    const int alpha{property(Property::Type::Opacity).value<int>()};
    color.setAlpha(alpha);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.translate(-offset);
    painter.drawPath(path);
}

void FreeformItem::erase(QPainter &painter, const QPointF &offset) const
{
    painter.save();
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(boundingBox().translated(-offset), Qt::transparent);
    painter.restore();
}

void FreeformItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.save();
    painter.translate(-offset);
    painter.drawPath(m_path);

    // UNCOMMENT TO SEE THE POLYGON'S STRUCTURE
    // using namespace Common::Utils::Freehand;
    // const qreal
    // thickness{property(Property::Type::StrokeWidth).value<qreal>()}; const auto
    // polygon = getStrokePolygon(getStrokePoints(m_points, m_pressures,
    // m_simulatePressure), thickness);

    // QPen pen; pen.setWidthF(0.25); pen.setCapStyle(Qt::RoundCap);
    // pen.setColor(Qt::red); painter.setPen(pen); for (auto &pt : polygon) {
    //     painter.drawPoint(pt);
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

    m_path.translate(amount);
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
