#pragma once

#include <QGroupBox>
#include <QVBoxLayout>

class ConfigGroupInterface : public QGroupBox
{
public:
    virtual void save() = 0;
    virtual void load() = 0;

    ConfigGroupInterface(QWidget *parent, const QString &groupTitle);

protected:
    QVBoxLayout *const m_layout{};
};
