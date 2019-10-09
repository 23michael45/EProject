#ifndef TangramData_h__
#define TangramData_h__

#include "Tangram/TangramUtility.h"


class TangramElementInfo
{
public:
	enum TangramTypeName
	{
		TTN_STRI_1,
		TTN_STRI_2,
		TTN_MTRI,
		TTN_LTRI_1,
		TTN_LTRI_2,
		TTN_SQR,
		TTN_PARA,
		TTN_NONE,
	};
	enum TangramType
	{
		TT_STRI,
		TT_MTRI,
		TT_LTRI,
		TT_SQR,
		TT_PARA,
	};
	TangramElementInfo();

	static TangramType Name2Type(TangramTypeName tName)
	{
		switch (tName)
		{
		case TangramElementInfo::TTN_STRI_1:
			return TangramType::TT_STRI;
		case TangramElementInfo::TTN_STRI_2:
			return TangramType::TT_STRI;
		case TangramElementInfo::TTN_MTRI:
			return TangramType::TT_MTRI;
		case TangramElementInfo::TTN_LTRI_1:
			return TangramType::TT_LTRI;
		case TangramElementInfo::TTN_LTRI_2:
			return TangramType::TT_LTRI;
		case TangramElementInfo::TTN_SQR:
			return TangramType::TT_SQR;
		case TangramElementInfo::TTN_PARA:
			return TangramType::TT_PARA;
		default:
			break;
		}
	}


	virtual void Init(std::vector<cv::Point> contour,TangramTypeName tName);

	bool PoseEqual(std::shared_ptr<TangramElementInfo> spInfo);
	bool ShapeEqual(std::shared_ptr<TangramElementInfo> spInfo);
	bool DistEqual(std::shared_ptr<TangramElementInfo> spBaseInfo);

	virtual double GetAngle();
	std::vector<cv::Point> mContour;
	double mArea;
	double mPerimeter;
	cv::Point mCenterPt;
	cv::RotatedRect mBoundingRect;


	TangramType mType;
	TangramTypeName mTypeName;

};

class TriangleElementInfo :public  TangramElementInfo
{
public:
	TriangleElementInfo();
	virtual double GetAngle() override;
};
class SquareElementInfo : public TangramElementInfo
{
public:
	SquareElementInfo();
	virtual double GetAngle() override;
};
class ParallelElementInfo : public TangramElementInfo
{
public:
	ParallelElementInfo();
	virtual double GetAngle() override;

	// 以pt1为基准
	double getAngelOfTwoVector(cv::Point2f &pt1, cv::Point2f &pt2, cv::Point2f &c);
	double Magnitude(cv::Point vec);

	bool IsFlip();
};




class TangramGraph
{
	friend class TangramDetector;
public:	 
	TangramGraph(std::vector<TangramElementInfo::TangramTypeName> typeNameVector);
	~TangramGraph();


	bool InitAsTemplate(cv::Mat hsvFrame, std::map<TangramElementInfo::TangramTypeName, cv::Vec3b> &HSVMap);

	void ClearElement();

	std::vector<std::vector<cv::Point>> FindTangramContours(cv::Mat hsvFrame);
	void FillWithContours(std::vector<std::vector<cv::Point>> contours, cv::Mat hsvFrame,std::map<TangramElementInfo::TangramTypeName, cv::Vec3b> HSVMap);



	void DetectPieces(cv::Mat frame,std::map<TangramElementInfo::TangramTypeName, cv::Vec3b> HSVMap);

	bool PiecesCenterDistByName(TangramElementInfo::TangramTypeName basePieceName, TangramElementInfo::TangramTypeName relativePieceName, cv::Point& dist);
	std::vector<std::shared_ptr<TangramElementInfo>> FindInfosByType(TangramElementInfo::TangramType type);

private:
	void FillPieceData(std::vector<cv::Point> approxes, TangramElementInfo::TangramTypeName typeName);


	std::map<TangramElementInfo::TangramTypeName, std::shared_ptr<TangramElementInfo>> m_ElementsMap;
};


#endif // TangramData_h__
