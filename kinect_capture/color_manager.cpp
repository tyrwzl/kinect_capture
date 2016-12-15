#include "color_manager.h"

ColorManager::ColorManager(int depth_height, int depth_width)
{
	// get and open kinect sensor
	errorCheck(GetDefaultKinectSensor(&kinect));
	errorCheck(kinect->Open());

	// get depth frame source and frame reader
	IColorFrameSource* color_frame_source;
	errorCheck(kinect->get_ColorFrameSource(&color_frame_source));
	errorCheck(color_frame_source->OpenReader(&color_frame_reader));

	// get width and height of a depth image
	IFrameDescription* color_frame_description;
	errorCheck(color_frame_source->get_FrameDescription(&color_frame_description));
	errorCheck(color_frame_description->get_Height(&color_height));
	errorCheck(color_frame_description->get_Width(&color_width));

	//init member
	color_vector.resize(color_width * color_height * color_channels);
	color_mat_coordinated.create(depth_height, depth_width, CV_8UC4);
}

ColorManager::~ColorManager()
{
}

void ColorManager::updateColorFrame(std::vector<UINT16> &depth_vector)
{
	IColorFrame* color_frame;
	unsigned int buffer_size = color_width * color_height * color_channels *sizeof(unsigned char);

	// AcquireLatestFrame() function may be failed, so this return void safely.
	HRESULT ret = color_frame_reader->AcquireLatestFrame(&color_frame);
	if (FAILED(ret)) return;

	// Raw color image is YUV format, so it requests BGRA format.
	errorCheck(color_frame->CopyConvertedFrameDataToArray(buffer_size, &color_vector[0], ColorImageFormat_Bgra));
	
	ICoordinateMapper *mapper;
	errorCheck(kinect->get_CoordinateMapper(&mapper));

	std::vector<ColorSpacePoint> color_space_points(depth_vector.size());
	errorCheck(mapper->MapDepthFrameToColorSpace(depth_vector.size(), &depth_vector[0], color_space_points.size(), &color_space_points[0]));

	for (int i = 0; i < color_mat_coordinated.total(); ++i) {
		int x = (int)color_space_points[i].X;
		int y = (int)color_space_points[i].Y;

		int src_index = ((y * color_width) + x) * color_channels;
		int dest_index = i * color_channels;

		if (isValidColorRange(x, y) && isValidDepthRange(i, depth_vector)) {
			color_mat_coordinated.data[dest_index + 0] = color_vector[src_index + 0];
			color_mat_coordinated.data[dest_index + 1] = color_vector[src_index + 1];
			color_mat_coordinated.data[dest_index + 2] = color_vector[src_index + 2];
		}
		else {
			color_mat_coordinated.data[dest_index + 0] = 255;
			color_mat_coordinated.data[dest_index + 1] = 255;
			color_mat_coordinated.data[dest_index + 2] = 255;
		}
	}
}

int ColorManager::getWidth()
{
	return color_width;
}

int ColorManager::getHeight()
{
	return color_height;
}

cv::Mat ColorManager::getCoordinatedMat()
{
	return color_mat_coordinated;
}

void ColorManager::errorCheck(HRESULT ret)
{
	if (FAILED(ret))
		std::cerr << "failed : " << std::hex << ret << std::endl;
}

bool ColorManager::isValidColorRange(int x, int y)
{
	return ((0 <= x) && (x < color_width)) && ((0 <= y) && (y < color_height));
}

bool ColorManager::isValidDepthRange(int index, std::vector<UINT16> &depth_vector)
{
	return (500 <= depth_vector[index]) && (depth_vector[index] <= 8000);
}
