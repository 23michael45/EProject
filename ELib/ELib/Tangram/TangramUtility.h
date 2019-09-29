#ifndef TangramUtility_h__
#define TangramUtility_h__


#include "opencv2/opencv.hpp"


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

double GetDist(cv::Point p1, cv::Point p2);
#endif // TangramUtility_h__
