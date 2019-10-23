#ifndef UnityInterface_h__
#define UnityInterface_h__

#include "Unity/IUnityInterface.h"
#include "Unity/IUnityGraphics.h"
#include "NativeGraphics/NativeGraphics.h"
extern "C" {
	UNITY_INTERFACE_EXPORT  char* CreateELib();

	UNITY_INTERFACE_EXPORT  void DestroyELib(char* handle);

	UNITY_INTERFACE_EXPORT  UnityRenderingEvent GetNativeRenderingEvent(char* handle, char* name);

	UNITY_INTERFACE_EXPORT  void SetTemplateGraph(char* handle, char* texData, int width, int height, int channel);

	UNITY_INTERFACE_EXPORT  void Feed(char* handle, char* texData, int width, int height, int channel);
	UNITY_INTERFACE_EXPORT  char* FeedNativeTexture(char* handle, void* textureHandle, int width, int height, int channel);

	UNITY_INTERFACE_EXPORT  char* SetPaintedTexture(char* handle, char* textureHandle, int width, int height, int channel);
}

#endif // UnityInterface_h__
