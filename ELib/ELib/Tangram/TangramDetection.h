#ifndef TangramDetection_h__
#define TangramDetection_h__

#include "Utility/Interface.h"
#include "Tangram/TangramData.h"
#include "Utility/FSM/FSM.h"
#include "Tangram/TangramStates.h"
#include <vector>
class TangramDetector : public std::enable_shared_from_this<TangramDetector>
{

	friend class WithBasePieceState;
	friend class NoBasePieceState;
public:
	TangramDetector() {};


	bool Init(cv::Mat templateFrame);

	void Update(cv::Mat& frame);
	void Test();
private:
	void DrawResult(std::vector<std::shared_ptr<TangramElementInfo>>& fitElementsVector);

	bool FindBaseWithEdge(cv::Mat hsvFrame, std::vector<std::shared_ptr<TangramElementInfo>>& fitElementsVector);


	bool PiecesAngleDist(std::shared_ptr<TangramElementInfo> spBasePiece, std::shared_ptr<TangramElementInfo> spRelativePiece, double& dist);
	bool PiecesCenterDist(std::shared_ptr<TangramElementInfo> spBasePiece, std::shared_ptr<TangramElementInfo> spRelativePiece, cv::Point& dist);



	std::shared_ptr<StateMachine<TangramDetector>> m_spStateMachine;

	std::shared_ptr<TangramElementInfo> m_spCurBaseElement;
	std::shared_ptr<TangramElementInfo> m_spTemplateBaseElement;


	std::shared_ptr<TangramGraph> m_spCurrentGraph;
	std::shared_ptr<TangramGraph> m_spTemplateGraph;

	double m_TemplateDivideCurRate;
	cv::Point m_ScreenCenter;


	std::map<TangramElementInfo::TangramTypeName, cv::Vec3b> m_HSVMap;
	std::vector<TangramElementInfo::TangramType> m_TypeVector;
	std::vector<TangramElementInfo::TangramTypeName> m_TypeNameVector;

	cv::Mat m_CurrentDrawFrame;
};




CPP_INTERFACE_API void TangramDetection();
#endif // TangramDetection_h__
