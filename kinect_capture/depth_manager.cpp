#include "depth_manager.h"



DepthManager::DepthManager()
{
	errorCheck(GetDefaultKinectSensor(&kinect));
	errorCheck(kinect->Open());

	IDepthFrameSource* depthFrameSource;
	errorCheck(kinect->get_DepthFrameSource(&depthFrameSource));
	errorCheck(depthFrameSource->OpenReader(&depth_frame_reader));

	IFrameDescription* depthFrameDescription;
	errorCheck(depthFrameSource->get_FrameDescription(&depthFrameDescription));
	errorCheck(depthFrameDescription->get_Height(&depth_height));
	errorCheck(depthFrameDescription->get_Width(&depth_width));

	depth_mat_raw = cv::Mat(depth_height, depth_width, CV_16UC1, cv::Scalar(0));
	depth_mat_converted = cv::Mat(depth_height, depth_width, CV_8UC1);
	time_stamp = 0;
}


DepthManager::~DepthManager()
{
}


void DepthManager::updateDepthFrame(){
	IDepthFrame* depthFrame;
	unsigned int bufferSize = depth_width * depth_height * sizeof(unsigned short);

	HRESULT ret = depth_frame_reader->AcquireLatestFrame(&depthFrame);

	if (FAILED(ret)) return;
    
	errorCheck(depthFrame->AccessUnderlyingBuffer(&bufferSize, reinterpret_cast<UINT16**>(&depth_mat_raw.data)));
	errorCheck(depthFrame->get_RelativeTime(&time_stamp));

	depth_mat_raw.convertTo(depth_mat_converted, CV_8U, 255.0f / 8000.0f, 0);

	depthFrame->Release();
}

cv::Mat DepthManager::getDepthMatRaw() {
    return depth_mat_raw;
}

cv::Mat DepthManager::getDepthMatConverted()
{
	return depth_mat_converted;
}

int DepthManager::getDepthWidth()
{
	return depth_width;
}

int DepthManager::getDepthHeight()
{
	return depth_height;
}

int64 DepthManager::getTimeStamp()
{
	return (int64 ) time_stamp;
}

void DepthManager::errorCheck(HRESULT ret)
{
	if (FAILED(ret)) 
		std::cerr << "failed : " <<  ret << std::endl;
}
