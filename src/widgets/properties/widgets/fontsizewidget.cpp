// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fontsizewidget.hpp"
#include "drawyglobalconfig.h"

#include "properties/property.hpp"
#include <KLocalizedString>
#include <QSpinBox>

FontSizeWidget::FontSizeWidget(QWidget *parent)
    : PropertyWidget{parent}
    , mSpinBox(new QSpinBox(parent))
{
    // TODO: Remove magic numbers
    mSpinBox->setMinimum(1);
    mSpinBox->setValue(DrawyGlobalConfig::self()->fontSize());

    mSpinBox->hide();
    m_widget = mSpinBox;

    connect(mSpinBox, &QSpinBox::valueChanged, this, [this]() {
        DrawyGlobalConfig::self()->setFontSize(static_cast<QSpinBox *>(mSpinBox)->value());
        DrawyGlobalConfig::self()->save();
        Q_EMIT changed(value());
    });
}

void FontSizeWidget::setValue(int value)
{
    mSpinBox->setValue(value);
}

QString FontSizeWidget::name() const
{
    return i18n("Font Size");
}

const Property FontSizeWidget::value() const
{
    return Property{static_cast<QSpinBox *>(mSpinBox)->value(), Property::Type::FontSize};
}

#include "moc_fontsizewidget.cpp"
