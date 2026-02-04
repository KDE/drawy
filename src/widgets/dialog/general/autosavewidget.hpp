#pragma once

#include "dialog/configgroupinterface.hpp"
#include <QCheckBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QWidget>

class AutoSaveWidget : public ConfigGroupInterface
{
public:
    AutoSaveWidget(QWidget *parent);

    void save() override;
    void load() override;
    void slotEnableChanged(Qt::CheckState state);

private:
    QSpinBox *const m_delay{};
    QWidget *const m_pathWidget{};
    QLineEdit *const m_path{};
    QCheckBox *const m_enabled{};
};
