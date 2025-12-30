// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QLayout>

class BoardLayout : public QLayout
{
    Q_OBJECT
public:
    explicit BoardLayout(QWidget *parent = nullptr);
    ~BoardLayout() override;

    void addItem(QLayoutItem *item) override;
    QLayoutItem *itemAt(int index) const override;
    QLayoutItem *takeAt(int index) override;
    void setGeometry(const QRect &rect) override;
    int count() const override;
    QSize sizeHint() const override;
    QSize minimumSize() const override;

    void setLeftWidget(QWidget *const item, bool pinned = false);
    void setTopWidget(QWidget *const item, bool pinned = false);
    void setRightWidget(QWidget *const item, bool pinned = false);
    void setBottomWidget(QWidget *const item, bool pinned = false);
    void setCentralWidget(QWidget *const item);
    void setMargins(int margins);

private:
    QLayoutItem *m_centralWidget{nullptr};
    QLayoutItem *m_leftWidget{nullptr};
    QLayoutItem *m_topWidget{nullptr};
    QLayoutItem *m_rightWidget{nullptr};
    QLayoutItem *m_bottomWidget{nullptr};

    std::array<QLayoutItem **, 5> m_widgets{};

    bool m_isLeftPinned{false};
    bool m_isTopPinned{false};
    bool m_isRightPinned{false};
    bool m_isBottomPinned{false};

    int m_margins{0};
};
