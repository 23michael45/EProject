#ifndef UnityNativeLoader_h__
#define UnityNativeLoader_h__

#include "Unity/IUnityInterface.h"
#include "Unity/IUnityGraphics.h"
#ifdef __cplusplus
extern "C" {
#endif

	//call from unity mannually
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityNativeLoader();
	// If exported by a plugin, this function will be called when the plugin is loaded.call from unity automatically
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces);
	// If exported by a plugin, this function will be called when the plugin is about to be unloaded.call from unity automatically
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload();

	//call from c++ mannually
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetUnityInterfaces(IUnityInterfaces* &pUnityInterfaces);



#ifdef __cplusplus
}
#endif

#endif // UnityNativeLoader_h__
