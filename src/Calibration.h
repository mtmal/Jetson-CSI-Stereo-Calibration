////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2021 Mateusz Malinowski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#ifndef __CHESSBOARDDETECTOR_H__
#define __CHESSBOARDDETECTOR_H__

#include "DataStruct.h"

/**
 * This class provides all means for stereo camera calibration using checkerboard.
 */
class Calibration
{
public:
    /**
     * Basic constructor, initialises all variables.
     *  @param imageSize the size of images that cameras will be taking.
     *  @param boardSize the size of the calibration checkerboard.
     *  @param windowSize the size of a window for sub-pixel refinement.
     *  @param squareSize the size of an individual square in metres.
     */
    Calibration(const cv::Size& imageSize, const cv::Size& boardSize, const int windowSize, const float squareSize);

    /**
     * Empty destructor.
     */
    virtual ~Calibration();

    /**
     * Finds chess corner using provided greyscale image.
     * Corner points are assigned to raw points buffer and overlaid on coloured image.
     *  @param[in/out] data the structure which provides the latest image and stores all image points.
     */
    bool findChessCorners(SingleCamDataStruct& data) const;

    /**
     * Performs a single camera calibration using provided data. OpenCV's calibrateCamera may be called several times.
     * If the RMS for individual image points is higher than the threshold, these points are removed and the algorithm
     * is repeated. The aim is to dynamically removed any images that may have errors and corrupt the overall calibration.
     *  @param folder the folder where a file with camera intrinsics should be saved.
     *  @param[in/out] data the structure which provides all image points and which stores buffers for all calibration outputs.
     */
    void calibrateSingleCamera(const std::string& folder, SingleCamDataStruct& data) const;

    /**
     * Performs a stereo camera calibration using already-calibrated intrinsics from individual cameras. OpenCV's
     * stereoCalibrate may be called several times. If the RMS for any image is higher than the threshold, the whole
     * pair is removed and the algorithm is repeated. After calibration, calibration quality is checked by estimating
     * epipolar error. Then, stereoRectify is called and epipolar error is estimated again, this time using new camera
     * matrices and rectification rotations. However, it gives higher error than using non-rectified matrices.
     * TODO: why?
     *  @param folder the path to the folder where a file with stereo camera extrinsics should be saved.
     *  @param[in/out] stereo the data structure holding all stereo camera image points and which hold buffers for
     *  outputs of stereo calibration.
     */
    void calibrateStereoCamera(const std::string& folder, StereoCamDataStruct& stereo) const;

    /**
     *  @param width new value for checkerboard width.
     */
    inline void setBoardWidth(const int width)
    {
    	mBoardSize.width = width;
    }

    /**
     *  @param height new value for checkerboard height.
     */
    inline void setBoardHeight(const int height)
    {
    	mBoardSize.height = height;
    }

    /**
     *  @param windowSize new size for window for sub-pixel refinement.
     */
    inline void setWindowSize(const int windowSize)
    {
    	mWindowSize.width  = windowSize;
    	mWindowSize.height = windowSize;
    }

    /**
     *  @param squareSize new size of individual square in the same units as stereo baseline.
     */
    inline void setSquareSize(const float squareSize)
    {
    	mSquareSize = squareSize;
    }

private:
    /**
     * Populates the list with checkerboard corners coordinates.
     *  @param[out] corners the list the corners coordinates.
     */
    void calcChessboardCorners(std::vector<cv::Point3f>& corners) const;

    /** The size of images that will be calibrated. */
    cv::Size mImageSize;
    /** The size of calibration board. */
    cv::Size mBoardSize;
    /** Window size for sub-pixel refinement. */
    cv::Size mWindowSize;
    /** The size of a single square in the same units as stereo baseline. */
    float mSquareSize;
};

#endif // __CHESSBOARDDETECTOR_H__
