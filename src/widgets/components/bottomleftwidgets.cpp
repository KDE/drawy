// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "bottomleftwidgets.hpp"
#include "common/constants.hpp"
#include "context/uicontext.hpp"
#include "frame.hpp"
#include "keybindings/actionmanager.hpp"
#include "page/pagemanager.hpp"
#include "page/pagemodel.hpp"
#include "page/pageswidget.hpp"
#include <KLocalizedString>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
BottomLeftWidgets::BottomLeftWidgets(ActionManager *actionManager, PageManager *pageManager, QWidget *parent)
    : QWidget{parent}
    , m_layout(new QHBoxLayout{this})
    , m_pageManager(pageManager)
    , m_pageModel(new PageModel(pageManager, this))
    , m_pageButton(new QToolButton{this})
    , m_actionManager(actionManager)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_layout->setContentsMargins(0, 0, 0, 0);

    const int iconWidth{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    const QSize iconSize{iconWidth, iconWidth};

    // Hamburger menu
    auto hamburgerMenu{new QToolButton{this}};
    hamburgerMenu->setIcon(QIcon::fromTheme(u"application-menu"_s));
    hamburgerMenu->setIconSize(iconSize);

    // Zoom controls
    auto zoomControlFrame{new Frame{this}};
    auto zoomControlLayout{new QHBoxLayout{zoomControlFrame}};

    auto zoomOutButton{new QToolButton{zoomControlFrame}};
    zoomOutButton->setAutoRaise(true);
    zoomOutButton->setIconSize(iconSize);
    zoomOutButton->setDefaultAction(actionManager->action(KStandardActions::ZoomOut));

    auto zoomLabel{new QToolButton{zoomControlFrame}};
    const int maxZoomValue{qRound(Common::zoomInLimit * 100)};
    zoomLabel->setText(i18nc("@label, percent value", "%1%", maxZoomValue));
    zoomLabel->setFixedWidth(zoomLabel->sizeHint().width());
    zoomLabel->setText(i18nc("@label, percent text", "100%"));
    zoomLabel->setAutoRaise(true);
    zoomLabel->setToolTip(i18nc("@info:tooltip", "Reset Zoom"));
    zoomLabel->setMinimumHeight(zoomOutButton->sizeHint().height());

    connect(zoomLabel, &QToolButton::clicked, this, &BottomLeftWidgets::resetZoom);

    auto zoomInButton{new QToolButton{zoomControlFrame}};
    zoomInButton->setAutoRaise(true);
    zoomInButton->setIconSize(iconSize);
    zoomInButton->setDefaultAction(actionManager->action(KStandardActions::ZoomIn));

    connect(this, &BottomLeftWidgets::zoomFactorChanged, this, [zoomLabel](qreal newZoomFactor) {
        const int zoomValue{qRound(newZoomFactor * 100)};
        zoomLabel->setText(i18nc("@label, percent value", "%1%", zoomValue));
    });

    zoomControlLayout->setSpacing(0);
    zoomControlLayout->addWidget(zoomOutButton);
    zoomControlLayout->addWidget(zoomLabel);
    zoomControlLayout->addWidget(zoomInButton);

    // Page controls
    auto pageControlFrame{new Frame{this}};
    auto pageControlLayout{new QHBoxLayout{pageControlFrame}};

    m_pageControlFrame = pageControlFrame;

    auto previousPageButton{new QToolButton{pageControlFrame}};
    previousPageButton->setAutoRaise(true);
    previousPageButton->setIconSize(iconSize);
    previousPageButton->setDefaultAction(actionManager->action(ActionManager::Action::PreviousPage));

    m_pageButton->setParent(pageControlFrame);
    m_pageButton->setAutoRaise(true);
    m_pageButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_pageButton->setMinimumHeight(previousPageButton->sizeHint().height());
    m_pageButton->setFixedWidth(Common::pageSwitcherWidth);
    m_pageButton->setToolTip(i18nc("@info:tooltip", "Manage Pages"));
    connect(m_pageButton, &QToolButton::clicked, this, &BottomLeftWidgets::showPagesPopup);

    auto nextPageButton{new QToolButton{pageControlFrame}};
    nextPageButton->setAutoRaise(true);
    nextPageButton->setIconSize(iconSize);
    nextPageButton->setDefaultAction(actionManager->action(ActionManager::Action::NextPage));

    pageControlLayout->setSpacing(0);
    pageControlLayout->addWidget(previousPageButton);
    pageControlLayout->addWidget(m_pageButton);
    pageControlLayout->addWidget(nextPageButton);

    connect(m_pageModel, &QAbstractItemModel::modelReset, this, &BottomLeftWidgets::updatePageButton);
    connect(m_pageModel, &QAbstractItemModel::dataChanged, this, &BottomLeftWidgets::updatePageButton);
    connect(m_pageModel, &QAbstractItemModel::rowsMoved, this, &BottomLeftWidgets::updatePageButton);

    auto updatePopupGeometry = [this]() {
        if (m_pagesPopup && m_pagesPopup->isVisible() && m_pagesWidget) {
            const int margin{style()->pixelMetric(QStyle::PM_LayoutLeftMargin)};
            const int contentHeight{m_pagesWidget->sizeHint().height() + 2 * margin};
            const int targetHeight{qMin(contentHeight, Common::pagesPopupMaxHeight)};
            const QPoint buttonTopLeft{m_pageControlFrame->mapToGlobal(QPoint{0, 0})};
            m_pagesPopup->setGeometry(buttonTopLeft.x(), buttonTopLeft.y() - targetHeight - 10, Common::pagesPopupWidth, targetHeight);
        }
    };
    connect(m_pageModel, &QAbstractItemModel::modelReset, this, updatePopupGeometry);
    connect(m_pageModel, &QAbstractItemModel::rowsInserted, this, updatePopupGeometry);
    connect(m_pageModel, &QAbstractItemModel::rowsRemoved, this, updatePopupGeometry);

    updatePageButton();

    m_layout->addWidget(zoomControlFrame, 0, Qt::AlignLeft);
    m_layout->addWidget(pageControlFrame, 0, Qt::AlignLeft);
}

