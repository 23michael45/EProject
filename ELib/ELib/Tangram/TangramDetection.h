#ifndef TangramDetection_h__
#define TangramDetection_h__

#include "Utility/Interface.h"
#include "Tangram/TangramData.h"
#include "Utility/FSM/FSM.h"
#include "Tangram/TangramStates.h"
class TangramDetector : public std::enable_shared_from_this<TangramDetector>
{
public:
	TangramDetector();


	bool Init();

	void Update(cv::Mat& frame);
	void Test();
private:
	void DetectPieces();
	void FindBase();;
	void FindRelative() {};


	cv::Vec3b HSV_U2CV(cv::Vec3b c);
	double GetColorDist(cv::Vec3b c1, cv::Vec3b c2);

	cv::Mat GetHsvMask(cv::Mat src, cv::Vec3b color, double dist);

	std::vector<cv::Point> FindLargestContour(cv::Mat src);

	std::map<std::string, cv::Vec3b> HSVPiecesMap;


	std::shared_ptr<StateMachine<TangramDetector>> m_spStateMachine;

	std::shared_ptr<TangramElementInfo> m_spBaseElement;

	cv::Mat &m_curFrame;


	std::map<TangramElementInfo::TangrameType , std::vector<std::shared_ptr<TangramElementInfo>>> m_Pieces;
};




CPP_INTERFACE_API void TangramDetection();
#endif // TangramDetection_h__
