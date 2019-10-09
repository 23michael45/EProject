#include "TangramDetection.h"



using namespace cv;




bool TangramDetector::Init(cv::Mat templateFrame)
{
	m_TypeVector.emplace_back(TangramElementInfo::TangramType::TT_STRI);
	m_TypeVector.emplace_back(TangramElementInfo::TangramType::TT_MTRI);
	m_TypeVector.emplace_back(TangramElementInfo::TangramType::TT_LTRI);
	m_TypeVector.emplace_back(TangramElementInfo::TangramType::TT_SQR);
	m_TypeVector.emplace_back(TangramElementInfo::TangramType::TT_PARA);

	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_STRI_1);
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_STRI_2);
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_MTRI);
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_LTRI_1);
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_LTRI_2);
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_SQR);
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_PARA);

	//cv::Vec3f TT_STRI_1(282, 51, 90);
	//cv::Vec3f TT_STRI_2(4, 29, 45);
	//cv::Vec3f TT_MTRI(15, 88, 97);
	//cv::Vec3f TT_LTRI_1(158, 74, 54);
	//cv::Vec3f TT_LTRI_2(4, 88, 87);
	//cv::Vec3f TT_SQR(54, 83, 100);
	//cv::Vec3f TT_PARA(207, 87, 89);
	//m_HSVMap[TangramElementInfo::TangramTypeName::TTN_STRI_1] = TT_STRI_1;
	//m_HSVMap[TangramElementInfo::TangramTypeName::TTN_STRI_2] = TT_STRI_2;
	//m_HSVMap[TangramElementInfo::TangramTypeName::TTN_MTRI] = TT_MTRI;
	//m_HSVMap[TangramElementInfo::TangramTypeName::TTN_LTRI_1] = TT_LTRI_1;
	//m_HSVMap[TangramElementInfo::TangramTypeName::TTN_LTRI_2] = TT_LTRI_2;
	//m_HSVMap[TangramElementInfo::TangramTypeName::TTN_SQR] = TT_SQR;
	//m_HSVMap[TangramElementInfo::TangramTypeName::TTN_PARA] = TT_PARA;


	m_spTemplateGraph = std::make_shared<TangramGraph>(m_TypeNameVector);



	cv::Mat hsv;
	cv::cvtColor(templateFrame, hsv, cv::COLOR_BGR2HSV);

	if(m_spTemplateGraph->InitAsTemplate(hsv,m_HSVMap))
	{
		m_spCurrentGraph = std::make_shared<TangramGraph>(m_TypeNameVector);
		return true;
	}
	return false;

	//m_spStateMachine = std::make_shared<StateMachine<TangramDetector>>();
	//ADD_STATE(m_spStateMachine, NoBasePieceState)
	//ADD_STATE(m_spStateMachine, WithBasePieceState)

	//m_spStateMachine->enterState<NoBasePieceState>();

}


