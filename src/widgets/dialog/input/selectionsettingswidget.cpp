/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "selectionsettingswidget.hpp"
#include "drawyglobalconfig.h"
#include <KLocalizedString>
#include <QButtonGroup>
#include <QRadioButton>

SelectionSettingsWidget::SelectionSettingsWidget(QWidget *parent)
    : ConfigGroupInterface{parent, i18n("Selection Settings")}
    , m_fullyContained{new QRadioButton(i18nc("@option:radio", "Select items fully contained"), this)}
    , m_overlap{new QRadioButton(i18nc("@option:radio", "Select items on overlap"), this)}
{
    m_fullyContained->setToolTip(i18nc("@info:tooltip", "Only items completely inside the selection box will be selected"));
    m_overlap->setToolTip(i18nc("@info:tooltip", "Any item touching the selection box will be selected"));

    m_layout->addWidget(m_fullyContained);
    m_layout->addWidget(m_overlap);

    const auto buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(m_fullyContained);
    buttonGroup->addButton(m_overlap);
}

void SelectionSettingsWidget::save()
{
    if (m_overlap->isChecked()) {
        DrawyGlobalConfig::self()->setSelectionMode(DrawyGlobalConfig::EnumSelectionMode::Overlap);
    } else {
        DrawyGlobalConfig::self()->setSelectionMode(DrawyGlobalConfig::EnumSelectionMode::FullyContained);
    }
    DrawyGlobalConfig::self()->save();
}

void SelectionSettingsWidget::load()
{
    if (DrawyGlobalConfig::self()->selectionMode() == DrawyGlobalConfig::EnumSelectionMode::Overlap) {
        m_overlap->setChecked(true);
    } else {
        m_fullyContained->setChecked(true);
    }
}

#include "moc_selectionsettingswidget.cpp"
