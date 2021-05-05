package motion

/*
#cgo !windows pkg-config: opencv4
#cgo CXXFLAGS:   --std=c++11
#cgo windows  CPPFLAGS:   -IC:/opencv/build/install/include
#cgo windows  LDFLAGS:    -LC:/opencv/build/install/x64/mingw/lib -lopencv_core451 -lopencv_face451 -lopencv_videoio451 -lopencv_imgproc451 -lopencv_highgui451 -lopencv_imgcodecs451 -lopencv_objdetect451 -lopencv_features2d451 -lopencv_video451 -lopencv_dnn451 -lopencv_xfeatures2d451 -lopencv_plot451 -lopencv_tracking451 -lopencv_img_hash451 -lopencv_calib3d451 -lopencv_bgsegm451 -lopencv_photo451
#include <stdlib.h>
#include "motion.h"
*/
import "C"
import (
	"gocv.io/x/gocv"
	"unsafe"
)

func Detect(img gocv.Mat, dst gocv.Mat, diff int) (bool)  {
	if C.Update_mhi(unsafe.Pointer(img.Ptr()), unsafe.Pointer(dst.Ptr()), C.int(diff)) == 0 {
		return true
	}

	return false
}