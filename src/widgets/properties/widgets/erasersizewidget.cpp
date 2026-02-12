// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "erasersizewidget.hpp"
#include "drawyglobalconfig.h"

#include <KLocalizedString>
#include <QSpinBox>

#include "properties/property.hpp"

EraserSizeWidget::EraserSizeWidget(QWidget *parent)
    : PropertyWidget{parent}
{
    auto box{new QSpinBox(parent)};
    box->setRange(1, 100);
    box->setValue(DrawyGlobalConfig::self()->eraserSize());

    box->hide();
    m_widget = box;

    connect(box, &QSpinBox::valueChanged, this, [this]() {
        DrawyGlobalConfig::self()->setEraserSize(static_cast<QSpinBox *>(m_widget)->value());
        DrawyGlobalConfig::self()->save();
        Q_EMIT changed(value());
    });
}

QString EraserSizeWidget::name() const
{
    return i18n("Eraser Size");
}

const Property EraserSizeWidget::value() const
{
    return Property{static_cast<QSpinBox *>(m_widget)->value(), Property::Type::EraserSize};
}

#include "moc_erasersizewidget.cpp"
