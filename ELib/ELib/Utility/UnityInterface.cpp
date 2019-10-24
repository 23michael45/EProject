#include "UnityInterface.h"
#include "Tangram/TangramDetection.h"
#include <opencv2/opencv.hpp>
#include "NativeGraphics/NativeGraphics.h"
#include <mutex>
#include <chrono>
#include <thread>
#include "MD5.h"
#if _WINDOWS
#include <Windows.h>
#endif

void InitGraphicEvent(int id)
{
	std::lock_guard<std::mutex> lock(GetNativeRenderingEventMutex());
	InitNativeGraphics();

}

void FeedEvent(int id)
{
	std::lock_guard<std::mutex> lock(GetNativeRenderingEventMutex());
	std::string name = "FeedTexture";

	std::shared_ptr<NativeRenderEvent> spEvent;
	std::shared_ptr<NativeTexture> spNativeTexture;

	if (GetNativeRenderingEvent(name, spEvent) && GetNativeTexture(name, spNativeTexture))
	{
		TangramDetector* pTD = static_cast<TangramDetector*>(spEvent->param);
		if (pTD != nullptr)
		{
			cv::Mat bgr;

			if (spNativeTexture->mChannel == 4)
			{
				cv::Mat textureUnity(spNativeTexture->mHeight, spNativeTexture->mWidth, CV_8UC4, spNativeTexture->mDataPtr);
				cv::Mat rgba;
				cv::flip(textureUnity, rgba, 0);
				cv::cvtColor(rgba, bgr, cv::COLOR_RGBA2BGR);
			}
			else if (spNativeTexture->mChannel == 3)
			{
				cv::Mat textureUnity(spNativeTexture->mHeight, spNativeTexture->mWidth, CV_8UC3, spNativeTexture->mDataPtr);
				cv::Mat rgb;
				cv::flip(textureUnity, rgb, 0);
				cv::cvtColor(rgb, bgr, cv::COLOR_RGB2BGR);
			}


			pTD->Update(bgr);


			cv::Mat& drawMat = pTD->GetDrawData();
			
			std::shared_ptr<NativeTexture> spPaintedTex;
			GetNativeTexture("PaintedTexture", spPaintedTex);
			if (spPaintedTex)
			{
				if (spPaintedTex->mChannel == 4)
				{
					cv::Mat drawRgba;
					cv::cvtColor(drawMat, drawRgba, cv::COLOR_BGR2RGBA);
					cv::flip(drawRgba, drawRgba, 0);
					ModifyNativeTexture("PaintedTexture", drawRgba.data);

				}
				else if (spPaintedTex->mChannel ==3)
				{
					cv::Mat drawRgb;
					cv::cvtColor(drawMat, drawRgb, cv::COLOR_BGR2RGB);
					cv::flip(drawRgb, drawRgb, 0);
					ModifyNativeTexture("PaintedTexture", drawRgb.data);
				}
			}


			//cv::Mat draw(height,width,CV_8UC3,buffer);
			//cv::imshow("draw", draw);
			//cv::waitKey(1);
		}
	}
}
extern "C" {
	UNITY_INTERFACE_EXPORT char* CreateELib()
	{
		auto tid = std::this_thread::get_id();

		AddNativeRenderingEvent("InitGraphicEvent", InitGraphicEvent, nullptr);

		TangramDetector* pTD = new TangramDetector();
		return (char*)pTD;
	}

	UNITY_INTERFACE_EXPORT void DestroyELib(char* handle)
	{
		TangramDetector* pTD = (TangramDetector*)handle;
		if (pTD)
		{
			delete pTD;
			printf("Delete TangramDetector");
		}

		UninitNativeGraphics();
	}

	UNITY_INTERFACE_EXPORT UnityRenderingEvent GetNativeRenderingEvent(char* handle, char* name)
	{
		TangramDetector* pTD = (TangramDetector*)handle;
		if (pTD)
		{
			std::shared_ptr<NativeRenderEvent> spEvent;
			GetNativeRenderingEvent(name, spEvent);

			return spEvent->func;

		}
		return nullptr;
	}

	UNITY_INTERFACE_EXPORT void Feed(char* handle, char* texData, int width, int height, int channel)
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

	UNITY_INTERFACE_EXPORT char* FeedNativeTexture(char* handle, void* textureHandle, int width, int height, int channel)
	{
		auto tid = std::this_thread::get_id();

		TangramDetector* pTD = (TangramDetector*)handle;
		if (pTD && textureHandle)
		{
			int len = 32;
#if _WINDOWS
			char* eventName = (char*)CoTaskMemAlloc(len);
#else

			char* eventName = new char(len);
#endif
			strcpy(eventName, "FeedTexture");

			AddNativeRenderingEvent(eventName, FeedEvent, pTD);
			AddNativeTexture(eventName, textureHandle, width, height, channel);

			return eventName;
		}
		return nullptr;
	}

	UNITY_INTERFACE_EXPORT void SetTemplateGraph(char* handle, char* texData, int width, int height, int channel)
	{
		TangramDetector* pTD = (TangramDetector*)handle;

#if ELIB_DEMO
		std::string md5 = md5sum(texData, width*height*channel);
		if (md5 != "c4ee27528c00b7fb1736472fcf24698c")
		{
			exit(-1);
		}
#endif

		if (pTD && texData)
		{
			cv::Mat bgr;
			if (channel == 4)
			{
				cv::Mat textureUnity(height, width, CV_8UC4, texData);
				cv::Mat rgba;
				cv::flip(textureUnity, rgba, 0);
				cv::cvtColor(rgba, bgr, cv::COLOR_RGBA2BGR);
			}
			else if (channel == 3)
			{
				cv::Mat textureUnity(height, width, CV_8UC3, texData);
				cv::Mat rgb;
				cv::flip(textureUnity, rgb, 0);
				cv::cvtColor(rgb, bgr, cv::COLOR_RGB2BGR);
			}
			//cv::imshow("bgr", bgr);
			//cv::waitKey();

			pTD->SetTemplateGraph(bgr);
		}
	}

	UNITY_INTERFACE_EXPORT char* SetPaintedTexture(char* handle, char* textureHandle, int width, int height, int channel)
	{
		TangramDetector* pTD = (TangramDetector*)handle;
		if (pTD && textureHandle)
		{
			int len = 32;
#if _WINDOWS
			char* eventName = (char*)CoTaskMemAlloc(len);
#else

			char* eventName = new char(len);
#endif
			strcpy(eventName, "PaintedTexture");

			AddNativeTexture(eventName, textureHandle, width, height, channel);

			return eventName;
		}
		return nullptr;
	}


	UNITY_INTERFACE_EXPORT void GetFittedElements(char* handle, char*& pElementArray, int &len)
	{
		TangramDetector* pTD = (TangramDetector*)handle;
		if (pTD)
		{
			auto fittedElements = pTD->GetFittedElements();
			
			len = fittedElements.size();

		}

	}

}