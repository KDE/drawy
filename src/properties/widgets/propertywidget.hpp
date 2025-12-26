// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QWidget>
class Property;

class PropertyWidget : public QObject {
    Q_OBJECT
public:
    PropertyWidget(QWidget *parent = nullptr);

    virtual QString name() const = 0;
    virtual const Property value() const = 0;

    QWidget *widget() const;

signals:
    void changed(Property property);

protected:
    QWidget *m_widget{};
};
