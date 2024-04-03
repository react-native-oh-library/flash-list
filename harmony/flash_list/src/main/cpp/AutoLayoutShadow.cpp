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

#include <react/renderer/components/view/ViewProps.h>
#include <glog/logging.h>
#include <sys/param.h>
#include <thread>
#include "AutoLayoutShadow.h"

namespace rnoh {

    /**checks for overlaps or gaps.between adjacent items and then applies a correction (Only Grid layouts with varying
     * spans) Performance: RecyclerListView renders very small number of views and this is not going to trigger multiple
     * layouts on Android side. Not expecting any major perf issu.*/
    void AutoLayoutShadow::clearGapsAndOverlaps(std::vector<CellContainerComponentInstance::Shared> sortedItems) {
        if (sortedItems.empty()) {
            return;
        }
        Float maxBound = 0;
        Float minBound = INT_MAX;
        Float maxBoundNeighbour = 0;
        lastMaxBoundOverall = 0;
        for (int i = 0; i < sortedItems.size() - 1; i++) {
            auto cell = sortedItems[i];
            auto neighbour = sortedItems[i + 1];
            // Only apply correction if the next cell is consecutive.
            bool isNeighbourConsecutive = neighbour->getIndex() == cell->getIndex() + 1;
            if (isWithinBounds(*cell)) {
                if (!horizontal) {
                    maxBound = MAX(maxBound, cell->getBottom());
                    minBound = MIN(minBound, cell->getTop());
                    maxBoundNeighbour = maxBound;
                    if (isNeighbourConsecutive) {
                        if (cell->getLeft() < neighbour->getLeft()) {
                            if (cell->getRight() != neighbour->getLeft()) {
                                neighbour->setRight(cell->getRight() + neighbour->getWidth());
                                neighbour->setLeft(cell->getRight());
                            }
                            if (cell->getTop() != neighbour->getTop()) {
                                neighbour->setBottom(cell->getTop() + neighbour->getHeight());
                                neighbour->setTop(cell->getTop());
                            }
                        } else {
                            neighbour->setBottom(maxBound + neighbour->getHeight());
                            neighbour->setTop(maxBound);
                        }
                    }
                    if (isWithinBounds(*neighbour)) {
                        maxBoundNeighbour = MAX(maxBound, neighbour->getBottom());
                    }
                } else {
                    maxBound = MAX(maxBound, cell->getRight());
                    minBound = MIN(minBound, cell->getLeft());
                    maxBoundNeighbour = maxBound;
                    if (isNeighbourConsecutive) {
                        if (cell->getTop() < neighbour->getTop()) {
                            if (cell->getBottom() != neighbour->getTop()) {
                                neighbour->setBottom(cell->getBottom() + neighbour->getHeight());
                                neighbour->setTop(cell->getBottom());
                            }
                            if (cell->getLeft() != neighbour->getLeft()) {
                                neighbour->setRight(cell->getLeft() + neighbour->getWidth());
                                neighbour->setLeft(cell->getLeft());
                            }
                        } else {
                            neighbour->setRight(maxBound + neighbour->getWidth());
                            neighbour->setLeft(maxBound);
                        }
                    }
                    if (isWithinBounds(*neighbour)) {
                        maxBoundNeighbour = MAX(maxBound, neighbour->getRight());
                    }
                }
            }
            if (horizontal) {
                lastMaxBoundOverall = MAX(lastMaxBoundOverall, cell->getRight());
                lastMaxBoundOverall = MAX(lastMaxBoundOverall, neighbour->getRight());
            } else {
                lastMaxBoundOverall = MAX(lastMaxBoundOverall, cell->getBottom());
                lastMaxBoundOverall = MAX(lastMaxBoundOverall, neighbour->getBottom());
            }
            cell->setLayout(cell->getLayoutMetrics());
            neighbour->setLayout(neighbour->getLayoutMetrics());
        }
        lastMaxBound = maxBoundNeighbour;
        lastMinBound = minBound;
    }

    /** Offset provider by react can be one frame behind the real one, it's important that this method is called with
     * offset taken directly from scrollview object*/
    Float AutoLayoutShadow::computeBlankFromGivenOffset(Float actualScrollOffset, Float distanceFromWindowStart,
                                                      Float distanceFromWindowEnd) {
        blankOffsetAtStart = lastMinBound - actualScrollOffset - distanceFromWindowStart;
        blankOffsetAtEnd = actualScrollOffset + windowSize - renderOffset - lastMaxBound - distanceFromWindowEnd;
        return MAX(blankOffsetAtStart, blankOffsetAtEnd);
    }

    /**It's importance to aviod correcting views outside the render window. An item that isn't being recycled might
     * still remain in the view tree. If views outside get considered then gaps between unused items will cause
     * algorithm to fail.*/
    bool AutoLayoutShadow::isWithinBounds(CellContainerComponentInstance &cell) {
        auto boundsStart = scrollOffset - renderOffset;
        auto boundsEnd = scrollOffset + windowSize;
        if (!this->horizontal) {
            return (cell.getTop() >= boundsStart || cell.getBottom() >= boundsStart) &&
                   (cell.getTop() <= boundsEnd || cell.getBottom() <= boundsEnd);
        } else {
            return (cell.getLeft() >= boundsStart || cell.getRight() >= boundsStart) &&
                   (cell.getLeft() <= boundsEnd || cell.getRight() <= boundsEnd);
        }
    }
} // namespace rnoh