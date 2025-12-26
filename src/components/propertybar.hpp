// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QFrame>
#include <QStackedWidget>
#include <QVBoxLayout>
class Tool;
class PropertyManager;

class PropertyBar : public QFrame {
    Q_OBJECT
public:
    PropertyBar(QWidget *parent = nullptr);

    void setPropertyManager(PropertyManager *manager);
    void updateToolProperties();

public slots:
    void updateProperties(Tool &);

private:
    QVBoxLayout *m_layout{};
    PropertyManager *m_propertyManager{};
};
