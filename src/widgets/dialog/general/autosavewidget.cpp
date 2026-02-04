#include "autosavewidget.hpp"
#include "dialog/general/configuregeneralwidget.hpp"
#include "properties/property.hpp"

#include "drawyglobalconfig.h"
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
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
        m_delay->setSuffix(tr(" minute(s)", "delay unit", m_delay->value()));
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
        auto home{QStandardPaths::writableLocation(QStandardPaths::HomeLocation)};
        auto directory{QFileDialog::getExistingDirectory(this, tr("Open Directory"), home, QFileDialog::ShowDirsOnly)};
        if (directory.isEmpty())
            return;

        auto finalPath{QDir::cleanPath(directory + QDir::separator() + u"drawy.autosave"_s)};
        m_path->setText(finalPath);
    });

    m_enabled->setText(tr("Restore previous sesion on start"));
    connect(m_enabled, &QCheckBox::checkStateChanged, this, &AutoSaveWidget::slotEnableChanged);

    m_layout->addWidget(m_enabled);
    m_layout->addWidget(delayLabel);
    m_layout->addWidget(m_delay);

    m_layout->addWidget(pathLabel);
    m_layout->addWidget(m_pathWidget);
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
    bool enable{state == Qt::Checked};
    m_delay->setEnabled(enable);
    m_pathWidget->setEnabled(enable);
}

#include "moc_autosavewidget.cpp"
