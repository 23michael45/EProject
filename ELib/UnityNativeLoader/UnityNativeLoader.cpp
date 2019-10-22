#include "UnityNativeLoader.h"
#include "PlatformBase.h"

#include <assert.h>
#include <math.h>
#include <vector>


static IUnityInterfaces* s_UnityInterfaces = NULL;


extern "C" {


	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityNativeLoader()
	{
		//Call to Load Dll in Unity
	}

	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
	{
		s_UnityInterfaces = unityInterfaces;
	}

	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload()
	{
	}


	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetUnityInterfaces(IUnityInterfaces* &pUnityInterfaces)
	{
		pUnityInterfaces = s_UnityInterfaces;
	}


}

