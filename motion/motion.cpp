#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>

#include "motion.h"

using namespace cv;
using namespace std;
using namespace cv::motempl;

cv::Mat mhi, orient, mask, segmask, zplane;
vector<cv::Rect> regions;

// various tracking parameters (in seconds)
const double MHI_DURATION = 1;
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;

// ring image buffer
vector<cv::Mat> buf;
int last = 0;


int Update_mhi(const void* imag, const void* ds, int diff_threshold)
{
    buf.resize(2);

    cv::Mat* a = (cv::Mat*)imag;
    cv::Mat* b = (cv::Mat*)ds;

    const cv::Mat img = *a;
    const cv::Mat dst = *b;

    double timestamp = (double)clock() / CLOCKS_PER_SEC; // get current time in seconds
    cv::Size size = img.size();
    int i, idx1 = last;
    cv::Rect comp_rect;
    double count;
    double angle;
    cv::Point center;
    double magnitude;
    cv::Scalar color;

    // allocate images at the beginning or
    // reallocate them if the frame size is changed
    if (mhi.size() != size) {
        mhi = cv::Mat::zeros(size, CV_32F);
        zplane = cv::Mat::zeros(size, CV_8U);


        buf[0] = cv::Mat::zeros(size, CV_8U);
        buf[1] = cv::Mat::zeros(size, CV_8U);
    }

    cv::cvtColor(img, buf[last], COLOR_BGR2GRAY); // convert frame to grayscale

    int idx2 = (last + 1) % 2; // index of (last - (N-1))th frame
    last = idx2;

    cv::Mat silh = buf[idx2];
    absdiff(buf[idx1], buf[idx2], silh); // get difference between frames

    threshold(silh, silh, diff_threshold, 1, THRESH_BINARY); // and threshold it
    updateMotionHistory(silh, mhi, timestamp, MHI_DURATION); // update MHI

    // convert MHI to blue 8u image
    mhi.convertTo(mask, CV_8U, 255. / MHI_DURATION, (MHI_DURATION - timestamp)*255. / MHI_DURATION);

    cv::Mat planes[] = { mask, zplane, zplane };
    cv::merge(planes, 3, dst);

    // calculate motion gradient orientation and valid orientation mask
    calcMotionGradient(mhi, mask, orient, MAX_TIME_DELTA, MIN_TIME_DELTA, 3);

    // segment motion: get sequence of motion components
    // segmask is marked motion components map. It is not used further
    regions.clear();
    segmentMotion(mhi, segmask, regions, timestamp, MAX_TIME_DELTA);

    // iterate through the motion components,
    // One more iteration (i == -1) corresponds to the whole image (global motion)
    for (i = -1; i < (int)regions.size(); i++) {

        if (i < 0) { // case of the whole image
            comp_rect = Rect(0, 0, size.width, size.height);
            color = Scalar(255, 255, 255);
            magnitude = 100;
        }
        else { // i-th motion component
            comp_rect = regions[i];
            if (comp_rect.width + comp_rect.height < 100) // reject very small components
                continue;
            color = Scalar(0, 0, 255);
            magnitude = 30;
        }

        // select component ROI
        cv::Mat silh_roi = silh(comp_rect);
        cv::Mat mhi_roi = mhi(comp_rect);
        cv::Mat orient_roi = orient(comp_rect);
        cv::Mat mask_roi = mask(comp_rect);

        // calculate orientation
        angle = calcGlobalOrientation(orient_roi, mask_roi, mhi_roi, timestamp, MHI_DURATION);
        angle = 360.0 - angle;  // adjust for images with top-left origin

        count = norm(silh_roi, NORM_L1);; // calculate number of points within silhouette ROI

        // check for the case of little motion
        if (count < comp_rect.width*comp_rect.height * 0.05)
            continue;

        // draw a clock with arrow indicating the direction
        center = Point((comp_rect.x + comp_rect.width / 2),
            (comp_rect.y + comp_rect.height / 2));

            return 1;
    }

    return 0;
}
