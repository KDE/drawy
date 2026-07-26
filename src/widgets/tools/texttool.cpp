// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "texttool.hpp"

#include <KLocalizedString>
#include <memory>

#include "canvas/canvas.hpp"
#include "command/commandhistory.hpp"
#include "command/insertitemcommand.hpp"
#include "components/propertybar.hpp"
#include "components/toolbar.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "event/event.hpp"
#include "item/factory/textfactory.hpp"
#include "keybindings/keybindmanager.hpp"
#include "properties/widgets/propertymanager.hpp"
#include <QClipboard>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QInputMethod>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QToolTip>

using namespace Qt::Literals::StringLiterals;

TextTool::TextTool(ApplicationContext *context)
    : DrawingTool(context)
{
    m_cursor = QCursor(Qt::CrossCursor);
    m_itemFactory = std::make_unique<TextFactory>();

    m_properties = {Property::Type::StrokeColor,
                    Property::Type::FontSize,
                    Property::Type::FontStyle,
                    Property::Type::FontFamily,
                    Property::Type::TextAlignment,
                    Property::Type::ListStyle};
}

void TextTool::mousePressed(ApplicationContext *context)
{
    QGuiApplication::inputMethod()->commit();

    const UIContext *uiContext{context->uiContext()};

    if (uiContext->appEvent()->button() == Qt::LeftButton) {
        auto spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext->coordinateTransformer()};
        auto renderingContext{context->renderingContext()};
        const QuadTree &quadTree{spatialContext->quadtree()};

        const QPointF worldPos{transformer.viewToWorld(uiContext->appEvent()->pos())};
        QList<std::shared_ptr<Item>> intersectingItems{quadTree.queryItems(worldPos, [](const std::shared_ptr<Item> &item, const QPointF &point) {
            return item->formType() == Item::FormType::Text && !item->locked() && item->boundingBox().contains(point);
        })};

        if (intersectingItems.empty()) {
            if (m_curItem == nullptr) {
                m_curItem = std::dynamic_pointer_cast<TextItem>(m_itemFactory->create());

                m_curItem->setProperty(Property::Type::StrokeColor, uiContext->propertyManager()->value(Property::Type::StrokeColor));
                m_curItem->setProperty(Property::Type::FontSize, uiContext->propertyManager()->value(Property::Type::FontSize));
                m_curItem->setProperty(Property::Type::FontStyle, uiContext->propertyManager()->value(Property::Type::FontStyle));
                m_curItem->setProperty(Property::Type::FontFamily, uiContext->propertyManager()->value(Property::Type::FontFamily));
                m_curItem->setProperty(Property::Type::TextAlignment, uiContext->propertyManager()->value(Property::Type::TextAlignment));
                m_curItem->setProperty(Property::Type::ListStyle, uiContext->propertyManager()->value(Property::Type::ListStyle));

                m_curItem->createTextBox(transformer.viewToWorld(uiContext->appEvent()->pos()));

                auto commandHistory{spatialContext->commandHistory()};
                commandHistory->push(std::make_shared<InsertItemCommand>(QList<std::shared_ptr<Item>>{m_curItem}));
            } else {
                cleanup();
                return;
            }
        } else {
            const auto clickedItem = std::dynamic_pointer_cast<TextItem>(intersectingItems.back());

            const QString link = clickedItem->getLink(worldPos);
            if (!link.isEmpty() && (uiContext->appEvent()->modifiers() & Qt::ControlModifier)) {
                QDesktopServices::openUrl(QUrl(link));
                return;
            }

            if (m_curItem != nullptr && m_curItem != clickedItem) {
                m_curItem->setMode(TextItem::Mode::Normal);
                renderingContext->cacheGrid().markDirty(transformer.worldToGrid(m_curItem->boundingBox()).toRect());
            }

            m_curItem = clickedItem;
            m_curItem->setCaret(worldPos);

            renderingContext->cacheGrid().markDirty(transformer.worldToGrid(m_curItem->boundingBox()).toRect());

            m_isSelecting = true;
            m_mouseMoved = false;
        }

        context->selectionContext()->reset();
        context->selectionContext()->addToSelection(m_curItem);

        m_curItem->setMode(TextItem::Mode::Edit);
        uiContext->keybindManager()->setEnabled(false);
        context->selectionContext()->setShouldRenderHandles(false);

        QGuiApplication::inputMethod()->show();

        renderingContext->markForRender();
        renderingContext->markForUpdate();
    }

    m_isDrawing = true;
    uiContext->propertyBar()->updateToolProperties();
}

