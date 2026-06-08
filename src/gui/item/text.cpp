// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "text.hpp"

#include "common/utils/math.hpp"
#include <QAbstractTextDocumentLayout>
#include <QFontMetricsF>
#include <QJsonObject>
#include <QTextBlock>
#include <QTextLayout>
#include <QTextLine>
#include <utility>

#include "common/constants.hpp"
#include "serializer/textdeserializer.hpp"
#include "serializer/textserializer.hpp"

using namespace Qt::Literals::StringLiterals;
TextItem::TextItem()
    : m_cursor(&m_document)
{
    m_properties[Property::Type::StrokeColor] = Property{QColor(Qt::white), Property::Type::StrokeColor};
    m_properties[Property::Type::Opacity] = Property{255, Property::Type::Opacity};
    m_properties[Property::Type::FontSize] = Property{18, Property::Type::FontSize};

    QObject::connect(&m_document, &QTextDocument::contentsChanged, &m_document, [this]() {
        updateBoundingBox();
    });
}

TextItem::~TextItem()
{
}

void TextItem::createTextBox(const QPointF position)
{
    m_boundingBox.setTopLeft(position);
    m_boundingBox.setWidth(Common::defaultTextBoxWidth);

    m_document.setDefaultFont(getFont());
    const QFontMetricsF metrics{getFont()};
    m_boundingBox.setHeight(metrics.height());

    setDirty(true);
}

bool TextItem::intersects(const QRectF &rect)
{
    return m_transform.map(m_boundingBox).intersects(rect);
}

void TextItem::draw(QPainter &painter, const QPointF &offset)
{
    painter.save();
    painter.translate(m_boundingBox.topLeft() - offset);

    const QColor color{getPen().color()};
    QAbstractTextDocumentLayout::PaintContext ctx;
    ctx.palette.setColor(QPalette::Text, color);

    if (m_mode == Mode::Edit && m_cursor.hasSelection()) {
        // draw selection
        QAbstractTextDocumentLayout::Selection selection;
        selection.cursor = m_cursor;
        selection.format.setBackground(Common::selectionBackgroundColor);
        selection.format.setForeground(color);
        ctx.selections.append(selection);
    }

    if (m_mode == Mode::Edit) {
        // draw caret
        const auto cursor = m_cursor;
        const auto block = cursor.block();
        if (const auto *layout = block.layout()) {
            const QTextLine line = layout->lineForTextPosition(cursor.positionInBlock());
            if (line.isValid()) {
                const qreal x = line.cursorToX(cursor.positionInBlock());
                const qreal y = layout->position().y() + line.y();
                painter.setPen(getPen());
                painter.drawLine(QPointF(x, y), QPointF(x, y + line.height()));
            }
        }
    }
    m_document.documentLayout()->draw(&painter, ctx);
    painter.restore();
}

void TextItem::commitTransformation()
{
    const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(m_transform)};
    Q_ASSERT(scaleX == scaleY);

    const QTransform filtered{scaleX, 0, 0, scaleY, 0, 0};

    const int curFontSize{property(Property::Type::FontSize).value<int>()};
    const int newFontSize{qRound(curFontSize * scaleX)};

    setProperty(Property::Type::FontSize, Property{newFontSize, Property::Type::FontSize});

    m_boundingBox = filtered.map(m_boundingBox).boundingRect();
    updateBoundingBox();
}

void TextItem::drawItem([[maybe_unused]] QPainter &painter, [[maybe_unused]] const QPointF &offset) const
{
}

QTextCursor &TextItem::cursor()
{
    return m_cursor;
}

TextItem::Mode TextItem::mode() const
{
    return m_mode;
}

void TextItem::setMode(Mode mode)
{
    m_mode = mode;
    setDirty(true);
}

void TextItem::setCaret(QPointF cursorPos)
{
    if (!m_boundingBox.contains(m_transform.inverted().map(cursorPos))) {
        return;
    }

    m_cursor.setPosition(getIndexFromCursor(cursorPos));
}

qsizetype TextItem::getIndexFromCursor(QPointF cursorPos) const
{
    const QPointF localPos{m_transform.inverted().map(cursorPos)};
    const QPointF docPos = localPos - m_boundingBox.topLeft();

    return m_document.documentLayout()->hitTest(docPos, Qt::FuzzyHit);
}

void TextItem::updateBoundingBox()
{
    m_document.setDefaultFont(getFont());
    m_document.setDefaultTextOption(getTextOptions());
    m_document.setTextWidth(-1);

    m_boundingBox.setSize(m_document.size());
    setDirty(true);
}

QFont TextItem::getFont() const
{
    QFont font;
    font.setPointSize(property(Property::Type::FontSize).value<int>());
    font.setFamily(u"Fuzzy Bubbles"_s);

    return font;
}

QPen TextItem::getPen() const
{
    QPen pen;

    QColor color{property(Property::Type::StrokeColor).value<QColor>()};
    color.setAlpha(property(Property::Type::Opacity).value<int>());
    pen.setColor(color);

    return pen;
}

constexpr int TextItem::getTextFlags()
{
    return Qt::TextExpandTabs;
}

QTextOption TextItem::getTextOptions()
{
    QTextOption options{};
    options.setTabStopDistance(Common::tabStopDistance);

    return options;
}

QString TextItem::text() const
{
    return m_document.toPlainText();
}

Item::FormType TextItem::formType() const
{
    return Item::FormType::Text;
}

void TextItem::updateAfterProperty()
{
    updateBoundingBox();
}

QJsonObject TextItem::serialize(int zorder) const
{
    const TextSerializer serialize(this);
    return serialize.serialize(zorder);
}

void TextItem::deserialize(const QJsonObject &obj)
{
    TextDeserializer deserializer(this);
    deserializer.deserialize(obj);
}

bool TextItem::needsCaching() const
{
    return true;
}

QDebug operator<<(QDebug d, const TextItem &t)
{
    d.space() << "text:" << t.text();
    d.space() << "Item:" << static_cast<const Item &>(t);
    return d;
}

bool TextItem::lockAspectRatioWhenResizing() const
{
    return true;
}
