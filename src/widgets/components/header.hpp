// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QHBoxLayout>
#include <QWidget>

class Header : public QWidget
{
public:
    Header(QWidget *parent = nullptr);

    void initialize();
    bool isInitialized() const;

    void resizeEvent(QResizeEvent *event) override;

private:
    QHBoxLayout *m_layout{nullptr};
    QWidget *m_centerWidget{nullptr};
    QWidget *m_rightSpaceBalancingWidget{nullptr};

    bool m_isInitialized{false};
};