void TextTool::mouseMoved(ApplicationContext *context)
{
    auto spatialContext{context->spatialContext()};
    CoordinateTransformer &transformer{spatialContext->coordinateTransformer()};
    auto renderingContext{context->renderingContext()};
    const UIContext *uiContext{context->uiContext()};
    const QuadTree &quadTree{spatialContext->quadtree()};
    m_mouseMoved = true;

    const QPointF worldPos{transformer.viewToWorld(uiContext->appEvent()->pos())};

    if (m_isSelecting) {
        renderingContext->canvas()->setCursor(Qt::IBeamCursor);
        QToolTip::hideText();
    } else {
        const QList<std::shared_ptr<Item>> intersectingItems{quadTree.queryItems(worldPos, [](const std::shared_ptr<Item> &item, const QPointF &point) {
            return item->formType() == Item::FormType::Text && !item->locked() && item->boundingBox().contains(point);
        })};

        if (!intersectingItems.empty()) {
            const auto item = std::dynamic_pointer_cast<TextItem>(intersectingItems.back());
            const QString link = item->getLink(worldPos);
            if (!link.isEmpty()) {
                renderingContext->canvas()->setCursor(Qt::PointingHandCursor);
                QToolTip::showText(QCursor::pos(), i18n("ctrl + click to open: %1", link), renderingContext->canvas());
            } else {
                renderingContext->canvas()->setCursor(Qt::IBeamCursor);
                QToolTip::hideText();
            }
        } else {
            renderingContext->canvas()->setCursor(Qt::CrossCursor);
            QToolTip::hideText();
        }
    }

    if (m_isSelecting && m_curItem) {
        const qsizetype curIndex{m_curItem->getIndexFromCursor(worldPos)};
        auto &cursor = m_curItem->cursor();

        if (m_doubleClicked || m_tripleClicked) {
            QTextCursor temp = cursor;
            temp.setPosition(curIndex);
            temp.select(m_doubleClicked ? QTextCursor::WordUnderCursor : QTextCursor::LineUnderCursor);

            const bool isLeft{curIndex < cursor.anchor()};
            if (isLeft) {
                cursor.setPosition(qMax(cursor.anchor(), cursor.position()), QTextCursor::MoveAnchor);
                cursor.setPosition(temp.selectionStart(), QTextCursor::KeepAnchor);
            } else {
                cursor.setPosition(qMin(cursor.anchor(), cursor.position()), QTextCursor::MoveAnchor);
                cursor.setPosition(temp.selectionEnd(), QTextCursor::KeepAnchor);
            }
        } else {
            cursor.setPosition(cursor.anchor(), QTextCursor::MoveAnchor);
            cursor.setPosition(curIndex, QTextCursor::KeepAnchor);
        }
        m_curItem->setDirty(true);
        renderingContext->cacheGrid().markDirty(transformer.worldToGrid(m_curItem->boundingBox()).toRect());
    }
    renderingContext->markForRender();
    renderingContext->markForUpdate();
}

void TextTool::mouseReleased([[maybe_unused]] ApplicationContext *context)
{
    m_isSelecting = false;
    m_doubleClicked = false;
    m_tripleClicked = false;
    context->uiContext()->propertyBar()->updateToolProperties();
}

void TextTool::mouseDoubleClick(ApplicationContext *context)
{
    if (!m_curItem) {
        return;
    }

    m_doubleClicked = true;
    if (!m_mouseMoved) {
        m_isSelecting = true;

        auto spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext->coordinateTransformer()};
        auto renderingContext{context->renderingContext()};

        m_curItem->cursor().select(QTextCursor::WordUnderCursor);
        m_curItem->setDirty(true);

        renderingContext->cacheGrid().markDirty(transformer.worldToGrid(m_curItem->boundingBox()).toRect());
        renderingContext->markForRender();
        renderingContext->markForUpdate();
        context->uiContext()->propertyBar()->updateToolProperties();
    }
}

