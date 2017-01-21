#include <opencv2\opencv.hpp>
#include <sstream>
#include <direct.h>
#include <vector>

#include "depth_manager.h"
#include "color_manager.h"

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
	DepthManager depth_manager;
	ColorManager color_manager(depth_manager.getDepthHeight(), depth_manager.getDepthWidth());

	std::string fileid = "2016_0221_043318";
	std::string filepath = fileid;

	_mkdir(filepath.c_str());
	filepath = fileid + "//depth";
	_mkdir(filepath.c_str());
	filepath = fileid + "//color";
	_mkdir(filepath.c_str());

	while (1) {
		depth_manager.updateDepthFrame();
		if (depth_manager.getTimeStamp() > 0) break;
	}

	int64 now_time_stamp, past_time_stamp = 0;
	std::vector<int64> too_long_time_stamps;

	std::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(0);

	while (1) {
		depth_manager.updateDepthFrame();
		color_manager.updateColorFrame(depth_manager.getDepthMatRaw());
	    now_time_stamp = depth_manager.getTimeStamp();
		int64 diff_time_stamp = now_time_stamp - past_time_stamp;

		if (diff_time_stamp > 0) {
			if (diff_time_stamp > 400000) too_long_time_stamps.push_back(now_time_stamp);
			std::string filename = fileid + "//depth//" + std::to_string(now_time_stamp) + ".png";
			cv::imwrite(filename, depth_manager.getDepthMatRaw(), compression_params);
			filename = fileid + "//color//" + std::to_string(now_time_stamp) + ".bmp";
			cv::imwrite(filename, color_manager.getCoordinatedMat());
			past_time_stamp = now_time_stamp;
		}
		if (isZeroMat(depth_manager.getDepthMatConverted())) break;
		if (cv::waitKey(10) == 'q') break;
	}
	
	std::ofstream ofs(fileid + "//" + fileid + ".log", std::ios::out);

	for (int time_i = 0; time_i < too_long_time_stamps.size(); ++time_i) {
		ofs << too_long_time_stamps[time_i] << std::endl;
	}
	ofs.close();

	return 0;
}