//如果有两个同类块，根据向量角度进行匹配
std::map<std::shared_ptr<TangramElementInfo>, std::vector< std::shared_ptr<TangramElementInfo>>> GetPairFromTypeVec(std::vector<std::shared_ptr<TangramElementInfo>> curVec, std::vector<std::shared_ptr<TangramElementInfo>> tempVec)
{
	std::map<std::shared_ptr<TangramElementInfo>, std::vector< std::shared_ptr<TangramElementInfo>>> pairVecMap;

	if (curVec.size() == 1 && tempVec.size() == 1)
	{
		std::shared_ptr<TangramElementInfo> spCurElement = curVec[0];
		std::shared_ptr<TangramElementInfo> spTempElement = tempVec[0];

		if (pairVecMap.find(spCurElement) == pairVecMap.end())
		{
			pairVecMap[spCurElement] = std::vector< std::shared_ptr<TangramElementInfo>>();
		}
		pairVecMap[spCurElement].emplace_back(spTempElement);
	}
	else if (curVec.size() == 1 && tempVec.size() == 2)
	{
		std::shared_ptr<TangramElementInfo> spCurElement = curVec[0];

		for (auto tempEleIter = tempVec.begin(); tempEleIter != tempVec.end(); tempEleIter++)
		{
			std::shared_ptr<TangramElementInfo> spTempElement = *tempEleIter;

			if (spCurElement->PoseEqual(spTempElement))
			{
				if (pairVecMap.find(spCurElement) == pairVecMap.end())
				{
					pairVecMap[spCurElement] = std::vector< std::shared_ptr<TangramElementInfo>>();
				}

				pairVecMap[spCurElement].emplace_back(spTempElement);
			}
		}
	}
	else if (curVec.size() == 2 && tempVec.size() == 2)
	{

		std::shared_ptr<TangramElementInfo> spTempElement0 = tempVec[0];
		std::shared_ptr<TangramElementInfo> spTempElement1 = tempVec[1];

		std::shared_ptr<TangramElementInfo> spCurElement0 = curVec[0];
		std::shared_ptr<TangramElementInfo> spCurElement1 = curVec[1];

		double RateTempDivCur = (spTempElement0->mPerimeter / spCurElement0->mPerimeter + spTempElement1->mPerimeter / spCurElement1->mPerimeter) / 2;

		cv::Point tempDist = spTempElement0->mCenterPt - spTempElement1->mCenterPt;
		cv::Point dist01 = spCurElement0->mCenterPt - spCurElement1->mCenterPt;
		cv::Point dist10 = spCurElement1->mCenterPt - spCurElement0->mCenterPt;
		if (abs(get_vector_angle(tempDist, dist01)) < abs(get_vector_angle(tempDist, dist10)))
		{
			if (pairVecMap.find(spCurElement0) == pairVecMap.end())
			{
				pairVecMap[spCurElement0] = std::vector< std::shared_ptr<TangramElementInfo>>();
			}

			pairVecMap[spCurElement0].emplace_back(spTempElement0);


			if (pairVecMap.find(spCurElement1) == pairVecMap.end())
			{
				pairVecMap[spCurElement1] = std::vector< std::shared_ptr<TangramElementInfo>>();
			}

			pairVecMap[spCurElement1].emplace_back(spTempElement1);

		}
		else
		{
			if (pairVecMap.find(spCurElement0) == pairVecMap.end())
			{
				pairVecMap[spCurElement0] = std::vector< std::shared_ptr<TangramElementInfo>>();
			}

			pairVecMap[spCurElement0].emplace_back(spTempElement1);


			if (pairVecMap.find(spCurElement1) == pairVecMap.end())
			{
				pairVecMap[spCurElement1] = std::vector< std::shared_ptr<TangramElementInfo>>();
			}

			pairVecMap[spCurElement1].emplace_back(spTempElement0);
		}

	}
	return pairVecMap;
}


void TangramDetector::DrawResult(std::vector<std::shared_ptr<TangramElementInfo>>& fitElementsVector)
{
	if(m_spCurBaseElement == nullptr)
	{
		return;
	}
	std::vector<std::vector<cv::Point>> contours;
	Scalar color(255, 0, 255);

	contours.clear();
	contours.emplace_back(m_spCurBaseElement->mContour);


	drawContours(m_CurrentDrawFrame, contours, 0, color, 2, LINE_AA);

	for (auto iter = fitElementsVector.begin(); iter != fitElementsVector.end(); iter++)
	{
		std::shared_ptr<TangramElementInfo> spCurInfo = *iter;

		contours.clear();
		contours.emplace_back(spCurInfo->mContour);

		Scalar color(255, 255, 0);
		drawContours(m_CurrentDrawFrame, contours, 0, color, 2, LINE_AA);


	}
}

