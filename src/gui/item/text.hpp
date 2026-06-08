// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawygui_export.h"

#include <QPainter>
#include <QRect>
#include <QTextCursor>
#include <QTextDocument>

#include "item.hpp"

class LIBDRAWYGUI_EXPORT TextItem : public Item
{
public:
    TextItem();
    ~TextItem() override;

    [[nodiscard]] QTextCursor &cursor();

    [[nodiscard]] bool intersects(const QRectF &rect) override;

    void draw(QPainter &painter, const QPointF &offset) override;

    void commitTransformation() override;
    [[nodiscard]] bool lockAspectRatioWhenResizing() const override;

    void createTextBox(const QPointF position);

    enum class Mode : int8_t {
        Edit = 0,
        Normal
    };

    [[nodiscard]] Mode mode() const;
    void setMode(Mode mode);

    [[nodiscard]] qsizetype getIndexFromCursor(QPointF cursor) const;
    void setCaret(QPointF cursorPos);

    [[nodiscard]] QString text() const;

    [[nodiscard]] Item::FormType formType() const override;

    void updateAfterProperty() override;

    [[nodiscard]] QJsonObject serialize(int zorder) const override;
    void deserialize(const QJsonObject &obj) override;

    [[nodiscard]] bool needsCaching() const override;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

private:
    [[nodiscard]] LIBDRAWYGUI_NO_EXPORT QFont getFont() const;
    [[nodiscard]] LIBDRAWYGUI_NO_EXPORT QPen getPen() const;

    [[nodiscard]] LIBDRAWYGUI_NO_EXPORT static QTextOption getTextOptions();
    constexpr LIBDRAWYGUI_NO_EXPORT static int getTextFlags();

    LIBDRAWYGUI_NO_EXPORT void updateBoundingBox();

    QTextDocument m_document;
    QTextCursor m_cursor;
    Mode m_mode{Mode::Normal};
};
LIBDRAWYGUI_EXPORT QDebug operator<<(QDebug d, const TextItem &t);
