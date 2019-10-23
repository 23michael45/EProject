#include "TangramDetection.h"



using namespace cv;




TangramDetector::TangramDetector()
{

	m_TypeVector.clear();
	m_TypeVector.emplace_back(TangramElementInfo::TangramType::TT_STRI);
	m_TypeVector.emplace_back(TangramElementInfo::TangramType::TT_MTRI);
	m_TypeVector.emplace_back(TangramElementInfo::TangramType::TT_LTRI);
	m_TypeVector.emplace_back(TangramElementInfo::TangramType::TT_SQR);
	m_TypeVector.emplace_back(TangramElementInfo::TangramType::TT_PARA);

	m_TypeNameVector.clear();
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_STRI_1);
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_STRI_2);
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_MTRI);
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_LTRI_1);
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_LTRI_2);
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_SQR);
	m_TypeNameVector.emplace_back(TangramElementInfo::TangramTypeName::TTN_PARA);
}

CPP_INTERFACE_API TangramDetector::~TangramDetector()
{
	//cv::destroyWindow("white");
}

bool TangramDetector::SetTemplateGraph(cv::Mat templateFrame)
{
	m_spTemplateGraph = std::make_shared<TangramGraph>(m_TypeNameVector);
	
	cv::Mat hsv;
	cv::cvtColor(templateFrame, hsv, cv::COLOR_BGR2HSV);

	m_ThreshAngle = 15;
	double divideThresh = 24;
	m_ThreshCenterDist = (double)max(templateFrame.cols, templateFrame.rows) / divideThresh;

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
	if (m_spCurrentGraph == nullptr)
	{
		return false;
	}
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
	//imshow("white", white);
	//cv::waitKey(1);

	//根据轮廓和颜色HSV，填入数据表
	m_spCurrentGraph->FillWithContours(contours, hsvFrame, m_HSVMap);


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

		//遍历所有类型(5种形状）,比较形状，同形状可能有两块，要全比对一次
		for (auto typeIter = m_TypeVector.begin(); typeIter != m_TypeVector.end(); typeIter++)
		{
			std::map<std::shared_ptr<TangramElementInfo>, std::vector< std::shared_ptr<TangramElementInfo>>> typeVecMap;
			std::vector<std::shared_ptr<TangramElementInfo>> curVec;
			std::vector<std::shared_ptr<TangramElementInfo>> tempVec;

			

			//当前遍历形状 与 基础块 相同
			if (m_spCurBaseElement->mType == *typeIter)
			{
				//这三种块只有一块，所以PASS
				if (*typeIter == TangramElementInfo::TangramType::TT_MTRI ||
					*typeIter == TangramElementInfo::TangramType::TT_SQR ||
					*typeIter == TangramElementInfo::TangramType::TT_PARA)
				{
					continue;
				}
				//其它两种块，每种有不同颜色的两块
				else
				{
					auto curVecSrc = m_spCurrentGraph->FindInfosByType(*typeIter);
					auto tempVecSrc = m_spTemplateGraph->FindInfosByType(*typeIter);
					
					//遍历当前帧检测到的块，取出的MAP是 当前块->要比较的模板块数组   可以一个当前块，要与1个或2个模板块比较
					for (auto sp : curVecSrc)
					{
						//如果不是基准块
						if (sp != m_spCurBaseElement)
						{
							
							//遍历模板图中的块
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
			//如果当前块与基础块不同
			else
			{
				curVec = m_spCurrentGraph->FindInfosByType(*typeIter);
				tempVec = m_spTemplateGraph->FindInfosByType(*typeIter);
				typeVecMap = GetPairFromTypeVec(curVec, tempVec);
			}

			//此处得到的typeVecMap  ，是当前块（可能是基础块或非基础块） 与 要比较的模板块数据   对应表



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


						double TempDivCurRate = spTemplateInfo->mPerimeter / spCurInfo->mPerimeter;

						double centerDist = GetDist(centerCurDist * TempDivCurRate, centerTemplateDist);


						//如果是小三角形，阈值要放大，不然不好识别
						double angRate = 1.0f;
						double distRate = 1.0f;
						if (spCurInfo->mType == TangramElementInfo::TangramType::TT_STRI)
						{
							angRate = 1.5f;
							distRate = 2.0f;
						}


						if (abs(angDist) < m_ThreshAngle * angRate && centerDist < m_ThreshCenterDist * distRate)
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

	//m_spStateMachine->updateWithDeltaTime();
}

CPP_INTERFACE_API bool TangramDetector::GetDrawData(char* &buffer, int &width, int &height,int &channel)
{
	if (m_CurrentDrawFrame.rows > 0 && m_CurrentDrawFrame.cols > 0)
	{
		buffer = (char*)m_CurrentDrawFrame.data;
		width = m_CurrentDrawFrame.cols;
		height = m_CurrentDrawFrame.rows;
		channel = m_CurrentDrawFrame.channels();
		return true;


	}
	return false;
}

CPP_INTERFACE_API cv::Mat& TangramDetector::GetDrawData()
{
	return m_CurrentDrawFrame;
}

//----------------------------------------------------------------------------------------------


