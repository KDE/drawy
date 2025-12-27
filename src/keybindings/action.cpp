// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "action.hpp"

#include <utility>

Action::Action(QString name, QString description, std::function<void()> callable, QObject *parent)
    : m_name{std::move(name)}
    , m_description{std::move(description)}
    , m_callable{std::move(callable)}
    , QObject{parent}
{
}

QString Action::name() const
{
    return m_name;
}

void Action::run()
{
    m_callable();
}

#include "moc_action.cpp"
