#pragma once
#include <opencv2/opencv.hpp>
#include <kinect.h>

class ColorManager
{
public:

	ColorManager(int depth_height, int depth_width);
	~ColorManager();

	void updateColorFrame(std::vector<UINT16> &depth_buffer);
	
	int getWidth();
	int getHeight();

	cv::Mat getCoordinatedMat();

private:

	void errorCheck(HRESULT ret);
	bool isValidColorRange(int x, int y);
	bool isValidDepthRange(int index, std::vector<UINT16> &depth_buffer);

	IKinectSensor* kinect;
	IColorFrameReader* color_frame_reader;

	int color_width;
	int color_height;
	const int color_channels = 4;

	std::vector<BYTE> color_vector;
	cv::Mat color_mat_coordinated;
};