#include <opencv2\opencv.hpp>
#include <sstream>
#include <direct.h>

#include "depth_manager.h"

bool isZeroMat(cv::Mat &DepthMat)
{
	int sum_pixel = cv::sum(DepthMat)[0];
	if (sum_pixel < 10 * 0.5 * DepthMat.rows * DepthMat.cols)
		return true;
	else
		return false;
}

int main(int argc, char** argv)
{
	DepthManager dpm;
	//cv::Mat depth_raw(cv::Size(dpm.getDepthWidth(), dpm.getDepthHeight()), CV_16UC1);
	std::stringstream ss;
	std::string fileid = "2016_0221_043318";
	
	_mkdir(fileid.c_str());
	_mkdir("depth");
	// _mkdir("color");
	// cv::namedWindow("depth");

	while (1) {
		dpm.updateDepthFrame();
		if (dpm.getTimeStamp() > 0) break;
	}

	int64 now_time_stamp, past_time_stamp = 0;

	while (1) {
		dpm.updateDepthFrame();
	    now_time_stamp = dpm.getTimeStamp();
		if (now_time_stamp - past_time_stamp > 0) {
			std::string filename = fileid + "//" + std::to_string(now_time_stamp) + ".bmp";
			cv::imwrite(filename, dpm.getDepthMatRaw());
			past_time_stamp = now_time_stamp;
		}
		if (isZeroMat(dpm.getDepthMatConverted())) break;
		if (cv::waitKey(10) == 'q') break;
	}
	
	//getchar();

	return 0;
}