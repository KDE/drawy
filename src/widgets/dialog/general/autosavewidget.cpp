// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "autosavewidget.hpp"
#include "dialog/general/configuregeneralwidget.hpp"

#include "drawyglobalconfig.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QStandardPaths>
#include <QToolButton>

using namespace Qt::StringLiterals;
AutoSaveWidget::AutoSaveWidget(QWidget *parent)
    : ConfigGroupInterface{parent, tr("Session")}
    , m_delay{new QSpinBox{this}}
    , m_pathWidget{new QWidget{this}}
    , m_path{new QLineEdit{m_pathWidget}}
    , m_enabled{new QCheckBox{this}}
{
    // delay
    m_delay->setMinimum(1);
    auto delayLabel{new QLabel(tr("Time interval for automatic saving"), this)};
    connect(m_delay, &QSpinBox::valueChanged, this, [this]() -> void {
        m_delay->setSuffix(i18np("minute", "minutes", m_delay->value()));
    });

    // path
    auto pathLabel{new QLabel(tr("Directory for automatic saving"), this)};
    auto pathLayout{new QHBoxLayout{m_pathWidget}};
    auto pathBrowseButton{new QToolButton{m_pathWidget}};
    pathBrowseButton->setIcon(QIcon::fromTheme(u"folder-open"_s));

    pathLayout->addWidget(m_path);
    pathLayout->addWidget(pathBrowseButton);
    pathLayout->setContentsMargins(0, 0, 0, 0);

    connect(pathBrowseButton, &QToolButton::clicked, this, [this]() -> void {
        const auto home{QStandardPaths::writableLocation(QStandardPaths::HomeLocation)};
        const auto directory{QFileDialog::getExistingDirectory(this, tr("Open Directory"), home, QFileDialog::ShowDirsOnly)};
        if (directory.isEmpty()) {
            return;
        }

        const auto finalPath{QDir::cleanPath(directory + QDir::separator() + u"drawy.autosave"_s)};
        m_path->setText(finalPath);
    });

    m_enabled->setText(tr("Restore previous session on start"));
    connect(m_enabled, &QCheckBox::checkStateChanged, this, &AutoSaveWidget::slotEnableChanged);

    m_layout->addWidget(m_enabled);
    m_layout->addWidget(delayLabel);
    m_layout->addWidget(m_delay);

    m_layout->addWidget(pathLabel);
    m_layout->addWidget(m_pathWidget);

    // for tests
    m_delay->setObjectName(u"m_delay"_s);
    m_path->setObjectName(u"m_path"_s);
    m_enabled->setObjectName(u"m_enabled"_s);
}

void AutoSaveWidget::save()
{
    DrawyGlobalConfig::self()->setDelay(m_delay->value());
    DrawyGlobalConfig::self()->setPath(m_path->text());
    DrawyGlobalConfig::self()->setAutoSaveEnabled(m_enabled->isChecked());
    DrawyGlobalConfig::self()->save();
}

void AutoSaveWidget::load()
{
    m_delay->setValue(DrawyGlobalConfig::self()->delay());
    m_path->setText(DrawyGlobalConfig::self()->path());
    m_enabled->setChecked(DrawyGlobalConfig::self()->autoSaveEnabled());
    slotEnableChanged(m_enabled->checkState());
}

void AutoSaveWidget::slotEnableChanged(Qt::CheckState state)
{
    const bool enable{state == Qt::Checked};
    m_delay->setEnabled(enable);
    m_pathWidget->setEnabled(enable);
}

#include "moc_autosavewidget.cpp"
