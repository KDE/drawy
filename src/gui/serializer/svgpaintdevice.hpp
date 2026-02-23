/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QPaintDevice>
#include <QPaintEngine>
#include <QXmlStreamWriter>

// There is a class in Qt for doing exactly this (QSvgGenerator).
// The reason we hand-roll our own is because QSvgGenerator:
// 1. Doesn't render text correctly
// 2. Renders text as SVG text elements, and not paths (we want them as paths as the font we use isn't standard),
//    and wrapping it to not do that seems impossible.
class SvgPaintEngine : public QPaintEngine
{
public:
    SvgPaintEngine(QXmlStreamWriter &writer);
    ~SvgPaintEngine() override;

    bool begin(QPaintDevice *device) override;
    bool end() override;

    void drawEllipse(const QRectF &rect) override;
    void drawImage(const QRectF &dest, const QImage &image, const QRectF &src, Qt::ImageConversionFlags flags) override;
    void drawPixmap(const QRectF &rect, const QPixmap &pixmap, const QRectF &src) override;
    void drawPath(const QPainterPath &path) override;
    void drawPolygon(const QPointF *points, int pointCount, QPaintEngine::PolygonDrawMode mode) override;
    void drawRects(const QRectF *rects, int rectCount) override;
    void drawLines(const QLineF *lines, int lineCount) override;
    void drawTextItem(const QPointF &p, const QTextItem &textItem) override;

    void updateState(const QPaintEngineState &) override
    {
    }
    QPaintEngine::Type type() const override
    {
        return QPaintEngine::Type::User;
    }

private:
    void writeStrokeFill();

    QXmlStreamWriter &m_writer;
};

class SvgPaintDevice : public QPaintDevice
{
public:
    SvgPaintDevice(QXmlStreamWriter &writer, int width, int height);
    ~SvgPaintDevice() override;

    QPaintEngine *paintEngine() const override
    {
        return &m_engine;
    }

protected:
    int metric(PaintDeviceMetric metric) const override;

private:
    mutable SvgPaintEngine m_engine;
    int m_width;
    int m_height;
};
