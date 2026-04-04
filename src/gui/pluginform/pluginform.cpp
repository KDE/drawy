/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "pluginform.hpp"

PluginForm::PluginForm(QObject *parent)
    : QObject{parent}
{
}

PluginForm::~PluginForm() = default;

const QList<PluginForm::PluginFormInfo> &PluginForm::pluginFormInfos() const
{
    return mPluginFormInfoList;
}

#include "moc_pluginform.cpp"
