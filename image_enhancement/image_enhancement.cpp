// image_enhancement.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include <opencv2/highgui/highgui.hpp>    
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
	Mat image = imread("F:\\cpp\\image_enhancement\\test_img\\1.bmp", IMREAD_COLOR);//IMREAD_GRAYSCALE   IMREAD_UNCHANGED   IMREAD_COLOR
	if (image.empty())
	{
		std::cout << "打开图片失败,请检查" << std::endl;
		return -1;
	}
	imshow("原图像", image);
	Mat imageRGB[3];
	split(image, imageRGB);
	for (int i = 0; i < 3; i++)
	{
		equalizeHist(imageRGB[i], imageRGB[i]);
	}
	merge(imageRGB, 3, image);
	imshow("直方图均衡化图像增强效果", image);
	waitKey();
	return 0;
}
