// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "propertywidget.hpp"
class QButtonGroup;
class QToolButton;

class ColorWidgetBase : public PropertyWidget
{
    Q_OBJECT
public:
    explicit ColorWidgetBase(QWidget *parent = nullptr);

    [[nodiscard]] QString name() const override;
    void setValue(const QVariant &val) override;

protected:
    [[nodiscard]] virtual QList<QColor> defaultColors() const = 0;
    void initialize();
    QButtonGroup *m_group = nullptr;
    QToolButton *m_currentColorButton = nullptr;

private:
    void assignCurrentColor(const QColor &col);
};
