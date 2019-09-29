#ifndef TangramData_h__
#define TangramData_h__

#include "Tangram/TangramUtility.h"


class TangramElementInfo
{
public:
	enum TangrameType
	{
		TT_STRI,
		TT_MTRI,
		TT_LTRI,
		TT_SQR,
		TT_PARA,
	};
	TangramElementInfo();

	virtual void Init(std::vector<cv::Point> contour);

	bool Equal(std::shared_ptr<TangramElementInfo> spInfo);

	virtual double GetAngle();
	std::vector<cv::Point> mContour;
	double mArea;
	double mPerimeter;
	cv::Point mCenterPt;
	cv::RotatedRect mBoundingRect;


	TangrameType mType;

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
#endif // TangramData_h__
