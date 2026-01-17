// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>

class Action : public QObject
{
    Q_OBJECT
private:
    QString m_name;
    QString m_description;
    std::function<void()> m_callable;

public:
    Action(QString name, QString description, std::function<void()> callable, QObject *parent);
    [[nodiscard]] QString name() const;
    void run();
};

namespace std
{
template<>
struct hash<Action> {
    size_t operator()(Action &action) const noexcept
    {
        return std::hash<QString>{}(action.name());
    }
};
}; // namespace std
