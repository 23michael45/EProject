#ifndef UnityInterface_h__
#define UnityInterface_h__

#include "Interface.h"
#include "NativeGraphics/NativeGraphics.h"
extern "C" {
	UNITY_EXPORT_INTERFACE_API char* CreateELib();

	UNITY_EXPORT_INTERFACE_API void DestroyELib(char* handle);

	UNITY_EXPORT_INTERFACE_API UnityRenderingEvent GetNativeRenderingEvent(char* handle, char* name);

	UNITY_EXPORT_INTERFACE_API void SetTemplateGraph(char* handle, char* texData, int width, int height);

	UNITY_EXPORT_INTERFACE_API void Feed(char* handle, char* texData, int width, int height);
	UNITY_EXPORT_INTERFACE_API char* FeedNativeTexture(char* handle, void* textureHandle, int width, int height);

	UNITY_EXPORT_INTERFACE_API char* SetPaintedTexture(char* handle, char* textureHandle, int width, int height);
}

#endif // UnityInterface_h__
