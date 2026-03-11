// SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "triangle.hpp"
#include "basicshapespluginutils.hpp"
#include "item/itemutils.hpp"
#include "triangledeserializer.hpp"
#include "triangleserializer.hpp"
#include <KLocalizedString>
#include <QJsonObject>
#include <QPainterPath>
using namespace Qt::Literals::StringLiterals;

TriangleItem::TriangleItem()
{
    m_properties[Property::Type::BackgroundColor] = Property{QColor(Qt::transparent), Property::Type::BackgroundColor};
    m_properties[Property::Type::BackgroundStyle] =
        Property{ItemUtils::convertItemBackgroundTypeEnumToString(Item::BackgroundType::Solid), Property::Type::BackgroundStyle};
}

QPainterPath TriangleItem::getPath() const
{
    const QPointF startPt = start();
    const QPointF endPt = end();
    const QPointF center = (startPt + endPt) / 2.0;

    const QPointF topPoint = QPointF(center.x(), startPt.y());
    const QPointF rightPoint = QPointF(endPt.x(), endPt.y());
    const QPointF leftPoint = QPointF(startPt.x(), endPt.y());

    const QPolygonF triangle({topPoint, rightPoint, leftPoint});

    QPainterPath painterPath;
    painterPath.addPolygon(triangle);
    painterPath.closeSubpath();

    return painterPath;
}

void TriangleItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    painter.save();
    painter.translate(-offset);

    prepareBackground(painter);

    painter.drawPath(getPath());

    painter.restore();
}

bool TriangleItem::intersects(const QRectF &rect)
{
    const QPainterPath path = getPath();
    if (isFilled()) {
        return path.intersects(rect);
    }
    return path.intersects(rect) && !path.contains(rect);
}

Item::FormType TriangleItem::formType() const
{
    return Item::FormType::Custom;
}

QJsonObject TriangleItem::serialize(int zorder) const
{
    const TriangleSerializer serialize(this);
    return serialize.serialize(zorder);
}

void TriangleItem::deserialize(const QJsonObject &obj)
{
    TriangleDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

bool TriangleItem::operator==(const TriangleItem &other) const
{
    return start() == other.start() && PolygonItem::operator==(other);
}

PluginForm::PluginFormInfo TriangleItem::pluginFormInfo()
{
    const PluginForm::PluginFormInfo info{
        .pluginName = BasicShapesPluginUtils::pluginName(),
        .toolTip = i18nc("@info:tooltip", "Triangle"),
        .name = BasicShapesPluginUtils::convertStandardFormPluginTypeToString(BasicShapesPluginUtils::BasicShapesPluginType::Triangle),
        .iconName = u"tool_brush"_s, // Just test. Icon is not ok
        .properties = {
            Property::Type::BackgroundColor,
            Property::Type::BackgroundStyle,
            Property::Type::StrokeWidth,
            Property::Type::StrokeColor,
            Property::Type::StrokeStyle,
            Property::Type::Opacity,
        }};
    return info;
}
