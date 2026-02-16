// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "freeform.hpp"

#include <QJsonObject>
#include <qlogging.h>

#include "common/constants.hpp"
#include "common/utils/freehand.hpp"
#include "itemutils.hpp"
#include "serializer/freeformdeserializer.hpp"
#include "serializer/freeformserializer.hpp"

FreeformItem::FreeformItem()
{
    m_properties[Property::Type::StrokeWidth] = Property{1, Property::Type::StrokeWidth};
    m_properties[Property::Type::StrokeColor] = Property{QColor(Qt::black), Property::Type::StrokeColor};
    m_properties[Property::Type::Opacity] = Property{Common::maxItemOpacity, Property::Type::Opacity};

    m_properties[Property::Type::StrokeStyle] = Property{ItemUtils::convertItemStrokeTypeEnumToString(Item::StrokeType::Solid), Property::Type::StrokeStyle};
}

int FreeformItem::minPointDistance()
{
    return 4;
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
    if (m_pointBuffer.empty()) {
        return;
    }

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
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
    m_path.setCachingEnabled(true);
#endif

    m_boundingBox = m_path.boundingRect().normalized();
}

bool FreeformItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect)) {
        return false;
    }

    return m_transform.map(m_path).intersects(rect);
}

void FreeformItem::draw(QPainter &painter, const QPointF &offset)
{
    finalizeStroke();

    QColor color{property(Property::Type::StrokeColor).value<QColor>()};
    const int alpha{property(Property::Type::Opacity).value<int>()};
    color.setAlpha(alpha);

    // if the stroke style is not solid, we draw a polyline
    if (!isSolid()) {
        drawNonSolidStroke(painter, offset);
        return;
    }

    // We'll be drawing a polygon. We don't want it to have an outline.
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);

    drawItem(painter, offset);
}

bool FreeformItem::isSolid() const
{
    return property(Property::Type::StrokeStyle).value<QString>() == ItemUtils::convertItemStrokeTypeEnumToString(Item::StrokeType::Solid);
}

void FreeformItem::drawNonSolidStroke(QPainter &painter, const QPointF &offset, bool drawBuffer) const
{
    QColor color{property(Property::Type::StrokeColor).value<QColor>()};
    const int alpha{property(Property::Type::Opacity).value<int>()};
    color.setAlpha(alpha);

    QPen pen{};
    pen.setStyle(ItemUtils::convertItemStrokeTypeStringToPenStyle(property(Property::Type::StrokeStyle).value<QString>()));
    pen.setColor(color);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(property(Property::Type::StrokeWidth).value<qreal>());

    painter.save();
    painter.setPen(pen);

    using namespace Common::Utils::Freehand;

    painter.translate(-offset);
    if (drawBuffer) {
        if (m_pointBuffer.size() == 1) {
            painter.drawPoint(m_pointBuffer.front());
        } else {
            painter.drawPath(getStrokeOutline(getStrokePoints(m_pointBuffer, m_pressureBuffer, false)));
        }
    } else {
        if (m_points.size() == 1) {
            painter.drawPoint(m_points.front());
        } else {
            painter.drawPath(getStrokeOutline(getStrokePoints(m_points, m_pressures, false)));
        }
    }

    painter.restore();
}

bool FreeformItem::isBufferFull() const
{
    return m_pointBuffer.size() >= m_maxBufferSize;
}

void FreeformItem::drawBuffer(QPainter &painter, const QPointF &offset) const
{
    // if the stroke style is not solid, we draw a polyline
    if (!isSolid()) {
        drawNonSolidStroke(painter, offset, true);
        return;
    }

    const qreal thickness{property(Property::Type::StrokeWidth).value<qreal>()};
    QColor color{property(Property::Type::StrokeColor).value<QColor>()};
    const int alpha{property(Property::Type::Opacity).value<int>()};
    color.setAlpha(alpha);

    const QPainterPath path{Common::Utils::Freehand::getStroke(m_pointBuffer, m_pressureBuffer, m_simulatePressure, thickness)};

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.translate(-offset);
    painter.drawPath(path);
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

Item::FormType FreeformItem::formType() const
{
    return Item::FormType::Freeform;
}

const QList<QPointF> &FreeformItem::points() const
{
    return m_points;
}

const QList<qreal> &FreeformItem::pressures() const
{
    return m_pressures;
}

QJsonObject FreeformItem::serialize(int zorder) const
{
    const FreeFormSerializer serialize(this);
    return serialize.serialize(zorder);
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

void FreeformItem::normalize()
{
    const QPointF topLeft{m_path.boundingRect().topLeft()};
    for (auto &point : m_points) {
        point -= topLeft;
    }

    m_path.translate(-topLeft);
    m_boundingBox.translate(-topLeft);
    m_transform.translate(topLeft.x(), topLeft.y());
}

void FreeformItem::commitTransformation()
{
    for (auto &point : m_points) {
        point = m_transform.map(point);
    }

    const qreal thickness{property(Property::Type::StrokeWidth).value<qreal>()};

    m_path = Common::Utils::Freehand::getStroke(m_points, m_pressures, m_simulatePressure, thickness);
    m_boundingBox = m_path.boundingRect().normalized();
    m_transform = {};

    setDirty(true);
}

QDebug operator<<(QDebug d, const FreeformItem &t)
{
    d.space() << "points:" << t.points();
    d.space() << "pressures:" << t.pressures();
    d.space() << "Item: " << static_cast<const Item &>(t);
    return d;
}
