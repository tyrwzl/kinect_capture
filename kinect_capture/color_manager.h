#pragma once
#include <opencv2/opencv.hpp>
#include <kinect.h>

class ColorManager
{
public:

	ColorManager();
	~ColorManager();

	void updateFrame();

	cv::Mat getRawMat();
	
	int getWidth();
	int getHeight();

private:

	void errorCheck(HRESULT ret);

	IKinectSensor* kinect;
	IColorFrameReader* color_frame_reader;

	int color_width;
	int color_height;

	cv::Mat color_mat_raw;

};