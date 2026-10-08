/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "data-structures/quadtree.hpp"
#include "libdrawywidgets_private_export.h"
#include <memory>
class QuadTree;
class ApplicationContext;
class CommandHistory;
class LIBDRAWYWIDGETS_TESTS_EXPORT Page : public QObject
{
    Q_OBJECT
public:
    explicit Page(ApplicationContext *context);
    ~Page();

    [[nodiscard]] static QString defaultName(int index);

    [[nodiscard]] QString name() const;
    void setName(const QString &newName);

    [[nodiscard]] CommandHistory *commandHistory() const;

    [[nodiscard]] const QPointF &offsetPos() const;
    void setOffsetPos(const QPointF &pos);

    QuadTree &quadtree() const;

    [[nodiscard]] qreal zoomFactor() const;
    void setZoomFactor(qreal newZoomFactor);

private:
    // Stores the position of the topleft corner of the viewport with respect to
    // to the world center. If viewport moves down/right, the coordinates increase
    QPointF m_offsetPos{};
    qreal m_zoomFactor{1};
    QString mName;
    std::unique_ptr<QuadTree> m_quadtree{nullptr};
    ApplicationContext *const m_applicationContext;
    std::unique_ptr<CommandHistory> m_commandHistory{nullptr};
};