void TextTool::mouseTripleClick(ApplicationContext *context)
{
    if (!m_curItem) {
        return;
    }

    m_tripleClicked = true;
    if (!m_mouseMoved) {
        m_isSelecting = true;

        auto spatialContext{context->spatialContext()};
        CoordinateTransformer &transformer{spatialContext->coordinateTransformer()};
        auto renderingContext{context->renderingContext()};

        m_curItem->cursor().select(QTextCursor::LineUnderCursor);
        m_curItem->setDirty(true);

        renderingContext->cacheGrid().markDirty(transformer.worldToGrid(m_curItem->boundingBox()).toRect());
        renderingContext->markForRender();
        renderingContext->markForUpdate();
        context->uiContext()->propertyBar()->updateToolProperties();
    }
}

void TextTool::keyPressed(ApplicationContext *context)
{
    if (!m_curItem) {
        return;
    }
    const auto ev{context->uiContext()->appEvent()};
    if (ev->key() == Qt::Key_Escape) {
        m_curItem->setMode(TextItem::Mode::Normal);
        context->uiContext()->keybindManager()->setEnabled(true);
        context->selectionContext()->setShouldRenderHandles(true);
        context->uiContext()->toolBar()->changeTool(Tool::Type::Selection);
        m_curItem = nullptr;

        QGuiApplication::inputMethod()->hide();

        context->renderingContext()->cacheGrid().markAllDirty();
        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();
        return;
    }
    if (m_curItem->mode() != TextItem::Mode::Edit) {
        return;
    }

    processKey(ev);
    m_curItem->setDirty(true);

    context->spatialContext()->quadtree().deleteItem(m_curItem);
    context->spatialContext()->quadtree().insertItem(m_curItem);

    context->renderingContext()->cacheGrid().markAllDirty();
    context->renderingContext()->markForRender();
    context->renderingContext()->markForUpdate();
    context->uiContext()->propertyBar()->updateToolProperties();
}

void TextTool::processKey(const Event *ev) const
{
    const bool ctrl = ev->modifiers() & Qt::ControlModifier;
    const bool shift = ev->modifiers() & Qt::ShiftModifier;
    auto &cursor = m_curItem->cursor();
    const auto moveMode = shift ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor;

    switch (ev->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (shift) {
            cursor.insertText(u"\u2028"_s);
        } else {
            cursor.insertText(u"\n"_s);
        }
        return;
    case Qt::Key_Left:
        cursor.movePosition(ctrl ? QTextCursor::WordLeft : QTextCursor::Left, moveMode);
        return;
    case Qt::Key_Right:
        cursor.movePosition(ctrl ? QTextCursor::WordRight : QTextCursor::Right, moveMode);
        return;
    case Qt::Key_Backspace:
        if (ctrl && !cursor.hasSelection()) {
            cursor.movePosition(QTextCursor::WordLeft, QTextCursor::KeepAnchor);
        }
        cursor.deletePreviousChar();
        return;
    case Qt::Key_Delete:
        if (ctrl && !cursor.hasSelection()) {
            cursor.movePosition(QTextCursor::WordRight, QTextCursor::KeepAnchor);
        }
        cursor.deleteChar();
        return;
    case Qt::Key_Up:
        cursor.movePosition(QTextCursor::Up, moveMode);
        return;
    case Qt::Key_Down:
        cursor.movePosition(QTextCursor::Down, moveMode);
        return;
    case Qt::Key_A:
        if (ctrl) {
            cursor.select(QTextCursor::Document);
            return;
        }
        break;
    case Qt::Key_Z:
        if (ctrl) {
            if (shift) {
                cursor.document()->redo();
            } else {
                cursor.document()->undo();
            }
            return;
        }
        break;
    case Qt::Key_C:
        if (ctrl && cursor.hasSelection()) {
            QGuiApplication::clipboard()->setText(cursor.selectedText());
            return;
        }
        break;
    case Qt::Key_V:
        if (ctrl) {
            const QString clipboardText{QGuiApplication::clipboard()->text()};
            if (!clipboardText.isEmpty()) {
                cursor.insertText(clipboardText);
            }
            return;
        }
        break;
    case Qt::Key_X:
        if (ctrl && cursor.hasSelection()) {
            QGuiApplication::clipboard()->setText(cursor.selectedText());
            cursor.removeSelectedText();
            return;
        }
        break;
    case Qt::Key_Home:
        cursor.movePosition(ctrl ? QTextCursor::Start : QTextCursor::StartOfLine, moveMode);
        return;
    case Qt::Key_End:
        cursor.movePosition(ctrl ? QTextCursor::End : QTextCursor::EndOfLine, moveMode);
        return;
    default:
        break;
    }
    if (!ev->text().isEmpty() && !ctrl) {
        cursor.insertText(ev->text());
    }
}

