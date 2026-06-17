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

    m_document.setDefaultFont(getFont());

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

    QTransform transform{m_transform};
    const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(transform)};

    if (m_mode == Mode::Normal && qFuzzyCompare(1.0, scaleY) && !qFuzzyCompare(1.0, scaleX)) {
        painter.scale(1.0 / scaleX, 1.0);
        const qreal width = m_wrapWidth > 0 ? m_wrapWidth : m_boundingBox.width();
        const qreal targetWidth = std::max(width * scaleX, minWrapWidth());
        m_document.setTextWidth(targetWidth);
    }

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
        QGuiApplication::inputMethod()->cursorRectangleChanged();
        // draw caret
        const QRectF rect = cursorRect(m_preeditCursorPos);
        if (rect.isValid()) {
            painter.setPen(getPen());
            painter.drawLine(rect.topLeft(), rect.bottomLeft());
        }
    }
    m_document.documentLayout()->draw(&painter, ctx);
    painter.restore();
}

void TextItem::resize(const QTransform operation)
{
    Item::resize(operation);

    const QSignalBlocker blocker{m_document};
    QTransform transform{m_transform};
    const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(transform)};

    if (m_mode == Mode::Normal && qFuzzyCompare(1.0, scaleY) && !qFuzzyCompare(1.0, scaleX)) {
        const qreal width = m_wrapWidth > 0 ? m_wrapWidth : m_boundingBox.width();
        const qreal targetWidth = std::max(width * scaleX, minWrapWidth());

        m_document.setTextWidth(targetWidth);
        m_boundingBox.setHeight(m_document.size().height());
    }
}

void TextItem::commitTransformation()
{
    const auto [scaleX, scaleY]{Common::Utils::Math::extractScale(m_transform)};
    const QTransform filtered{scaleX, 0, 0, scaleY, 0, 0};

    if (!qFuzzyCompare(1.0, scaleX)) {
        const qreal width = m_wrapWidth > 0 ? m_wrapWidth : m_boundingBox.width();
        m_wrapWidth = std::max(width * scaleX, minWrapWidth());
    }

    m_boundingBox = filtered.map(m_boundingBox).boundingRect();

    if (!qFuzzyCompare(1.0, scaleY)) {
        const qreal curFontSize{property(Property::Type::FontSize).value<qreal>()};
        const qreal newFontSize{std::max(1.0, curFontSize * scaleY)};
        setProperty(Property::Type::FontSize, Property{newFontSize, Property::Type::FontSize});
    }
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

    if (m_wrapWidth > 0) {
        m_document.setTextWidth(m_wrapWidth);
    }

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
    options.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    return options;
}

QString TextItem::text() const
{
    return m_document.toPlainText();
}

QString TextItem::html() const
{
    return m_document.toHtml();
}

Item::FormType TextItem::formType() const
{
    return Item::FormType::Text;
}

QRectF TextItem::cursorRect(int offset) const
{
    const auto block = m_cursor.block();
    if (const auto *layout = block.layout()) {
        const int pos = m_cursor.positionInBlock() + offset;
        const QTextLine line = layout->lineForTextPosition(pos);
        if (line.isValid()) {
            const qreal x = layout->position().x() + line.cursorToX(pos);
            const qreal y = layout->position().y() + line.y();
            const qreal height = line.height();

            return QRectF(x, y, 1.0, height);
        }
    }
    return QRectF();
}

void TextItem::updatePreedit(const QString &preedit, const QList<QInputMethodEvent::Attribute> &attributes)
{
    m_preeditString = preedit;
    m_preeditCursorPos = preedit.length();

    QList<QTextLayout::FormatRange> formats;
    for (const auto &attr : attributes) {
        if (attr.type == QInputMethodEvent::Cursor) {
            m_preeditCursorPos = attr.start;
        } else if (attr.type == QInputMethodEvent::TextFormat) {
            QTextLayout::FormatRange range;
            range.start = attr.start + m_cursor.positionInBlock();
            range.length = attr.length;
            range.format = qvariant_cast<QTextCharFormat>(attr.value);
            formats.append(range);
        }
    }
    auto layout = m_cursor.block().layout();
    if (!m_preeditString.isEmpty()) {
        layout->setPreeditArea(m_cursor.positionInBlock(), m_preeditString);
        layout->setFormats(formats);
    } else {
        layout->setPreeditArea(-1, QString());
        layout->clearFormats();
    }
    updateBoundingBox();
}

void TextItem::updateAfterProperty()
{
    QTextCursor cursor = m_cursor;
    cursor.select(QTextCursor::Document);
    QTextCharFormat format;
    format.setFontPointSize(property(Property::Type::FontSize).value<qreal>());
    cursor.mergeCharFormat(format);

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

qreal TextItem::minWrapWidth() const
{
    const QFontMetricsF metrics{getFont()};
    return metrics.maxWidth();
}
