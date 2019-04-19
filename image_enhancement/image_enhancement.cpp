// image_enhancement.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include <opencv2/highgui/highgui.hpp>    
#include <opencv2/imgproc/imgproc.hpp>

#include <windows.h>
#include <io.h>

using namespace std;
using namespace cv;

/*取得指定文件夹下所有的文件名，不包括目录递归*/
void getFiles(string path, vector<string>& files)
{
	//文件句柄  
	long long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			/*if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else*/
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void Histogram(Mat &inputImage, Mat &outputImage)
{
	Mat imageRGB[3];
	split(inputImage, imageRGB);
	for (int i = 0; i < 3; i++)
	{
		equalizeHist(imageRGB[i], imageRGB[i]);
	}
	merge(imageRGB, 3, outputImage);
}


void Laplace(Mat &inputImage, Mat &outputImage)
{
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
	filter2D(inputImage, outputImage, CV_8UC3, kernel);
}


void Log(Mat &inputImage, Mat &outputImage)
{
	outputImage.create(inputImage.size(), CV_32FC3);
	for (int i = 0; i < inputImage.rows; i++)
	{
		for (int j = 0; j < inputImage.cols; j++)
		{
			outputImage.at<Vec3f>(i, j)[0] = log(1 + inputImage.at<Vec3b>(i, j)[0]);
			outputImage.at<Vec3f>(i, j)[1] = log(1 + inputImage.at<Vec3b>(i, j)[1]);
			outputImage.at<Vec3f>(i, j)[2] = log(1 + inputImage.at<Vec3b>(i, j)[2]);
		}
	}
	//归一化到0~255  
	normalize(outputImage, outputImage, 0, 255, CV_MINMAX);
	//转换成8bit图像显示  
	convertScaleAbs(outputImage, outputImage);
}



void Gamma(Mat &inputImage, Mat &outputImage)
{
	outputImage.create(inputImage.size(), CV_32FC3);
	for (int i = 0; i < inputImage.rows; i++)
	{
		for (int j = 0; j < inputImage.cols; j++)
		{
			outputImage.at<Vec3f>(i, j)[0] = (inputImage.at<Vec3b>(i, j)[0])*(inputImage.at<Vec3b>(i, j)[0])*(inputImage.at<Vec3b>(i, j)[0]);
			outputImage.at<Vec3f>(i, j)[1] = (inputImage.at<Vec3b>(i, j)[1])*(inputImage.at<Vec3b>(i, j)[1])*(inputImage.at<Vec3b>(i, j)[1]);
			outputImage.at<Vec3f>(i, j)[2] = (inputImage.at<Vec3b>(i, j)[2])*(inputImage.at<Vec3b>(i, j)[2])*(inputImage.at<Vec3b>(i, j)[2]);
		}
	}
	//归一化到0~255  
	normalize(outputImage, outputImage, 0, 255, CV_MINMAX);
	//转换成8bit图像显示  
	convertScaleAbs(outputImage, outputImage);
}


void ShowEnhancement()
{
	Mat inputImage = imread("F:\\cpp\\image_enhancement\\test_img\\1.bmp", IMREAD_COLOR);//IMREAD_GRAYSCALE   IMREAD_UNCHANGED   IMREAD_COLOR
	if (inputImage.empty())
	{
		std::cout << "打开图片失败,请检查" << std::endl;
	}
	imshow("原图像", inputImage);

	Mat outputImage;
	//Histogram(inputImage, outputImage);
	//Laplace(inputImage, outputImage);
	Log(inputImage, outputImage);
	//Gamma(inputImage, outputImage);

	imshow("output", outputImage);
	waitKey();
}

int SaveEnhancement(string inputDir, string outputDir)
{
	//获取该路径下的所有文件  
	vector<string> files;
	getFiles(inputDir, files);
	for (int i = 0; i < files.size(); i++)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		
		_splitpath_s(files[i].c_str() , drive, dir, fname, ext);
		if (string(ext) == ".png" || string(ext) == ".bmp" || string(ext) == ".jpg")
		{
			//读入图像
			Mat inputImage = imread(files[i], IMREAD_COLOR);//IMREAD_GRAYSCALE   IMREAD_UNCHANGED   IMREAD_COLOR

			if (inputImage.empty())
			{
				std::cout << "打开图片失败,请检查" << std::endl;
				return -1;
			}
			cout << files[i].c_str() << endl;
			cout << "height is " << inputImage.rows << " width is " << inputImage.cols << " channel is " << inputImage.channels() << endl;

			Mat outputImage;
			Histogram(inputImage, outputImage);
			//Laplace(inputImage, outputImage);
			//Log(inputImage, outputImage);
			//Gamma(inputImage, outputImage);
			string savePath = outputDir;
			savePath = savePath.append("\\").append(fname).append(ext);
			imwrite(savePath, outputImage);
		}
	}
	return 0;
}
int main(int argc, char *argv[])
{
	SaveEnhancement("E:\\test\\test","G:\\output");
}


