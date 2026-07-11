// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawygui_export.h"

#include <QInputMethodEvent>
#include <QRect>
#include <QTextCursor>
#include <QTextDocument>

#include "item.hpp"
class SpellCheckHighlighter;

class LIBDRAWYGUI_EXPORT TextItem : public Item
{
public:
    TextItem();
    ~TextItem() override;

    [[nodiscard]] QTextCursor &cursor();

    [[nodiscard]] bool intersects(const QRectF &rect) override;

    void draw(QPainter &painter, const QPointF &offset) override;

    void resize(QTransform operation) override;
    void commitTransformation() override;
    [[nodiscard]] bool lockAspectRatioWhenResizing() const override;

    void createTextBox(const QPointF position);

    void setWrapWidth(qreal wrapWidth);
    [[nodiscard]] qreal wrapWidth() const;
    [[nodiscard]] qreal minWrapWidth() const;

    enum class Mode : int8_t {
        Edit = 0,
        Normal
    };

    [[nodiscard]] Mode mode() const;
    void setMode(Mode mode);

    [[nodiscard]] qsizetype getIndexFromCursor(QPointF cursor) const;
    void setCaret(QPointF cursorPos);

    [[nodiscard]] QString text() const;
    [[nodiscard]] QString html() const;
    void setHtml(const QString &html);

    [[nodiscard]] SpellCheckHighlighter *highlighter() const;

    [[nodiscard]] QRectF cursorRect(int offset = 0) const;

    void updatePreedit(const QString &preedit, const QList<QInputMethodEvent::Attribute> &attributes);

    [[nodiscard]] Item::FormType formType() const override;

    void updateAfterProperty() override;

    [[nodiscard]] Property property(const Property::Type propertyType) const override;
    [[nodiscard]] bool needsPropertyUpdate(const Property &property) const override;
    void setProperty(const Property::Type propertyType, Property newObj) override;

    [[nodiscard]] QJsonObject serialize(int zorder) const override;
    void deserialize(const QJsonObject &obj) override;

    [[nodiscard]] bool needsCaching() const override;

protected:
    void drawItem(QPainter &painter, const QPointF &offset) const override;

private:
    [[nodiscard]] LIBDRAWYGUI_NO_EXPORT QFont getFont() const;
    [[nodiscard]] LIBDRAWYGUI_NO_EXPORT QTextOption getTextOptions() const;

    LIBDRAWYGUI_NO_EXPORT void scaleTextFragments(const qreal scaleY);
    LIBDRAWYGUI_NO_EXPORT void updateBoundingBox();

    QTextDocument m_document;
    SpellCheckHighlighter *m_highlighter;
    QTextCursor m_cursor;
    QTextCharFormat m_currentFormat;
    Mode m_mode{Mode::Normal};

    QString m_preeditString;
    int m_preeditCursorPos{0};

    qreal m_wrapWidth{-1};
};
LIBDRAWYGUI_EXPORT QDebug operator<<(QDebug d, const TextItem &t);
