#ifndef RenderingPlugin_h__
#define RenderingPlugin_h__

#include "Unity/IUnityInterface.h"
#include "Unity/IUnityGraphics.h"
#ifdef __cplusplus
extern "C" {
#endif

	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API StartRenderPlugin();
	// If exported by a plugin, this function will be called when the plugin is loaded.
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces);
	// If exported by a plugin, this function will be called when the plugin is about to be unloaded.
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload();


	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetRenderEventFunc(UnityRenderingEvent func);
	UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetRenderEventFunc();
	
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API BeginModifyTexture(void* textureHandle, int textureWidth, int textureHeight, int channel,void* &dataPtr, int* outRowPitch);
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API EndModifyTexture(void* textureHandle, int textureWidth, int textureHeight, int channel, int rowPitch, void* dataPtr);

	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API BeginModifyVertexBuffer(void* bufferHandle, void* &dataPtr,size_t* outBufferSize);
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API EndModifyVertexBuffer(void* bufferHandle);

	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetTextureBuffer(void* bufferHandle,int textureWidth, int textureHeight, int channel,void* dataPtr);
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetVertextBuffer(void* bufferHandle, void* &dataPtr, int& len);




#ifdef __cplusplus
}
#endif
#endif // RenderingPlugin_h__
