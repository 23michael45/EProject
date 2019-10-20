#ifndef TangramUtility_h__
#define TangramUtility_h__


#include "opencv2/opencv.hpp"
#include "Utility/Interface.h"
#include <math.h> 



/************************************************************************
*��������        get_point_angle
*
*�������ã�      ��֪2������㣬��� 0------->x ��ʱ������ת���ٽǶȵ���λ��
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
*����������
*CvPoint2D32f pointO  - ���
*CvPoint2D32f pointA  - �յ�
*
*��������ֵ��
*double         ����OA���� 0------->x ��ʱ������ת���ٽǶȵ���λ��
**************************************************************************/
double get_point_angle(cv::Point pointO, cv::Point pointA);


//Calcucate angle between vector A and B
//���ؽǶ�; return degree
int get_vector_angle(cv::Point v1,cv::Point v2);

double GetDist(cv::Point p1, cv::Point p2);

cv::Vec3b HSV_U2CV(cv::Vec3b c);

double GetColorDist(cv::Vec3b c1, cv::Vec3b c2);



cv::Mat GetHsvMask(cv::Mat src, cv::Vec3b color, double dist);

CPP_INTERFACE_API std::vector<cv::Point> FindLargestContour(cv::Mat src);


cv::Vec3b MeanColorInMask(cv::Mat frame, std::vector<cv::Point> contour);

CPP_INTERFACE_API void FindTangramContour(cv::Mat frame, cv::Mat draw,bool bGray = false);

#endif // TangramUtility_h__
