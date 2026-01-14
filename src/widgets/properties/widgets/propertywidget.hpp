// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_private_export.h"
#include "properties/property.hpp"
#include <QWidget>

class LIBDRAWYWIDGETS_TESTS_EXPORT PropertyWidget : public QObject
{
    Q_OBJECT
public:
    explicit PropertyWidget(QWidget *parent = nullptr);

    virtual QString name() const = 0;
    virtual const Property value() const = 0;

    QWidget *widget() const;

Q_SIGNALS:
    void changed(Property property);

protected:
    QWidget *m_widget{};
};