bool TangramDetector::FindBaseWithEdge(cv::Mat hsvFrame, std::vector<std::shared_ptr<TangramElementInfo>>& fitElementsVector)
{
	//清空记录
	m_spCurrentGraph->ClearElement();
	m_spCurBaseElement = nullptr;
	m_spTemplateBaseElement = nullptr;


	//取形状轮廓
	auto contours = m_spCurrentGraph->FindTangramContours(hsvFrame);

	cv::Mat white = cv::Mat::zeros(hsvFrame.size(), CV_8UC1);
	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(white, contours, i, cv::Scalar(255), cv::FILLED);
	}
	imshow("while", white);

	//根据轮廓和颜色HSV，填入数据表
	m_spCurrentGraph->FillWithContours(contours, hsvFrame, m_HSVMap);



	float minAngleThresh = 5;
	float minCenterDist = 9999;


	double minDistToCenter = 9999;


	//查找基准块
	for (auto typeIter = m_TypeVector.begin(); typeIter != m_TypeVector.end(); typeIter++)
	{
		std::vector<std::shared_ptr<TangramElementInfo>> curVec = m_spCurrentGraph->FindInfosByType(*typeIter);
		std::vector<std::shared_ptr<TangramElementInfo>> tempVec = m_spTemplateGraph->FindInfosByType(*typeIter);

		auto typeMap = GetPairFromTypeVec(curVec, tempVec);

		for (auto iter = typeMap.begin(); iter != typeMap.end(); iter++)
		{
			std::shared_ptr<TangramElementInfo> spCur = iter->first;


			for (auto itertemp : iter->second)
			{
				std::shared_ptr<TangramElementInfo> spTemp = itertemp;

				if (spCur->PoseEqual(spTemp))
				{
					double dist = GetDist(spCur->mCenterPt, m_ScreenCenter);
					if (dist < minDistToCenter)
					{
						minDistToCenter = dist;
						m_spCurBaseElement = spCur;
						m_spTemplateBaseElement = spTemp;
					}
				}
			}

	
		}

	}

	if (m_spCurBaseElement != nullptr && m_spTemplateBaseElement != nullptr)
	{

		//比较形状，同形状可能有两块，要全比对一次
		for (auto typeIter = m_TypeVector.begin(); typeIter != m_TypeVector.end(); typeIter++)
		{
			std::map<std::shared_ptr<TangramElementInfo>, std::vector< std::shared_ptr<TangramElementInfo>>> typeVecMap;
			std::vector<std::shared_ptr<TangramElementInfo>> curVec;
			std::vector<std::shared_ptr<TangramElementInfo>> tempVec;

			if (m_spCurBaseElement->mType == *typeIter)
			{
				if (*typeIter == TangramElementInfo::TangramType::TT_MTRI ||
					*typeIter == TangramElementInfo::TangramType::TT_SQR ||
					*typeIter == TangramElementInfo::TangramType::TT_PARA)
				{
					continue;
				}
				else
				{
					auto curVecSrc = m_spCurrentGraph->FindInfosByType(*typeIter);
					auto tempVecSrc = m_spTemplateGraph->FindInfosByType(*typeIter);
					
					for (auto sp : curVecSrc)
					{
						if (sp != m_spCurBaseElement)
						{
							for (auto sptemp : tempVecSrc)
							{

								if (typeVecMap.find(sp) == typeVecMap.end())
								{
									typeVecMap[sp] = std::vector< std::shared_ptr<TangramElementInfo>>();
								}

								typeVecMap[sp].emplace_back(sptemp);
							}
						}
					}
				}
			}
			else
			{
				curVec = m_spCurrentGraph->FindInfosByType(*typeIter);
				tempVec = m_spTemplateGraph->FindInfosByType(*typeIter);
				typeVecMap = GetPairFromTypeVec(curVec, tempVec);
			}





			//typeVecMap 一个curInfo可能和一个或两个templateInfo比对，所以minCenterDist放在第一层for里边
			std::shared_ptr<TangramElementInfo> minDistCurInfo = nullptr;
			for (auto iter : typeVecMap)
			{
				float minCenterDist = 9999;
				auto spCurInfo = iter.first;

				for (auto spTemplateInfo : iter.second)
				{

					if (spCurInfo == nullptr || spTemplateInfo == nullptr)
					{
						continue;
					}

					if (spCurInfo != m_spCurBaseElement && spTemplateInfo != m_spTemplateBaseElement)
					{
						//摄相头中，当前块与基准块距离
						cv::Point centerCurDist;
						PiecesCenterDist(m_spCurBaseElement, spCurInfo, centerCurDist);


						//模板图中，当前块与基准块距离
						cv::Point centerTemplateDist;
						PiecesCenterDist(m_spTemplateBaseElement, spTemplateInfo, centerTemplateDist);

						//摄相头与模板图中，当前块的角度差
						double angDist;
						PiecesAngleDist(spCurInfo, spTemplateInfo, angDist);
						double threshAngle = 20;
						double threshCenter = 20;

						double TempDivCurRate = spTemplateInfo->mPerimeter / spCurInfo->mPerimeter;

						double centerDist = GetDist(centerCurDist * TempDivCurRate, centerTemplateDist);



						if (abs(angDist) < threshAngle && centerDist < threshCenter)
						{

							if (centerDist < minCenterDist)
							{
								minCenterDist = centerDist;
								minDistCurInfo = spCurInfo;
							}
						}

					}
				}


				if (minDistCurInfo != nullptr)
				{
					fitElementsVector.emplace_back(minDistCurInfo);
				}
			}
		}
	}




	return true;



}


