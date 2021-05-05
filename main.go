package main

import (
	"github.com/rama333/MotionDetect/motion"
	"fmt"
	"github.com/sirupsen/logrus"
	"gocv.io/x/gocv"
	"time"
)

var (
	deviceID = "0"
	diff_threshold = 30
)

func  main() {

	st := time.Now()

	defer func() {
		logrus.Infof("stopped in %s second", time.Now().Sub(st))
	}()

	logrus.SetLevel(logrus.DebugLevel)

	webcam, err := gocv.OpenVideoCapture(deviceID)

	if err != nil {
		fmt.Printf("Error opening video capture device: %v\n", deviceID)
		return
	}

	defer webcam.Close()

	window := gocv.NewWindow("Montion detect")
	window.ResizeWindow(1200, 1100)
	defer window.Close()

	img := gocv.NewMat()
	defer img.Close()

	buffImage := gocv.NewMat()

	for {

		//webcam.Grab()

		if ok := webcam.Read(&img); !ok {
			fmt.Printf("Device closed: %v\n", deviceID)
			return
		}

		if buffImage.Empty() {
			buffImage = img
			continue
		}

		motion.Detect(img, buffImage, diff_threshold)

		buffImage = img

		window.IMShow(img)
		if window.WaitKey(1) >= 0 {
			break
		}
	}
}