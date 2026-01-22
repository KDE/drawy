/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "configuresettingsdialog.hpp"
#include "configuregeneralwidget.hpp"
#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QPushButton>
#include <QWindow>

namespace
{
const char myConfigGroupName[] = "ConfigureSettingsDialog";
}
using namespace Qt::Literals::StringLiterals;
ConfigureSettingsDialog::ConfigureSettingsDialog(QWidget *parent)
    : KPageDialog(parent)
    , mConfigureGeneralWidget(new ConfigureGeneralWidget(this))
{
    setWindowTitle(tr("Configure Drawy"));
    setFaceType(KPageDialog::List);

    const QString generalPageName = tr("General");
    mConfigureGeneralWidgetPage = new KPageWidgetItem(mConfigureGeneralWidget, generalPageName);
    mConfigureGeneralWidgetPage->setIcon(QIcon(u":/drawy.svg"_s));
    addPage(mConfigureGeneralWidgetPage);

    buttonBox()->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults);

    connect(buttonBox()->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ConfigureSettingsDialog::slotAccepted);
    connect(buttonBox()->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &ConfigureSettingsDialog::reject);
    connect(buttonBox()->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &ConfigureSettingsDialog::slotRestoreDefaults);
    readConfig();
    load();
}

ConfigureSettingsDialog::~ConfigureSettingsDialog()
{
    writeConfig();
}

void ConfigureSettingsDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ConfigureSettingsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void ConfigureSettingsDialog::slotAccepted()
{
    mConfigureGeneralWidget->save();
}

void ConfigureSettingsDialog::load()
{
    mConfigureGeneralWidget->load();
}

void ConfigureSettingsDialog::slotRestoreDefaults()
{
    mConfigureGeneralWidget->restoreToDefaults();
}
#include "moc_configuresettingsdialog.cpp"
