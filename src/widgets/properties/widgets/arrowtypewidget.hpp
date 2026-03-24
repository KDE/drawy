// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "propertywidget.hpp"
class QButtonGroup;
class ApplicationContext;

class ArrowTypeWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit ArrowTypeWidget(ApplicationContext *context, QWidget *parent = nullptr);

    void setValue(const QVariant &value) override;

protected:
    QButtonGroup *m_group;
};
