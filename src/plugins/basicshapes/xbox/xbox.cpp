// SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "xbox.hpp"
#include "basicshapespluginutils.hpp"
#include "item/itemutils.hpp"
#include "xboxdeserializer.hpp"
#include "xboxserializer.hpp"
#include <KLocalizedString>
#include <QJsonObject>
#include <QPainterPath>
using namespace Qt::Literals::StringLiterals;

XBoxItem::XBoxItem()
{
}

void XBoxItem::drawItem(QPainter &painter, const QPointF &offset) const
{
    prepareBackground(painter);
    painter.drawRect(QRectF(start() - offset, end() - offset));
    painter.drawLine(start() - offset, end() - offset);
    painter.drawLine(QPointF(end().x(), start().y()) - offset, QPointF(start().x(), end().y()) - offset);
}

bool XBoxItem::intersects(const QRectF &rect)
{
    if (!boundingBox().intersects(rect)) {
        return false;
    }

    QPainterPath path;
    path.addRect(QRectF{start(), end()});
    path.moveTo(start());
    path.lineTo(end());
    path.moveTo(QPointF(end().x(), start().y()));
    path.lineTo(QPointF(start().x(), end().y()));

    path = m_transform.map(path);

    if (isFilled()) {
        return path.intersects(rect);
    }

    return path.intersects(rect) && !path.contains(rect);
}

Item::FormType XBoxItem::formType() const
{
    return Item::FormType::Custom;
}

QJsonObject XBoxItem::serialize(int zorder) const
{
    const XBoxSerializer serialize(this);
    return serialize.serialize(zorder);
}

void XBoxItem::deserialize(const QJsonObject &obj)
{
    XBoxDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

bool XBoxItem::operator==(const XBoxItem &other) const
{
    return start() == other.start() && PolygonItem::operator==(other);
}

PluginForm::PluginFormInfo XBoxItem::pluginFormInfo()
{
    const PluginForm::PluginFormInfo info{
        .useCustomIcon = true,
        .pluginName = BasicShapesPluginUtils::pluginName(),
        .toolTip = i18nc("@info:tooltip", "X Box"),
        .name = BasicShapesPluginUtils::convertStandardFormPluginTypeToString(BasicShapesPluginUtils::BasicShapesPluginType::XBox),
        .iconName = u"draw-xbox"_s,
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
