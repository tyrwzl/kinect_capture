#pragma once
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <kinect.h>


class DepthManager
{
public:

	DepthManager();
	~DepthManager();

	void updateDepthFrame();

	cv::Mat getDepthMatRaw();
	cv::Mat getDepthMatConverted();

	int getDepthWidth();
	int getDepthHeight();

	int64 getTimeStamp();

private:

	void errorCheck(HRESULT ret);

	IKinectSensor* kinect;
	IDepthFrameReader* depth_frame_reader;

	int depth_width;
	int depth_height;

	cv::Mat depth_mat_raw;
	cv::Mat depth_mat_converted;

	TIMESPAN time_stamp;

};

