#include "color_manager.h"

ColorManager::ColorManager()
{
	errorCheck(GetDefaultKinectSensor(&kinect));
	errorCheck(kinect->Open());

	IColorFrameSource* color_frame_source;
	errorCheck(kinect->get_ColorFrameSource(&color_frame_source));
	errorCheck(color_frame_source->OpenReader(&color_frame_reader));

	IFrameDescription* color_frame_description;
	errorCheck(color_frame_source->get_FrameDescription(&color_frame_description));
	errorCheck(color_frame_description->get_Height(&color_height));
	errorCheck(color_frame_description->get_Width(&color_width));

	color_mat_raw = cv::Mat(color_height, color_width, CV_8UC3, cv::Scalar(0));
}

ColorManager::~ColorManager()
{
}

void ColorManager::updateFrame()
{
	IColorFrame* color_frame;
	unsigned int buffer_size = color_width * color_height * sizeof(unsigned char);

	HRESULT ret = color_frame_reader->AcquireLatestFrame(&color_frame);

	if (FAILED(ret)) return;

	errorCheck(color_frame->AccessRawUnderlyingBuffer(&buffer_size, reinterpret_cast<BYTE**>(&color_mat_raw.data)));


}

cv::Mat ColorManager::getRawMat()
{
	return color_mat_raw;
}

int ColorManager::getWidth()
{
	return color_width;
}

int ColorManager::getHeight()
{
	return color_height;
}

void ColorManager::errorCheck(HRESULT ret)
{
	if (FAILED(ret))
		std::cerr << "failed : " << ret << std::endl;
}
