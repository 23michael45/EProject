#include "Tangram/TangramData.h"

TangramElementInfo::TangramElementInfo()
{
	
}

void TangramElementInfo::Init(std::vector<cv::Point> contour, TangramTypeName tName)
{
	mType = Name2Type(tName);
	mTypeName = tName;

	mContour = contour;
	cv::Moments M = cv::moments(contour);
	mCenterPt.x = int(M.m10 / M.m00);
	mCenterPt.y = int(M.m01 / M.m00);

	mArea = cv::contourArea(contour);
	mPerimeter = cv::arcLength(contour, false);

	mBoundingRect = minAreaRect(contour);


	//Point2f P[4];
	//rect.points(P);

	//for (int j = 0; j <= 3; j++)
	//{
	//	line(contourImg, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 1);
	//	line(contourImg, P[j], P[(j + 1) % 4], Scalar(111), 2);
	//}

	//
	////绘制轮廓的最小外结圆
	//Point2f center; float radius;
	//minEnclosingCircle(contours[i], center, radius);
	//circle(画布1, center, radius, Scalar(255), 2);
}

bool TangramElementInfo::PoseEqual(std::shared_ptr<TangramElementInfo> spInfo)
{
	double threshold = 5.0f;
	if (mType == spInfo->mType)
	{
		double ang0 = GetAngle();
		double ang1 = spInfo->GetAngle();
		if (abs(ang0 - ang1) < threshold)
		{
			return true;
		}
	}
	return false;
}

bool TangramElementInfo::ShapeEqual(std::shared_ptr<TangramElementInfo> spInfo)
{
	double threshold = 5.0f;
	if (mType == spInfo->mType)
	{
		if (abs(mPerimeter - spInfo->mPerimeter) < threshold)
		{
			return true;
		}
	}
	return false;
}

bool TangramElementInfo::DistEqual(std::shared_ptr<TangramElementInfo> spBaseInfo)
{
	double threshold = 5.0f;
	if (mType == spBaseInfo->mType)
	{
		if (GetDist(mCenterPt , spBaseInfo->mCenterPt) < threshold)
		{
			return true;
		}
	}
}

double TangramElementInfo::GetAngle()
{
	return 0;
}

TriangleElementInfo::TriangleElementInfo()
{

}

double TriangleElementInfo::GetAngle()
{
	double dist0 = GetDist(mContour[0], mContour[1]);
	double dist1 = GetDist(mContour[1], mContour[2]);
	double dist2 = GetDist(mContour[2], mContour[0]);

	double maxdist = std::max(dist0, std::max(dist1, dist2));
	if (maxdist == dist0)
	{
		return get_point_angle(mContour[0], mContour[1]);
	}
	else if (maxdist == dist1)
	{
		return get_point_angle(mContour[1], mContour[2]);
	}
	else if (maxdist == dist2)
	{
		return get_point_angle(mContour[2], mContour[0]);
	}
}

SquareElementInfo::SquareElementInfo()
{

}

double SquareElementInfo::GetAngle()
{
	auto vec = mContour[0] - mContour[1];
	double ang = get_vector_angle(cv::Point(1, 0), vec);

	if (ang < 0)
	{
		ang += 360;
	}

	ang = fmod(ang,90);

	return ang;
}

ParallelElementInfo::ParallelElementInfo()
{

}

double ParallelElementInfo::GetAngle()
{
	cv::Point e0 = mContour[1] - mContour[0];
	cv::Point e1 = mContour[2] - mContour[1];

	auto v2 = cv::Point2f(1.0f, 0.0f);
	auto v0 = cv::Point2f(0.0f, 0.0f);

	double theta = 0;
	if (Magnitude(e0) > Magnitude(e1))
	{
		auto v1 = cv::Point2f(float(e0.x), float(e0.y));
		theta = getAngelOfTwoVector(v1, v2, v0);

		if (theta < 0)
		{
			theta += 180;
		}
		if (theta > 180)
		{
			theta -= 180;
		}
	}
	else
	{

		auto v1 = cv::Point2f(float(e1.x), float(e1.y));
		theta = getAngelOfTwoVector(v1, v2, v0);

		if (theta < 0)
		{
			theta += 180;
		}
		if (theta > 180)
		{
			theta -= 180;
		}
	}
	return theta;
}

