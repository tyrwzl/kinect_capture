#pragma once
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <kinect.h>
#include <memory>


class DepthManager
{
public:

	DepthManager();
	~DepthManager();

	void updateDepthFrame();

	const cv::Mat& getDepthMatRaw() const {
		return depth_mat_raw;
	}

	cv::Mat getDepthMatConverted();
	std::vector<UINT16> getDepthVector();

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

	std::vector<UINT16> depth_vector;

	TIMESPAN time_stamp;

};