void TextTool::inputMethodInvoked(ApplicationContext *context)
{
    if (!m_curItem || m_curItem->mode() != TextItem::Mode::Edit) {
        return;
    }
    const auto ev{context->uiContext()->appEvent()};
    auto &cursor = m_curItem->cursor();

    cursor.beginEditBlock(); // the whole block is treated as a single operation on undo/redo

    if (ev->replacementLength() > 0) {
        const int start = cursor.position() + ev->replacementStart();
        cursor.setPosition(start);
        cursor.setPosition(start + ev->replacementLength(), QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
    }
    if (!ev->commitString().isEmpty()) {
        cursor.insertText(ev->commitString());
    }
    m_curItem->updatePreedit(ev->preeditString(), ev->attributes());

    cursor.endEditBlock();

    QGuiApplication::inputMethod()->update(Qt::ImCursorPosition | Qt::ImCursorRectangle);
    m_curItem->setDirty(true);
    context->spatialContext()->quadtree().deleteItem(m_curItem);
    context->spatialContext()->quadtree().insertItem(m_curItem);

    context->renderingContext()->cacheGrid().markAllDirty();
    context->renderingContext()->markForRender();
    context->renderingContext()->markForUpdate();
}

QVariant TextTool::inputMethodQueryInvoked([[maybe_unused]] ApplicationContext *context, Qt::InputMethodQuery query)
{
    if (!m_curItem) {
        return {};
    }
    const auto &cursor = m_curItem->cursor();
    const QTextBlock block = cursor.block();

    switch (query) {
    case Qt::ImEnabled:
        return (m_curItem->mode() == TextItem::Mode::Edit);
    case Qt::ImCursorRectangle: {
        QRectF rect = m_curItem->cursorRect();
        rect.translate(m_curItem->normalizedBoundingBox().topLeft());
        rect = m_curItem->transformObj().mapRect(rect);
        return m_context->spatialContext()->coordinateTransformer().worldToView(rect).toAlignedRect();
    }
    case Qt::ImFont:
        return cursor.charFormat().font();
    case Qt::ImCursorPosition:
        return cursor.position() - block.position();
    case Qt::ImAbsolutePosition:
        return cursor.position();
    case Qt::ImSurroundingText:
        return cursor.block().text();
    case Qt::ImTextBeforeCursor:
        return block.text().left(cursor.position() - block.position());
    case Qt::ImTextAfterCursor:
        return block.text().mid(cursor.position() - block.position());
    case Qt::ImCurrentSelection:
        return cursor.selectedText();
    case Qt::ImMaximumTextLength:
        return QVariant();
    case Qt::ImAnchorPosition:
        return qBound(0, cursor.anchor() - block.position(), block.length());
    default:
        return QVariant();
    }
}

void TextTool::keyReleased([[maybe_unused]] ApplicationContext *context)
{
}

void TextTool::cleanup()
{
    QGuiApplication::inputMethod()->commit();
    QGuiApplication::inputMethod()->hide();
    if (!m_curItem) {
        return;
    }

    auto spatialContext{m_context->spatialContext()};
    auto renderingContext{m_context->renderingContext()};
    auto uiContext{m_context->uiContext()};
    auto &transformer{spatialContext->coordinateTransformer()};
    auto &quadTree{spatialContext->quadtree()};

    m_curItem->setMode(TextItem::Mode::Normal);
    renderingContext->cacheGrid().markDirty(transformer.worldToGrid(m_curItem->boundingBox()).toRect());

    // enable keybindings again
    uiContext->keybindManager()->setEnabled(true);
    m_context->selectionContext()->setShouldRenderHandles(true);

    if (m_curItem->text().isEmpty()) {
        quadTree.deleteItem(m_curItem);
    }

    m_context->selectionContext()->reset();

    m_curItem = nullptr;
    renderingContext->markForRender();
    renderingContext->markForUpdate();
}

std::shared_ptr<TextItem> TextTool::curItem() const
{
    return m_curItem;
}

Tool::Type TextTool::type() const
{
    return Tool::Type::Text;
}

QString TextTool::icon() const
{
    return u"tool_text"_s;
}