double ParallelElementInfo::getAngelOfTwoVector(cv::Point2f &pt1, cv::Point2f &pt2, cv::Point2f &c)
{
	double theta = atan2(pt1.x - c.x, pt1.y - c.y) - atan2(pt2.x - c.x, pt2.y - c.y);
	if (theta > CV_PI)
		theta -= 2 * CV_PI;
	if (theta < -CV_PI)
		theta += 2 * CV_PI;

	theta = theta * 180.0 / CV_PI;
	return theta;
}

double ParallelElementInfo::Magnitude(cv::Point vec)
{
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
}

bool ParallelElementInfo::IsFlip()
{
	cv::Point e0 = mContour[1] - mContour[0];
	cv::Point e1 = mContour[2] - mContour[1];

	auto v1 = cv::Point2f((float)e0.x, (float)e0.y);
	auto v2 = cv::Point2f((float)e1.x, (float)e1.y);
	auto v0 = cv::Point2f(0.0f, 0.0f);
	double theta = getAngelOfTwoVector(v1, v2, v0);

	if (Magnitude(e0) > Magnitude(e1))
	{
		if (theta < 90 && theta >= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (theta < 90 && theta >= 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}
//--------------------------------------------------------------------------------------------------------
TangramGraph::TangramGraph(std::vector<TangramElementInfo::TangramTypeName> typeNameVector)
{
	for (auto iter = typeNameVector.begin(); iter != typeNameVector.end(); iter++)
	{
		m_ElementsMap[*iter] = nullptr;
	}

	ClearElement();

}

TangramGraph::~TangramGraph()
{
	ClearElement();
}

bool TangramGraph::InitAsTemplate(cv::Mat hsvFrame,std::map<TangramElementInfo::TangramTypeName, cv::Vec3b> &HSVMap)
{
	std::vector<std::vector<cv::Point>> contours = FindTangramContours(hsvFrame);
	if (contours.size() != 7)
	{
		return false;
	}
	else
	{
		int index = 0;
		if (contours[index].size() == 3)
		{
			std::shared_ptr<TriangleElementInfo> sp = std::make_shared<TriangleElementInfo>();
			sp->Init(contours[index], TangramElementInfo::TangramTypeName::TTN_LTRI_1);

			m_ElementsMap[sp->mTypeName] = sp;
		}

		index = 1;
		if (contours[index].size() == 3)
		{
			std::shared_ptr<TriangleElementInfo> sp = std::make_shared<TriangleElementInfo>();
			sp->Init(contours[index], TangramElementInfo::TangramTypeName::TTN_LTRI_2);

			m_ElementsMap[sp->mTypeName] = sp;
		}

		index = 2;
		if (contours[index].size() == 3)
		{
			std::shared_ptr<TriangleElementInfo> sp = std::make_shared<TriangleElementInfo>();
			sp->Init(contours[index], TangramElementInfo::TangramTypeName::TTN_MTRI);

			m_ElementsMap[sp->mTypeName] = sp;
		}
		else if (contours[index].size() == 4)
		{
			std::shared_ptr<ParallelElementInfo> sp = std::make_shared<ParallelElementInfo>();
			sp->Init(contours[index], TangramElementInfo::TangramTypeName::TTN_PARA);

			m_ElementsMap[sp->mTypeName] = sp;
		}




		index = 3;
		if (contours[index].size() == 3)
		{
			std::shared_ptr<TriangleElementInfo> sp = std::make_shared<TriangleElementInfo>();
			sp->Init(contours[index], TangramElementInfo::TangramTypeName::TTN_MTRI);

			m_ElementsMap[sp->mTypeName] = sp;
		}
		else if (contours[index].size() == 4)
		{
			std::shared_ptr<ParallelElementInfo> sp = std::make_shared<ParallelElementInfo>();
			sp->Init(contours[index], TangramElementInfo::TangramTypeName::TTN_PARA);

			m_ElementsMap[sp->mTypeName] = sp;
		}



		index = 4;
		if (contours[index].size() == 4)
		{
			std::shared_ptr<SquareElementInfo> sp = std::make_shared<SquareElementInfo>();
			sp->Init(contours[index], TangramElementInfo::TangramTypeName::TTN_SQR);

			m_ElementsMap[sp->mTypeName] = sp;
		}

		index = 5;
		if (contours[index].size() == 3)
		{
			std::shared_ptr<TriangleElementInfo> sp = std::make_shared<TriangleElementInfo>();
			sp->Init(contours[index], TangramElementInfo::TangramTypeName::TTN_STRI_1);

			m_ElementsMap[sp->mTypeName] = sp;
		}

		index = 6;
		if (contours[index].size() == 3)
		{
			std::shared_ptr<TriangleElementInfo> sp = std::make_shared<TriangleElementInfo>();
			sp->Init(contours[index], TangramElementInfo::TangramTypeName::TTN_STRI_2);

			m_ElementsMap[sp->mTypeName] = sp;
		}

		for (auto iter : m_ElementsMap)
		{
			if (iter.second == nullptr)
			{
				return false;
			}
		}



		std::vector<cv::Mat> hsvChannels;
		for (auto iter : m_ElementsMap)
		{
			cv::Vec3b hsvMean = MeanColorInMask(hsvFrame, iter.second->mContour);
			HSVMap[iter.first] = hsvMean;
		}

		return true;
	}

}

void TangramGraph::ClearElement()
{
	for (auto iter = m_ElementsMap.begin(); iter != m_ElementsMap.end(); iter++)
	{
		iter->second = nullptr;
	}
}

void TangramGraph::DetectPieces(cv::Mat frame,std::map<TangramElementInfo::TangramTypeName, cv::Vec3b> HSVMap)
{
	cv::Mat hsv;


	cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

	ClearElement();

	float hsvDistThreshold = 15;


	for (auto iter = HSVMap.begin(); iter != HSVMap.end(); iter++)
	{
		cv::Mat mask = GetHsvMask(hsv, iter->second, hsvDistThreshold);
		//cv::Mat mask = GetHsvMask(hsv, HSV_U2CV(iter->second), hsvDistThreshold);


		cv::GaussianBlur(mask, mask, cv::Size(5, 5), 0);

		//cv::Size morphSize = frame.size() / 50;
		cv::Size morphSize(6, 6);
		cv::Mat morph(morphSize, CV_8U, cv::Scalar(1));

		cv::erode(mask, mask, morph);
		cv::dilate(mask, mask, morph);

		//cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, morph);
		//cv::morphologyEx(mask, mask, cv::MORPH_OPEN, morph);

		cv::threshold(mask, mask, 0, 255, cv::THRESH_BINARY);

		//cv::imshow("mask", mask);
		//cv::waitKey();

		//Canny(mask, mask, 100, 250);

		//cv::morphologyEx(mask, mask, cv::MORPH_GRADIENT, element_9);

		


		auto contour = FindLargestContour(mask);
		if (contour.size() > 0)
		{
			double epsilon = mask.rows / 16;

			std::vector<cv::Point> approxes;
			cv::approxPolyDP(contour, approxes, epsilon, true);

			FillPieceData(approxes, iter->first);
		}
	}
}

bool compArea(const std::vector<cv::Point> &a, const std::vector<cv::Point> &b)
{
	double a_area = cv::contourArea(a);
	double b_area = cv::contourArea(b);

	return a_area > b_area;
}
bool compPerimeter(const std::vector<cv::Point> &a, const std::vector<cv::Point> &b)
{
	double a_perimeter = cv::arcLength(a,true);
	double b_perimeter = cv::arcLength(b,true);

	return a_perimeter > b_perimeter;
}

std::vector<std::vector<cv::Point>> TangramGraph::FindTangramContours(cv::Mat hsvFrame)
{
	std::vector<cv::Mat> hsvChannels;

	cv::split(hsvFrame, hsvChannels);


	cv::Mat gray;
	cv::threshold(hsvChannels[1], gray, 64, 255, cv::THRESH_BINARY_INV);


	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	//cv::imshow("hsvChannels[0]", hsvChannels[0]);
	cv::imshow("hsvChannels[1]", hsvChannels[1]);
	//cv::imshow("hsvChannels[2]", hsvChannels[2]);
	//cv::imshow("gray", gray);
	//cv::waitKey();

	findContours(gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

	double frameArea = gray.cols * gray.rows;

	std::vector<std::vector<cv::Point>> tangramContours;
	for (int i = 0; i < contours.size(); i++)
	{
		auto contour = contours[i];
		double area = cv::contourArea(contour);
		if (area < frameArea / 4 && area > frameArea / 1024)
		{
			tangramContours.push_back(contour);
		}
	}

	std::vector<std::vector<cv::Point>> approxesVector;
	for (int i = 0; i < tangramContours.size(); i++)
	{
		auto contour = tangramContours[i];

		double epsilon = gray.rows / 16;
		std::vector<cv::Point> approxes;
		cv::approxPolyDP(contour, approxes, epsilon, true);

		if (approxes.size() == 3 || approxes.size() == 4)
		{
			approxesVector.emplace_back(approxes);

		}


	}

	std::sort(approxesVector.begin(), approxesVector.end(), compPerimeter);

	return approxesVector;

}

void TangramGraph::FillWithContours(std::vector<std::vector<cv::Point>> contours, cv::Mat hsvFrame, std::map<TangramElementInfo::TangramTypeName, cv::Vec3b> HSVMap)
{
	for (int i = 0; i < contours.size(); i++)
	{
		auto contour = contours[i];

		cv::Point pos;
		cv::Moments M = cv::moments(contour);
		pos.x = int(M.m10 / M.m00);
		pos.y = int(M.m01 / M.m00);

		if (pos.x < 0 || pos.x >= hsvFrame.cols || pos.y < 0 || pos.y >= hsvFrame.rows)
		{
			continue;
		}

		cv::Vec3b hsvColor = hsvFrame.at<cv::Vec3b>(pos.y, pos.x);


		//find min hsv color
		int minHDist = 9999;
		TangramElementInfo::TangramTypeName tName;
		cv::Vec3b minHsvColor;
		for (auto iter = HSVMap.begin(); iter != HSVMap.end(); iter++)
		{

			if (contour.size() == 4 && iter->first != TangramElementInfo::TTN_PARA && iter->first != TangramElementInfo::TTN_SQR)
			{
				continue;
			}
			if (contour.size() == 3 && (iter->first == TangramElementInfo::TTN_PARA || iter->first == TangramElementInfo::TTN_SQR))
			{
				continue;
			}

			int dist = abs(hsvColor[0] - iter->second[0]);
			if (dist > 90)
			{
				dist = 180 - dist;
			}
			if (dist < minHDist)
			{
				minHDist = dist;
				minHsvColor = iter->second;
				tName = iter->first;
			}
		}

		FillPieceData(contour, tName);
	}
}

bool TangramGraph::PiecesCenterDistByName(TangramElementInfo::TangramTypeName basePieceName, TangramElementInfo::TangramTypeName relativePieceName, cv::Point& dist)
{
	if (m_ElementsMap[basePieceName] != nullptr &&
		m_ElementsMap[relativePieceName] != nullptr)
	{
		auto basePiece = m_ElementsMap[basePieceName];
		auto relativePiece = m_ElementsMap[relativePieceName];

		dist = relativePiece->mCenterPt - basePiece->mCenterPt;

		return true;
	}
	else
	{
		return false;
	}
}


std::vector<std::shared_ptr<TangramElementInfo>> TangramGraph::FindInfosByType(TangramElementInfo::TangramType type)
{
	std::vector< std::shared_ptr<TangramElementInfo>> vec;
	for (auto iter = m_ElementsMap.begin(); iter != m_ElementsMap.end(); iter++)
	{
		if (iter->second != nullptr)
		{
			if (iter->second->mType == type)
			{
				vec.emplace_back(iter->second);
			}

		}
	}
	return vec;
}

void TangramGraph::FillPieceData(std::vector<cv::Point> approxes, TangramElementInfo::TangramTypeName typeName)
{
	if (TangramElementInfo::Name2Type(typeName) == TangramElementInfo::TT_STRI ||
		TangramElementInfo::Name2Type(typeName) == TangramElementInfo::TT_MTRI ||
		TangramElementInfo::Name2Type(typeName) == TangramElementInfo::TT_LTRI)
	{
		if (approxes.size() == 3)
		{

			std::shared_ptr<TriangleElementInfo> spInfo = std::make_shared<TriangleElementInfo>();
			spInfo->Init(approxes, typeName);

			m_ElementsMap[typeName] = spInfo;
		}
		else
		{

			m_ElementsMap[typeName] = nullptr;
		}
	}
	else if (TangramElementInfo::Name2Type(typeName) == TangramElementInfo::TT_PARA)
	{
		if (approxes.size() == 4)
		{

			std::shared_ptr<ParallelElementInfo> spInfo = std::make_shared<ParallelElementInfo>();
			spInfo->Init(approxes, typeName);

			m_ElementsMap[typeName] = spInfo;
		}
		else
		{

			m_ElementsMap[typeName] = nullptr;
		}
	}
	else if (TangramElementInfo::Name2Type(typeName) == TangramElementInfo::TT_SQR)
	{
		if (approxes.size() == 4)
		{

			std::shared_ptr<SquareElementInfo> spInfo = std::make_shared<SquareElementInfo>();
			spInfo->Init(approxes, typeName);

			m_ElementsMap[typeName] = spInfo;
		}
		else
		{

			m_ElementsMap[typeName] = nullptr;
		}
	}
}
