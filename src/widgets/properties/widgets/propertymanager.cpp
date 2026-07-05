// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "propertymanager.hpp"

#include "actionswidget.hpp"
#include "backgroundcolorwidget.hpp"
#include "endarrowtypewidget.hpp"
#include "erasersizewidget.hpp"
#include "fontsizewidget.hpp"
#include "keybindings/actionmanager.hpp"
#include "opacitywidget.hpp"
#include "properties/widgets/alignmentwidget.hpp"
#include "properties/widgets/backgroundstylewidget.hpp"
#include "properties/widgets/cornerrectangletypewidget.hpp"
#include "properties/widgets/stokestylewidget.hpp"
#include "properties/widgets/zorderwidget.hpp"
#include "startarrowtypewidget.hpp"
#include "strokecolorwidget.hpp"
#include "strokewidthwidget.hpp"

PropertyManager::PropertyManager(ActionManager *actionManager, QWidget *parent)
    : QObject{parent}
{
    m_widgets[Property::Type::StrokeWidth] = new StrokeWidthWidget(parent);
    m_widgets[Property::Type::StrokeColor] = new StrokeColorWidget(parent);
    m_widgets[Property::Type::StrokeStyle] = new StokeStyleWidget(parent);
    m_widgets[Property::Type::EraserSize] = new EraserSizeWidget(parent);
    m_widgets[Property::Type::FontSize] = new FontSizeWidget(parent);
    m_widgets[Property::Type::Actions] = new ActionsWidget(actionManager, parent);
    m_widgets[Property::Type::BackgroundColor] = new BackgroundColorWidget(parent);
    m_widgets[Property::Type::Opacity] = new OpacityWidget(parent);
    m_widgets[Property::Type::ZOrder] = new ZOrderWidget(actionManager, parent);
    m_widgets[Property::Type::Alignment] = new AlignmentWidget(actionManager, parent);
    m_widgets[Property::Type::BackgroundStyle] = new BackgroundStyleWidget(parent);
    m_widgets[Property::Type::StartArrowType] = new StartArrowTypeWidget(parent);
    m_widgets[Property::Type::EndArrowType] = new EndArrowTypeWidget(parent);
    m_widgets[Property::Type::CornerRectangleType] = new CornerRectangleTypeWidget(parent);

    for (auto i = m_widgets.cbegin(), end = m_widgets.cend(); i != end; ++i) {
        connect(i.value(), &PropertyWidget::changed, this, &PropertyManager::propertyUpdated);
    }
}

PropertyManager::~PropertyManager()
{
    qDeleteAll(m_widgets);
}

PropertyWidget *PropertyManager::widget(const Property::Type type) const
{
    if (!m_widgets.contains(type)) {
        throw std::logic_error("A valid widget for the given Property::Type does not exist.");
    }

    return m_widgets.value(type);
}

Property PropertyManager::value(const Property::Type type) const
{
    return widget(type)->value();
}

void PropertyManager::slotUpdateWidgets()
{
    for (const auto &widget : std::as_const(m_widgets)) {
        widget->updateWidget();
    }
}

#include "moc_propertymanager.cpp"
