#include "depth_manager.h"



DepthManager::DepthManager()
{
	// get and open kinect sensor
	errorCheck(GetDefaultKinectSensor(&kinect));
	errorCheck(kinect->Open());

	// get depth frame source and frame reader
	IDepthFrameSource* depth_frame_source;
	errorCheck(kinect->get_DepthFrameSource(&depth_frame_source));
	errorCheck(depth_frame_source->OpenReader(&depth_frame_reader));

	// get width and height of a depth image
	IFrameDescription* depth_frame_description;
	errorCheck(depth_frame_source->get_FrameDescription(&depth_frame_description));
	errorCheck(depth_frame_description->get_Height(&depth_height));
	errorCheck(depth_frame_description->get_Width(&depth_width));

	// init member
	depth_mat_raw.create(depth_height, depth_width, CV_16UC1);
	depth_mat_converted.create(depth_height, depth_width, CV_8UC1);
	depth_vector.resize(depth_height * depth_width);
	time_stamp = 0;
}


DepthManager::~DepthManager()
{
}


void DepthManager::updateDepthFrame(){
	IDepthFrame* depthFrame;
	unsigned int buffer_size = depth_width * depth_height * sizeof(unsigned short);

	// AcquireLatestFrame() function may be failed, so this return void safely
	HRESULT ret = depth_frame_reader->AcquireLatestFrame(&depthFrame);
	if (FAILED(ret)) return;
    
	// for cv::Mat
	//errorCheck(depthFrame->AccessUnderlyingBuffer(&buffer_size, reinterpret_cast<UINT16**>(&depth_mat_raw.data)));
	// for cordinated mapping of color image
	//errorCheck(depthFrame->CopyFrameDataToArray(depth_vector.size(), &depth_vector[0]));
	errorCheck(depthFrame->CopyFrameDataToArray(depth_vector.size(), reinterpret_cast<UINT16*>(depth_mat_raw.data)));
	// get time stamp of depth image
	errorCheck(depthFrame->get_RelativeTime(&time_stamp));
	// convert 16bit depth data to 8 bit depth data
	depth_mat_raw.convertTo(depth_mat_converted, CV_8UC1, 255.0f / 8000.0f, 0);

	depthFrame->Release();
}

cv::Mat DepthManager::getDepthMatRaw() {
    return depth_mat_raw;
}

cv::Mat DepthManager::getDepthMatConverted()
{
	return depth_mat_converted;
}

std::vector<UINT16> DepthManager::getDepthVector()
{
	return depth_vector;
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
	return (int64) time_stamp;
}

void DepthManager::errorCheck(HRESULT ret)
{
	if (FAILED(ret)) 
		std::cerr << "failed : " <<  std::hex << ret << std::endl;
}
