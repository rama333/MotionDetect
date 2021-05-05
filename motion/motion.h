#ifndef _OPENCV3_CORE_H_
#define _OPENCV3_CORE_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <opencv2/opencv.hpp>
#include "opencv2/optflow.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
extern "C" {
#endif

#ifdef __cplusplus

typedef cv::Mat* Mat;
typedef cv::TermCriteria* TermCriteria;
#else
typedef void* Mat;
typedef void* TermCriteria;
#endif

int Update_mhi(const void* image, const void* dst, int diff_threshold);

#ifdef __cplusplus
}
#endif

#endif //_OPENCV3_CORE_H_
