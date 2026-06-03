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
    , mSpinBox(new QSpinBox(parent))
{
    mSpinBox->setRange(1, 100);
    mSpinBox->setValue(DrawyGlobalConfig::self()->eraserSize());
    mSpinBox->hide();
    m_widget = mSpinBox;

    connect(mSpinBox, &QSpinBox::valueChanged, this, [this]() {
        DrawyGlobalConfig::self()->setEraserSize(mSpinBox->value());
        DrawyGlobalConfig::self()->save();
        Q_EMIT changed(value());
    });
}

QString EraserSizeWidget::name() const
{
    return i18n("Eraser Size");
}

Property EraserSizeWidget::value() const
{
    return Property{mSpinBox->value(), Property::Type::EraserSize};
}

#include "moc_erasersizewidget.cpp"
