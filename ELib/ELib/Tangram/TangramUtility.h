#ifndef TangramUtility_h__
#define TangramUtility_h__


#include "opencv2/opencv.hpp"


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

double GetDist(cv::Point p1, cv::Point p2);
#endif // TangramUtility_h__
