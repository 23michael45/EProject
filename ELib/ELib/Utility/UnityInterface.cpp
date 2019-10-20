#include "UnityInterface.h"
#include "Tangram/TangramDetection.h"
#include <opencv2/opencv.hpp>


UNITY_EXPORT_INTERFACE_API char* CreateELib()
{
	TangramDetector* pTD = new TangramDetector();
	return (char*)pTD;
}

UNITY_EXPORT_INTERFACE_API void DestroyELib(char* handle)
{
	TangramDetector* pTD = (TangramDetector*)handle;
	if (pTD)
	{
		delete pTD;
		printf("Delete TangramDetector");
	}
}

UNITY_EXPORT_INTERFACE_API void Feed(char* handle, char* texData,int width,int height)
{
	TangramDetector* pTD = (TangramDetector*)handle;
	if (pTD)
	{
		cv::Mat textureUnity(height, width, CV_8UC4, texData);
		cv::Mat rgba;
		cv::flip(textureUnity, rgba, 0);

		cv::Mat bgr;
		cv::cvtColor(rgba, bgr, cv::COLOR_RGBA2BGR);

		pTD->Update(bgr);
	}
}

UNITY_EXPORT_INTERFACE_API void SetTemplateGraph(char* handle, char* texData, int width, int height)
{
	TangramDetector* pTD = (TangramDetector*)handle;
	if (pTD)
	{
		cv::Mat textureUnity(height, width, CV_8UC4, texData);
		cv::Mat rgba;
		cv::flip(textureUnity, rgba, 0);
		cv::Mat bgr;
		cv::cvtColor(rgba, bgr, cv::COLOR_RGBA2BGR);
		pTD->SetTemplateGraph(bgr);
	}
}

UNITY_EXPORT_INTERFACE_API bool GetPaintedTexture(char* handle, char* &texData, int &width, int &height)
{
	TangramDetector* pTD = (TangramDetector*)handle;
	if (pTD)
	{
		return pTD->GetDrawData(texData, width, height);
	}

}
