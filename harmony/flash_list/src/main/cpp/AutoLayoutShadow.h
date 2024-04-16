/**
 * MIT License
 *
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

//
// Created on 5/3/2024.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef HARMONY_AUTOLAYOUTSHADOW_H
#define HARMONY_AUTOLAYOUTSHADOW_H

#include "react/renderer/graphics/Float.h"
#include "CellContainerComponentInstance.h"

using namespace facebook;
using namespace react;

namespace rnoh {

    class AutoLayoutShadow {
    private:
        Float lastMaxBound;  // Tracks where the last pixel is drawn in the visible window
        Float lastMinBound;  // Tracks where first pixel is drawn in the visible window
        bool isWithinBounds(CellContainerComponentInstance &);

    public:
        bool horizontal{false};
        Float scrollOffset{0};
        Float offsetFromStart{0};
        Float windowSize{0};
        Float renderOffset{0};
        Float blankOffsetAtStart{0};  // Tracks blank area from the top
        Float blankOffsetAtEnd{0};    // Tracks blank area from the bottom
        Float lastMaxBoundOverall{0}; // Tracks where the last pixel is drawn in the overall

        AutoLayoutShadow(){};
        ~AutoLayoutShadow(){};
        void clearGapsAndOverlaps(std::vector<CellContainerComponentInstance::Shared> sortedItems);
        Float computeBlankFromGivenOffset(Float actualScrollOffset, Float distanceFromWindowStart, Float distanceFromWindowEnd);
    };
} // namespace rnoh

#endif // HARMONY_AUTOLAYOUTSHADOW_H
