/******************************************************************************
* Copyright (c) 2018(-2021) STMicroelectronics.
* All rights reserved.
*
* This file is part of the TouchGFX 4.18.1 distribution.
*
* This software is licensed under terms that can be found in the LICENSE file in
* the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
*******************************************************************************/

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Drawable.hpp>
#include <touchgfx/widgets/canvas/CWRUtil.hpp>
#include <touchgfx/widgets/canvas/Canvas.hpp>
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>

namespace touchgfx
{
Line::Line()
    : CanvasWidget(),
      startX(0), startY(0), endX(0), endY(0),
      lineWidth(CWRUtil::toQ5<int>(1)),
      lineEnding(BUTT_CAP_ENDING),
      minimalRect(),
      lineCapArcIncrement(18)
{
    Drawable::setWidthHeight(0, 0);
}

void Line::setStart(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5)
{
    if (startX == xQ5 && startY == yQ5)
    {
        return;
    }

    startX = xQ5;
    startY = yQ5;

    updateCachedShape();
}

void Line::updateStart(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5)
{
    if (startX == xQ5 && startY == yQ5)
    {
        return;
    }

    Rect rectBefore = getMinimalRect();
    invalidateRect(rectBefore);

    startX = xQ5;
    startY = yQ5;

    updateCachedShape();

    Rect rectAfter = getMinimalRect();
    invalidateRect(rectAfter);
}

void Line::setEnd(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5)
{
    if (endX == xQ5 && endY == yQ5)
    {
        return;
    }

    endX = xQ5;
    endY = yQ5;

    updateCachedShape();
}

void Line::updateEnd(CWRUtil::Q5 xQ5, CWRUtil::Q5 yQ5)
{
    if (endX == xQ5 && endY == yQ5)
    {
        return;
    }

    Rect rectBefore = getMinimalRect();
    invalidateRect(rectBefore);

    endX = xQ5;
    endY = yQ5;

    updateCachedShape();

    Rect rectAfter = getMinimalRect();
    invalidateRect(rectAfter);
}

void Line::setLineEndingStyle(Line::LINE_ENDING_STYLE lineEndingStyle)
{
    lineEnding = lineEndingStyle;
    updateCachedShape();
}

Line::LINE_ENDING_STYLE Line::getLineEndingStyle() const
{
    return lineEnding;
}

void Line::setCapPrecision(int precision)
{
    if (precision < 1)
    {
        precision = 1;
    }
    if (precision > 180)
    {
        precision = 180;
    }
    lineCapArcIncrement = precision;
}

bool Line::drawCanvasWidget(const Rect& invalidatedArea) const
{
    Canvas canvas(this, invalidatedArea);

    CWRUtil::Q5 radius;
    int angleInDegrees = CWRUtil::angle(xCorner[0] - startX, yCorner[0] - startY, radius);

    canvas.moveTo(xCorner[0], yCorner[0]);
    canvas.lineTo(xCorner[1], yCorner[1]);
    if (lineEnding == ROUND_CAP_ENDING)
    {
        // Fixed 10 steps (steps 0 and 9 are at Corner[1] and [2])
        for (int i = lineCapArcIncrement; i < 180; i += lineCapArcIncrement)
        {
            canvas.lineTo(endX + radius * CWRUtil::sine(angleInDegrees - i), endY - radius * CWRUtil::cosine(angleInDegrees - i));
        }
    }
    canvas.lineTo(xCorner[2], yCorner[2]);
    canvas.lineTo(xCorner[3], yCorner[3]);
    if (lineEnding == ROUND_CAP_ENDING)
    {
        // Fixed 10 steps (steps 0 and 9 are at Corner[3] and [0])
        for (int i = 180 - lineCapArcIncrement; i > 0; i -= lineCapArcIncrement)
        {
            canvas.lineTo(startX + radius * CWRUtil::sine(angleInDegrees + i), startY - radius * CWRUtil::cosine(angleInDegrees + i));
        }
    }

    return canvas.render();
}

void Line::updateCachedShape()
{
    CWRUtil::Q5 dx = endX - startX;
    CWRUtil::Q5 dy = endY - startY;
    CWRUtil::Q5 d = CWRUtil::toQ5<int>(0);
    // Look for horizontal, vertical or no-line
    if ((int32_t)dx == 0)
    {
        if ((int32_t)dy == 0)
        {
            xCorner[0] = xCorner[1] = xCorner[2] = xCorner[3] = startX;
            yCorner[0] = yCorner[1] = yCorner[2] = yCorner[3] = startY;
            return;
        }
        d = abs(dy);
    }
    else if ((int32_t)dy == 0)
    {
        d = abs(dx);
    }
    else
    {
        // We want to hit as close to the limit inside 32bits.
        // sqrt(0x7FFFFFFF / 2) = 46340, which is roughly toQ5(1448)
        static const int32_t MAXVAL = 46340;
        int32_t common_divisor = gcd(abs((int32_t)dx), abs((int32_t)dy));
        // First try to scale down
        if (common_divisor != 1)
        {
            dx = CWRUtil::Q5((int32_t)dx / common_divisor);
            dy = CWRUtil::Q5((int32_t)dy / common_divisor);
        }
        // Neither dx or dy is zero, find the largest multiplier / smallest divisor to stay within limit
        if (abs((int32_t)dx) <= MAXVAL || abs((int32_t)dy) <= MAXVAL)
        {
            // Look for largest multiplier
            int32_t mulx = MAXVAL / abs((int32_t)dx);
            int32_t muly = MAXVAL / abs((int32_t)dy);
            int32_t mult = MIN(mulx, muly);
            dx = CWRUtil::Q5(mult * (int32_t)dx);
            dy = CWRUtil::Q5(mult * (int32_t)dy);
        }
        else
        {
            // Look for smallest divisor
            int32_t divx = abs((int32_t)dx) / MAXVAL;
            int32_t divy = abs((int32_t)dy) / MAXVAL;
            int32_t divi = MAX(divx, divy) + 1;
            dx = CWRUtil::Q5((int32_t)dx / divi);
            dy = CWRUtil::Q5((int32_t)dy / divi);
        }
        d = CWRUtil::length(dx, dy);
    }

    dy = CWRUtil::muldivQ5(lineWidth, dy, d) / 2;
    dx = CWRUtil::muldivQ5(lineWidth, dx, d) / 2;

    switch (lineEnding)
    {
    case BUTT_CAP_ENDING:
        xCorner[0] = startX - dy;
        yCorner[0] = startY + dx;
        xCorner[1] = endX - dy;
        yCorner[1] = endY + dx;
        xCorner[2] = endX + dy;
        yCorner[2] = endY - dx;
        xCorner[3] = startX + dy;
        yCorner[3] = startY - dx;
        break;
    case ROUND_CAP_ENDING:
    // Nothing cached, calculated on each draw, but extremes are same as SQUARE_CAP_ENDING, so
    // Fall Through (for calculations)
    default:
    case SQUARE_CAP_ENDING:
        xCorner[0] = startX - dy - dx;
        yCorner[0] = startY + dx - dy;
        xCorner[1] = endX - dy + dx;
        yCorner[1] = endY + dx + dy;
        xCorner[2] = endX + dy + dx;
        yCorner[2] = endY - dx + dy;
        xCorner[3] = startX + dy - dx;
        yCorner[3] = startY - dx - dy;
        break;
    }

    CWRUtil::Q5 xMin = xCorner[0];
    CWRUtil::Q5 xMax = xCorner[0];
    CWRUtil::Q5 yMin = yCorner[0];
    CWRUtil::Q5 yMax = yCorner[0];
    for (int i = 1; i < 4; i++)
    {
        if (xCorner[i] < xMin)
        {
            xMin = xCorner[i];
        }
        if (xCorner[i] > xMax)
        {
            xMax = xCorner[i];
        }
        if (yCorner[i] < yMin)
        {
            yMin = yCorner[i];
        }
        if (yCorner[i] > yMax)
        {
            yMax = yCorner[i];
        }
    }
    int16_t minX = xMin.to<int>();
    int16_t minY = yMin.to<int>();
    int16_t maxX = xMax.to<int>();
    int16_t maxY = yMax.to<int>();
    minimalRect = Rect(minX, minY, maxX - minX + 1, maxY - minY + 1);

    if (lineEnding == ROUND_CAP_ENDING)
    {
        xCorner[0] = startX - dy;
        yCorner[0] = startY + dx;
        xCorner[1] = endX - dy;
        yCorner[1] = endY + dx;
        xCorner[2] = endX + dy;
        yCorner[2] = endY - dx;
        xCorner[3] = startX + dy;
        yCorner[3] = startY - dx;
    }
}

Rect Line::getMinimalRect() const
{
    return minimalRect;
}

void Line::updateLengthAndAngle(CWRUtil::Q5 length, CWRUtil::Q5 angle)
{
    updateEnd(startX + length * CWRUtil::sine(angle), startY - length * CWRUtil::cosine(angle));
}
} // namespace touchgfx
