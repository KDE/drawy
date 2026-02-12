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
{
    // TODO: Remove magic numbers
    auto box{new QSpinBox(parent)};
    box->setMinimum(1);
    box->setValue(DrawyGlobalConfig::self()->fontSize());

    box->hide();
    m_widget = box;

    connect(box, &QSpinBox::valueChanged, this, [this]() {
        DrawyGlobalConfig::self()->setFontSize(static_cast<QSpinBox *>(m_widget)->value());
        DrawyGlobalConfig::self()->save();
        Q_EMIT changed(value());
    });
}

QString FontSizeWidget::name() const
{
    return i18n("Font Size");
}

const Property FontSizeWidget::value() const
{
    return Property{static_cast<QSpinBox *>(m_widget)->value(), Property::Type::FontSize};
}

#include "moc_fontsizewidget.cpp"
