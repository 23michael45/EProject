#ifndef TangramUtility_h__
#define TangramUtility_h__


#include "opencv2/opencv.hpp"
#include "Utility/Interface.h"
#include <math.h> 



/************************************************************************
*函数名：        get_point_angle
*
*函数作用：      已知2个坐标点，求从 0------->x 逆时针需旋转多少角度到该位置
*
*                   |
*                   |
*                   |
*                   |
*------------------------------------> x
*                   | 0
*                   |
*                   |
*                   |
*                   v
*                   y
*
*函数参数：
*CvPoint2D32f pointO  - 起点
*CvPoint2D32f pointA  - 终点
*
*函数返回值：
*double         向量OA，从 0------->x 逆时针需旋转多少角度到该位置
**************************************************************************/
double get_point_angle(cv::Point pointO, cv::Point pointA);


//Calcucate angle between vector A and B
//返回角度; return degree
int get_vector_angle(cv::Point v1,cv::Point v2);

double GetDist(cv::Point p1, cv::Point p2);

cv::Vec3b HSV_U2CV(cv::Vec3b c);

double GetColorDist(cv::Vec3b c1, cv::Vec3b c2);



cv::Mat GetHsvMask(cv::Mat src, cv::Vec3b color, double dist);

CPP_INTERFACE_API std::vector<cv::Point> FindLargestContour(cv::Mat src);


cv::Vec3b MeanColorInMask(cv::Mat frame, std::vector<cv::Point> contour);

CPP_INTERFACE_API void FindTangramContour(cv::Mat frame, cv::Mat draw,bool bGray = false);

#endif // TangramUtility_h__