bool TangramDetector::PiecesAngleDist(std::shared_ptr<TangramElementInfo> spBasePiece, std::shared_ptr<TangramElementInfo> spRelativePiece, double& dist)
{
	if (spBasePiece != nullptr &&
		spRelativePiece != nullptr)
	{
		dist = spBasePiece->GetAngle() - spRelativePiece->GetAngle();

		return true;
	}
	else
	{
		return false;
	}
}

bool TangramDetector::PiecesCenterDist(std::shared_ptr<TangramElementInfo> spBasePiece, std::shared_ptr<TangramElementInfo> spRelativePiece, cv::Point& dist)
{
	if (spBasePiece != nullptr &&
		spRelativePiece != nullptr)
	{
		dist = spBasePiece->mCenterPt - spRelativePiece->mCenterPt;

		return true;
	}
	else
	{
		return false;
	}
}

void TangramDetector::Update(cv::Mat& frame)
{
	m_ScreenCenter = cv::Point(frame.cols / 2, frame.rows / 2);


	m_CurrentDrawFrame = frame.clone();


	cv::Mat hsv;
	cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

	std::vector<std::shared_ptr<TangramElementInfo>> fitElementsVector;
	if (FindBaseWithEdge(hsv, fitElementsVector))
	{
		DrawResult(fitElementsVector);
	}

	cv::imshow("Draw", m_CurrentDrawFrame);
	//m_spStateMachine->updateWithDeltaTime();
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

	//cv::Mat src = cv::imread("Parallel.jpg");
	//cv::Mat src = cv::imread("Triangle.jpg");
	cv::Mat src = cv::imread("Square.jpg");
	cv::Mat gray;
	cv::cvtColor(src, gray, COLOR_BGR2GRAY);
	cv::threshold(gray, gray, 200, 255, THRESH_BINARY_INV);

	int step = 5;
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
		//TriangleElementInfo tri;
		SquareElementInfo sqrare;
		sqrare.Init(approxes, TangramElementInfo::TangramTypeName::TTN_PARA);

		printf("\n %f", sqrare.GetAngle());

		cv::imshow("img_rot", img_rot);
		cv::waitKey();
	}


	/*int step = 1;
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
		parallel.Init(approxes,TangramElementInfo::TangramTypeName::TTN_PARA);

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
		parallel.Init(approxes,TangramElementInfo::TangramTypeName::TTN_PARA);

		printf("\n %d %f", parallel.IsFlip(), parallel.GetAngle());
	}*/


}

cv::Mat FixWidthResize(cv::Mat src,int fixedWidth)
{
	cv::Mat ret;
	int fixedHeight = fixedWidth * src.rows / src.cols;
	cv::resize(src, ret, cv::Size(fixedWidth, fixedHeight));
	return ret;
}

void TangramDetection()
{
	cv::Mat templateFrame = cv::imread("ECat.jpg");
	templateFrame = FixWidthResize(templateFrame, 512);



	cv::Mat testImage = cv::imread("ECat.jpg");
	testImage = FixWidthResize(testImage, 512);


	auto cap = cv::VideoCapture(0);
	TangramDetector detector;

	detector.Init(templateFrame);


	bool quit = false;
	cv::Mat frame;

	int frameCount = 0;
	while (cap.isOpened()) {

		//real camera
		bool ret = cap.read(frame);

		//test image
		//frame = testImage.clone();
		//bool ret = true;

		if (ret)
		{
			detector.Update(frame);

			cv::imshow("frame", frame);
			if (cv::waitKey(1) == 27)
			{
				break;
			}
		}
	}
}