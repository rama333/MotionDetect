# Motion detected for golang with opencv

_Example_

```
   webcam, err := gocv.OpenVideoCapture(deviceID)
   
   ...
   
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
   
```