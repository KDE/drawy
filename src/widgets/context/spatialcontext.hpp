// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QList>
#include <QObject>
#include <QPointF>
class QuadTree;
class CoordinateTransformer;
class ApplicationContext;
class CommandHistory;
class Page;

class SpatialContext : public QObject
{
    Q_OBJECT
public:
    explicit SpatialContext(ApplicationContext *context);
    ~SpatialContext() override;

    // SpatialContext
    QuadTree &quadtree() const;
    CoordinateTransformer &coordinateTransformer() const;
    [[nodiscard]] CommandHistory *commandHistory() const;

    [[nodiscard]] const QPointF &offsetPos() const;
    void setOffsetPos(const QPointF &pos);

    void reset();

    // Forwards the signals of the current page's command history
    void updateCommandHistoryConnections();

Q_SIGNALS:
    void undoRedoChanged();
    void redoTextChanged(const QString &redoText);
    void undoTextChanged(const QString &undoText);
    void commandHistoryChanged();

private:
    [[nodiscard]] Page *currentPage() const;
    std::unique_ptr<CoordinateTransformer> m_coordinateTransformer{nullptr};

    ApplicationContext *const m_applicationContext;
    QList<QMetaObject::Connection> m_commandHistoryConnections;
};
