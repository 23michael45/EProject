#include "TangramDetection.h"



using namespace cv;

cv::Vec3f HSV_SmallTriangle1(282, 52, 89);
cv::Vec3f HSV_SmallTriangle2(4, 29, 45);

cv::Vec3f HSV_MidTriangle(15, 88, 97);

cv::Vec3f HSV_BigTriangle1(158, 74, 54);
cv::Vec3f HSV_BigTriangle2(4, 88, 87);

cv::Vec3f HSV_Square(54, 83, 100);

cv::Vec3f HSV_Parallel(207, 87, 89);


bool TangramDetector::Init()
{
	HSVPiecesMap["SmallTriangle1"] = HSV_SmallTriangle1;
	HSVPiecesMap["SmallTriangle2"] = HSV_SmallTriangle2;
	HSVPiecesMap["MidTriangle"] = HSV_MidTriangle;
	HSVPiecesMap["BigTriangle1"] = HSV_BigTriangle1;
	HSVPiecesMap["BigTriangle2"] = HSV_BigTriangle2;
	HSVPiecesMap["Square"] = HSV_Square;
	HSVPiecesMap["Parallel"] = HSV_Parallel;

	m_Pieces[TangramElementInfo::TangrameType::TT_STRI] = std::vector<std::shared_ptr<TangramElementInfo>>(2);
	m_Pieces[TangramElementInfo::TangrameType::TT_MTRI] = std::vector<std::shared_ptr<TangramElementInfo>>(2);
	m_Pieces[TangramElementInfo::TangrameType::TT_LTRI] = std::vector<std::shared_ptr<TangramElementInfo>>(1);
	m_Pieces[TangramElementInfo::TangrameType::TT_STRI] = std::vector<std::shared_ptr<TangramElementInfo>>(1);
	m_Pieces[TangramElementInfo::TangrameType::TT_PARA] = std::vector<std::shared_ptr<TangramElementInfo>>(1);

	m_spStateMachine = std::make_shared<StateMachine<TangramDetector>>();
	ADD_STATE(m_spStateMachine, NoBasePieceState)
		ADD_STATE(m_spStateMachine, WithBasePieceState)
		return true;
}

void TangramDetector::DetectPieces()
{
	cv::Mat hsv;
	cv::cvtColor(m_curFrame, hsv, COLOR_BGR2HSV);

	float hsvDistThreshold = 20;


	int curIndex = 0;

	for (auto iter = HSVPiecesMap.begin(); iter != HSVPiecesMap.end(); iter++)
	{
		cv::Mat mask = GetHsvMask(hsv, HSV_U2CV(iter->second), hsvDistThreshold);

		cv::GaussianBlur(mask, mask, Size(5, 5), 0);

		cv::Mat element_9(9, 9, CV_8U, cv::Scalar(1));

		cv::morphologyEx(mask, mask, cv::MORPH_OPEN, element_9);
		cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, element_9);

		cv::threshold(mask, mask, 0, 255, THRESH_BINARY);

		//Canny(mask, mask, 100, 250);

		//cv::morphologyEx(mask, mask, cv::MORPH_GRADIENT, element_9);


		auto contour = FindLargestContour(mask);

		double epsilon = mask.rows / 16;

		std::vector<cv::Point> approxes;
		cv::approxPolyDP(contour, approxes, epsilon, true);


		std::shared_ptr<TangramElementInfo> spTangramInfo = std::make_shared<TangramElementInfo>();
		spTangramInfo->Init(approxes);

		if (spTangramInfo->Equal(m_spBaseElement))
		{

		}


		//m_Pieces[curIndex] = spTangramInfo;

		curIndex++;
	}

}

void TangramDetector::FindBase()
{
	if (m_spBaseElement != nullptr)
	{
		return;
	}

	cv::Mat hsv;
	cv::cvtColor(m_curFrame, hsv, COLOR_BGR2HSV);

	float hsvDistThreshold = 20;


	int curIndex = 0;
	int blIndex = -1;
	int blX = 999999;
	int blY = 999999;
	for (auto iter = HSVPiecesMap.begin(); iter != HSVPiecesMap.end(); iter++)
	{
		cv::Mat mask = GetHsvMask(hsv, HSV_U2CV(iter->second), hsvDistThreshold);


		cv::GaussianBlur(mask, mask, Size(5, 5), 0);

		cv::Mat element_9(9, 9, CV_8U, cv::Scalar(1));

		cv::morphologyEx(mask, mask, cv::MORPH_OPEN, element_9);
		cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, element_9);

		cv::threshold(mask, mask, 0, 255, THRESH_BINARY);

		//Canny(mask, mask, 100, 250);

		//cv::morphologyEx(mask, mask, cv::MORPH_GRADIENT, element_9);


		auto contour = FindLargestContour(mask);

		double epsilon = mask.rows / 16;

		std::vector<cv::Point> approxes;
		cv::approxPolyDP(contour, approxes, epsilon, true);


		std::shared_ptr<TangramElementInfo> spTangramInfo = std::make_shared<TangramElementInfo>();
		spTangramInfo->Init(approxes);

		if (spTangramInfo->Equal(m_spBaseElement))
		{

		}


		pieces[curIndex] = spTangramInfo;

		curIndex++;
	}


	for (int i = 0; i < pieces.size(); i++)
	{
		if (i == blIndex)
		{

		}
		else
		{

		}
	}
}

