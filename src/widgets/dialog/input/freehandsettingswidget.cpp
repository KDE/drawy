/*
 * SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "freehandsettingswidget.hpp"
#include "common/utils/freehand.hpp"
#include "drawyglobalconfig.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QSlider>

FreehandSettingsWidget::FreehandSettingsWidget(QWidget *parent)
    : ConfigGroupInterface{parent, i18n("Freehand Settings")}
    , m_streamlineSlider{new QSlider(Qt::Horizontal, this)}
    , m_thinningSlider{new QSlider(Qt::Horizontal, this)}
{
    auto formLayout = new QFormLayout;
    formLayout->setContentsMargins({});
    m_layout->addLayout(formLayout);
    auto setupSliderRow = [this, formLayout](QSlider *slider, const QString &name, const QString &infoText) {
        slider->setMinimum(0);
        slider->setMaximum(10);
        slider->setSingleStep(1);
        slider->setPageStep(1);

        auto rowLayout = new QHBoxLayout();

        auto nameLabel = new QLabel(name, this);
        nameLabel->setToolTip(infoText);
        nameLabel->setCursor(Qt::WhatsThisCursor);

        auto valueLabel = new QLabel(QString::number(slider->value()), this);
        valueLabel->setMinimumWidth(20);

        connect(slider, &QSlider::valueChanged, this, [valueLabel](int val) {
            valueLabel->setText(QString::number(val / 10.0));
        });

        rowLayout->addWidget(slider);
        rowLayout->addWidget(valueLabel);

        formLayout->addRow(nameLabel, rowLayout);
    };

    setupSliderRow(m_streamlineSlider, i18n("Stabilization"), i18n("Makes lines steadier by reducing hand shake."));
    setupSliderRow(m_thinningSlider, i18n("Pressure Sensitivity"), i18n("Controls how much pressure changes line thickness."));
}

FreehandSettingsWidget::~FreehandSettingsWidget() = default;

void FreehandSettingsWidget::save()
{
    DrawyGlobalConfig::self()->setStreamline(m_streamlineSlider->value() / 10.0);
    DrawyGlobalConfig::self()->setThinning(m_thinningSlider->value() / 10.0);
    DrawyGlobalConfig::self()->save();

    Common::Utils::Freehand::updateSettings();
}

void FreehandSettingsWidget::load()
{
    m_streamlineSlider->setValue(DrawyGlobalConfig::self()->streamline() * 10);
    m_thinningSlider->setValue(DrawyGlobalConfig::self()->thinning() * 10);
}

#include "moc_freehandsettingswidget.cpp"
