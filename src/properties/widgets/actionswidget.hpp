// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ACTIONSWIDGET_H
#define ACTIONSWIDGET_H

#include "propertywidget.hpp"

class ActionsWidget : public PropertyWidget {
public:
    explicit ActionsWidget(QWidget *parent = nullptr);

    QString name() const override;
    const Property value() const override;
};

#endif  // TOOLACTIONS_H
