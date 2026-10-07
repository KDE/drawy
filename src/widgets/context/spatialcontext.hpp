// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
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

private:
    [[nodiscard]] Page *currentPage() const;
    std::unique_ptr<CoordinateTransformer> m_coordinateTransformer{nullptr};

    ApplicationContext *const m_applicationContext;
};