void BottomLeftWidgets::updatePageButton()
{
    const int current{m_pageManager->currentPage()};
    const int count{m_pageModel->rowCount()};
    if (current < 0 || current >= count) {
        m_pageButton->setText({});
        return;
    }
    const QString name{m_pageModel->index(current).data(Qt::DisplayRole).toString()};
    const QString fullText{i18nc("@action:button page name", "%1", name)};
    const int margin{m_pageButton->style()->pixelMetric(QStyle::PM_ButtonMargin, nullptr, m_pageButton)};
    const int iconWidth{m_pageButton->icon().isNull() ? 0 : m_pageButton->iconSize().width() + margin};
    const int availableWidth{qMax(0, Common::pageSwitcherWidth - iconWidth - 2 * margin)};
    m_pageButton->setText(m_pageButton->fontMetrics().elidedText(fullText, Qt::ElideRight, availableWidth));
    m_pageButton->setToolTip(fullText);
}

void BottomLeftWidgets::showPagesPopup()
{
    if (!m_pagesPopup) {
        auto popup{new Frame{this}};
        popup->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
        popup->setAttribute(Qt::WA_TranslucentBackground);
        m_pagesPopup = popup;
        auto popupLayout{new QVBoxLayout{m_pagesPopup}};
        const int margin{style()->pixelMetric(QStyle::PM_LayoutLeftMargin)};
        popupLayout->setContentsMargins(margin, margin, margin, margin);
        m_pagesWidget = new PagesWidget{m_actionManager, m_pageModel, m_pagesPopup};
        popupLayout->addWidget(m_pagesWidget);
        m_pagesPopup->setFocusProxy(m_pagesWidget);
        m_pagesPopup->setFixedWidth(Common::pagesPopupWidth);
        m_pagesPopup->setMaximumHeight(Common::pagesPopupMaxHeight);
    }

    const int margin{style()->pixelMetric(QStyle::PM_LayoutLeftMargin)};
    const int contentHeight{m_pagesWidget->sizeHint().height() + 2 * margin};
    const int targetHeight{qMin(contentHeight, Common::pagesPopupMaxHeight)};

    // Open above the frame as it is in the bottom of the window
    const QPoint buttonTopLeft{m_pageControlFrame->mapToGlobal(QPoint{0, 0})};
    m_pagesPopup->setGeometry(buttonTopLeft.x(), buttonTopLeft.y() - targetHeight - 10, Common::pagesPopupWidth, targetHeight);
    m_pagesPopup->show();
    m_pagesPopup->setFocus();
}

#include "moc_bottomleftwidgets.cpp"
