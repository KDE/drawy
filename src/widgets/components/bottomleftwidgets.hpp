// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QHBoxLayout>
#include <QWidget>
class ActionManager;
class PageManager;
class PageModel;
class QToolButton;
class BottomLeftWidgets : public QWidget
{
    Q_OBJECT
public:
    explicit BottomLeftWidgets(ActionManager *actionManager, PageManager *pageManager, QWidget *parent = nullptr);

Q_SIGNALS:
    void resetZoom();
    void zoomFactorChanged(qreal newZoomFactor);

private:
    void updatePageButton();
    void showPagesPopup();
    QHBoxLayout *const m_layout;
    PageManager *const m_pageManager;
    PageModel *const m_pageModel;
    QToolButton *const m_pageButton;
    ActionManager *const m_actionManager;
    QWidget *m_pagesPopup = nullptr;
};
