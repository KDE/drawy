// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "configgroupinterface.hpp"

ConfigGroupInterface::ConfigGroupInterface(QWidget *parent, const QString &groupTitle)
    : QGroupBox{groupTitle, parent}
    , m_layout{new QVBoxLayout{this}}
{
}

#include "moc_configgroupinterface.cpp"
