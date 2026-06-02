// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "strokewidthwidget.hpp"
#include "drawyglobalconfig.h"
#include <KLocalizedString>
#include <QSpinBox>

#include "properties/property.hpp"

StrokeWidthWidget::StrokeWidthWidget(QWidget *parent)
    : PropertyWidget{parent}
    , mSpinBox(new QSpinBox(parent))
{
    mSpinBox->setRange(1, 10);
    mSpinBox->setValue(DrawyGlobalConfig::self()->strokeWidth());
    mSpinBox->setKeyboardTracking(false);

    mSpinBox->hide();
    m_widget = mSpinBox;

    connect(mSpinBox, &QSpinBox::valueChanged, this, [this]() {
        DrawyGlobalConfig::self()->setStrokeWidth(mSpinBox->value());
        DrawyGlobalConfig::self()->save();
        Q_EMIT changed(value());
    });
}

QString StrokeWidthWidget::name() const
{
    return i18n("Thickness");
}

void StrokeWidthWidget::setValue(const QVariant &value)
{
    const QSignalBlocker blocker(mSpinBox);
    mSpinBox->setValue(value.toInt());
}

Property StrokeWidthWidget::value() const
{
    return Property{mSpinBox->value(), Property::Type::StrokeWidth};
}

#include "moc_strokewidthwidget.cpp"
