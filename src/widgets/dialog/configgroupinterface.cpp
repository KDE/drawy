#include "configgroupinterface.hpp"

ConfigGroupInterface::ConfigGroupInterface(QWidget *parent, const QString &groupTitle)
    : QGroupBox{parent}
    , m_layout{new QVBoxLayout{this}}
{
    setTitle(groupTitle);
}

#include "moc_configgroupinterface.cpp"