cv::Vec3b TangramDetector::HSV_U2CV(cv::Vec3b c)
{
	float h = c[0];
	float s = c[1];
	float v = c[2];

	return cv::Vec3b(h / 2, s / 100 * 255, v / 100 * 255);
}

double TangramDetector::GetColorDist(cv::Vec3b c1, cv::Vec3b c2)
{
	return sqrt(pow(c2[0] - c1[0], 2) + pow(c2[1] - c1[1], 2) + pow(c2[2] - c1[2], 2));
}

cv::Mat TangramDetector::GetHsvMask(cv::Mat src, cv::Vec3b color, double dist)
{
	//std::cout << src << std::endl;

	cv::Mat mask(src.size(), CV_8UC1);
	for (int c = 0; c < src.cols; c++)
	{
		for (int r = 0; r < src.rows; r++)
		{

			cv::Vec3b& col = src.at<cv::Vec3b>(r, c);

			double cdist = GetColorDist(col, color);
			if (cdist < dist)
			{
				mask.at<uchar>(r, c) = 255;
			}
			else
			{
				mask.at<uchar>(r, c) = 0;
			}
		}
	}
	return mask;
}

std::vector<cv::Point> TangramDetector::FindLargestContour(cv::Mat src)
{
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	findContours(src, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	int largest_area = 0;
	int largest_contour_index = 0;
	cv::Rect bounding_rect;

	for (int i = 0; i < contours.size(); i++) // iterate through each contour.
	{
		double a = contourArea(contours[i], false);  //  Find the area of contour
		if (a > largest_area) {
			largest_area = a;
			largest_contour_index = i;                //Store the index of largest contour
			bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
		}
	}

	//cv::Mat draw = src.clone();
	//drawContours(draw, contours, largest_contour_index, cv::Scalar(255, 0, 0), cv::FILLED, 8);
	//cv::imshow("img_rot", draw);
	//cv::waitKey();

	return contours[largest_contour_index];
}


void TangramDetector::Update(cv::Mat& frame)
{
	m_curFrame = frame;
	m_spStateMachine->updateWithDeltaTime();
}

//----------------------------------------------------------------------------------------------




cv::Mat rotateImage(cv::Mat img, int degree)
{
	//旋转中心为图像中心
	cv::Point center;
	center.x = float(img.cols / 2.0 + 0.5);
	center.y = float(img.rows / 2.0 + 0.5);
	//计算二维旋转的仿射变换矩阵
	float m[6];
	cv::Mat M = cv::Mat(2, 3, CV_32F, m);
	M = cv::getRotationMatrix2D(center, degree, 1);
	//变换图像，并用黑色填充其余值

	cv::Mat img_rotate;
	cv::warpAffine(img, img_rotate, M, img.size());
	return img_rotate;
}
void TangramDetector::Test()
{

	cv::Mat src = cv::imread("Image/Parallel.jpg");
	cv::Mat gray;
	cv::cvtColor(src, gray, COLOR_BGR2GRAY);
	cv::threshold(gray, gray, 200, 255, THRESH_BINARY_INV);

	int step = 15;
	for (int i = 0; i < 360; i += step)
	{
		auto img_rot = rotateImage(gray, i);
		auto contour = FindLargestContour(img_rot);

		double epsilon = img_rot.rows / 16;
		std::vector<cv::Point> approxes;
		cv::approxPolyDP(contour, approxes, epsilon, true);

		for (int i = 0; i < approxes.size(); i++)
		{
			approxes[i] = cv::Point(approxes[i].x, img_rot.rows - approxes[i].y);
		}
		ParallelElementInfo parallel;
		parallel.Init(approxes);

		printf("\n %d %f", parallel.IsFlip(), parallel.GetAngle());
	}

	printf("\n-----------------------------------------------------------------------------------------");

	cv::flip(gray, gray, 0);

	for (int i = 0; i < 360; i += step)
	{
		auto img_rot = rotateImage(gray, i);
		auto contour = FindLargestContour(img_rot);

		double epsilon = img_rot.rows / 16;
		std::vector<cv::Point> approxes;
		cv::approxPolyDP(contour, approxes, epsilon, true);

		for (int i = 0; i < approxes.size(); i++)
		{
			approxes[i] = cv::Point(approxes[i].x, img_rot.rows - approxes[i].y);
		}

		ParallelElementInfo parallel;
		parallel.Init(approxes);

		printf("\n %d %f", parallel.IsFlip(), parallel.GetAngle());
	}


}