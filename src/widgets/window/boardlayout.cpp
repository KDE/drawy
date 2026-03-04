// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "boardlayout.hpp"

#include <QWidget>

BoardLayout::BoardLayout(QWidget *parent)
    : QLayout(parent)
{
    m_widgets[0] = &m_centralWidget;
    m_widgets[1] = &m_leftWidget;
    m_widgets[2] = &m_topWidget;
    m_widgets[3] = &m_rightWidget;
    m_widgets[4] = &m_bottomWidget;
}

BoardLayout::~BoardLayout()
{
    delete m_centralWidget;
    delete m_leftWidget;
    delete m_topWidget;
    delete m_rightWidget;
    delete m_bottomWidget;
    delete m_bottomLeftWidget;
    delete m_topLeftWidget;
}

void BoardLayout::addItem(QLayoutItem *item)
{
    if (m_centralWidget == nullptr) {
        m_centralWidget = item;
    } else {
        delete item;
    }
}

QLayoutItem *BoardLayout::itemAt(int index) const
{
    try {
        return *m_widgets.at(index);
    } catch (const std::out_of_range &) {
        return nullptr;
    }
}

QLayoutItem *BoardLayout::takeAt(int index)
{
    try {
        QLayoutItem *item{*m_widgets.at(index)};
        *(m_widgets.at(index)) = nullptr;
        return item;
    } catch (const std::out_of_range &) {
        return nullptr;
    }
}

void BoardLayout::setLeftWidget(QWidget *const item, bool pinned)
{
    delete m_leftWidget;
    m_leftWidget = new QWidgetItem(item);
    m_isLeftPinned = pinned;
    if (item) {
        item->raise();
    }
}

void BoardLayout::setTopWidget(QWidget *const item, bool pinned)
{
    delete m_topWidget;
    m_topWidget = new QWidgetItem(item);
    m_isTopPinned = pinned;
    if (item) {
        item->raise();
    }
}

void BoardLayout::setRightWidget(QWidget *const item, bool pinned)
{
    delete m_rightWidget;
    m_rightWidget = new QWidgetItem(item);
    m_isRightPinned = pinned;
    if (item) {
        item->raise();
    }
}

void BoardLayout::setBottomWidget(QWidget *const item, bool pinned)
{
    delete m_bottomWidget;
    m_bottomWidget = new QWidgetItem(item);
    m_isBottomPinned = pinned;
    if (item) {
        item->raise();
    }
}

void BoardLayout::setTopLeftWidget(QWidget *const item, bool pinned)
{
    delete m_topLeftWidget;
    m_topLeftWidget = new QWidgetItem(item);
    m_isTopLeftPinned = pinned;
    if (item) {
        item->raise();
    }
}

void BoardLayout::setTopRightWidget(QWidget *const item, bool pinned)
{
    delete m_topRightWidget;
    m_topRightWidget = new QWidgetItem(item);
    m_isTopRightPinned = pinned;
    if (item) {
        item->raise();
    }
}

void BoardLayout::setBottomLeftWidget(QWidget *const item, bool pinned)
{
    delete m_bottomLeftWidget;
    m_bottomLeftWidget = new QWidgetItem(item);
    m_isBottomLeftPinned = pinned;
    if (item) {
        item->raise();
    }
}

void BoardLayout::setBottomRightWidget(QWidget *const item, bool pinned)
{
    delete m_bottomRightWidget;
    m_bottomRightWidget = new QWidgetItem(item);
    m_isBottomRightPinned = pinned;
    if (item) {
        item->raise();
    }
}

void BoardLayout::setCentralWidget(QWidget *const item)
{
    delete m_centralWidget;
    m_centralWidget = new QWidgetItem(item);
}

void BoardLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    if (m_centralWidget != nullptr) {
        m_centralWidget->setGeometry(rect);
        if (QWidget *w = m_centralWidget->widget()) {
            w->lower();
        }
    }

    if (m_topLeftWidget != nullptr) {
        const QSize size{effectiveSize(m_topLeftWidget)};
        m_topLeftWidget->setGeometry(QRect(m_margins, m_margins, size.width(), size.height()));
    }
    if (m_topRightWidget != nullptr) {
        const QSize size{effectiveSize(m_topRightWidget)};
        m_topRightWidget->setGeometry(QRect(rect.width() - size.width() - m_margins, m_margins, size.width(), size.height()));
    }
    if (m_bottomLeftWidget != nullptr) {
        const QSize size{effectiveSize(m_bottomLeftWidget)};
        m_bottomLeftWidget->setGeometry(QRect(m_margins, rect.height() - size.height() - m_margins, size.width(), size.height()));
    }
    if (m_bottomRightWidget != nullptr) {
        const QSize size{effectiveSize(m_bottomRightWidget)};
        m_bottomRightWidget->setGeometry(
            QRect(rect.width() - size.width() - m_margins, rect.height() - size.height() - m_margins, size.width(), size.height()));
    }
    if (m_leftWidget != nullptr) {
        const QSize size{effectiveSize(m_leftWidget)};
        m_leftWidget->setGeometry(QRect(m_margins, (rect.height() - size.height()) / 2, size.width(), size.height()));
    }
    if (m_rightWidget != nullptr) {
        const QSize size{effectiveSize(m_rightWidget)};
        m_rightWidget->setGeometry(QRect(rect.width() - size.width() - m_margins, (rect.height() - size.height()) / 2, size.width(), size.height()));
    }
    if (m_topWidget != nullptr) {
        const QSize size{effectiveSize(m_topWidget)};

        int leftPosition{(rect.width() - size.width()) / 2};
        if (m_topLeftWidget != nullptr) {
            leftPosition = qMax(leftPosition, m_topLeftWidget->geometry().right() + m_margins);
        }

        m_topWidget->setGeometry(QRect(leftPosition, m_margins, size.width(), size.height()));
    }
    if (m_bottomWidget != nullptr) {
        // TODO: prevent overlaps
        const QSize size{effectiveSize(m_bottomWidget)};
        m_bottomWidget->setGeometry(QRect((rect.width() - size.width()) / 2, rect.height() - size.height() - m_margins, size.width(), size.height()));
    }
}

int BoardLayout::count() const
{
    // there are only 5 widgets so its o(1)
    int count{0};
    for (const auto item : m_widgets) {
        if (*item != nullptr) {
            count++;
        }
    }
    return count;
}

QSize BoardLayout::sizeHint() const
{
    return (m_centralWidget == nullptr ? QSize(0, 0) : m_centralWidget->sizeHint());
}

QSize BoardLayout::minimumSize() const
{
    return (m_centralWidget == nullptr ? QSize(10, 10) : m_centralWidget->minimumSize());
}

void BoardLayout::setMargins(int margins)
{
    m_margins = margins;
}

QSize BoardLayout::effectiveSize(QLayoutItem *item)
{
    QSize s = item->sizeHint();
    const QSize min = item->minimumSize();
    const QSize max = item->maximumSize();

    s.setWidth(qBound(min.width(), s.width(), max.width()));
    s.setHeight(qBound(min.height(), s.height(), max.height()));
    return s;
}

#include "moc_boardlayout.cpp"
