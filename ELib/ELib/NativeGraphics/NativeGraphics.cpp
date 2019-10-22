#include "NativeGraphics.h"
#include "PlatformBase.h"
#include "RenderAPI.h"

#include <assert.h>
#include <math.h>

NativeTexture::NativeTexture(void* handle, int width, int height, int channel)
{
	mHandle = handle;
	mWidth = width;
	mHeight = height;
	mChannel = channel;

	mDataPtr = new char[width * height * channel];
}

NativeTexture::~NativeTexture()
{
	delete[] mDataPtr;
}



std::map<std::string, NativeMesh> gNativeMeshMap;
std::map<std::string, std::shared_ptr<NativeTexture>> gNativeTextureMap;
std::map<std::string, std::shared_ptr<NativeRenderEvent>> gNativeRenderingEventMap;

std::mutex gMeshMapMutex;
std::mutex gTextureMapMutex;
std::mutex gEventMapMutex;
std::mutex gEventRenderingMutex;

IUnityInterfaces* g_UnityInterfaces = NULL;
IUnityGraphics* g_Graphics = NULL;
static RenderAPI* g_CurrentAPI = NULL;
static UnityGfxRenderer g_DeviceType = kUnityGfxRendererNull;


static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
	// Create graphics API implementation upon initialization
	if (eventType == kUnityGfxDeviceEventInitialize)
	{
		assert(g_CurrentAPI == NULL);
		g_DeviceType = g_Graphics->GetRenderer();
		g_CurrentAPI = CreateRenderAPI(g_DeviceType);
	}

	// Let the implementation process the device related events
	if (g_CurrentAPI)
	{
		g_CurrentAPI->ProcessDeviceEvent(eventType, g_UnityInterfaces);
	}

	// Cleanup graphics API implementation upon shutdown
	if (eventType == kUnityGfxDeviceEventShutdown)
	{
		delete g_CurrentAPI;
		g_CurrentAPI = NULL;
		g_DeviceType = kUnityGfxRendererNull;
	}
}

void InitNativeGraphics()
{
	GetUnityInterfaces(g_UnityInterfaces);

	g_Graphics = g_UnityInterfaces->Get<IUnityGraphics>();
	g_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);

#if SUPPORT_VULKAN
	if (s_Graphics->GetRenderer() == kUnityGfxRendererNull)
	{
		extern void RenderAPI_Vulkan_OnPluginLoad(IUnityInterfaces*);
		RenderAPI_Vulkan_OnPluginLoad(unityInterfaces);
	}
#endif // SUPPORT_VULKAN

	// Run OnGraphicsDeviceEvent(initialize) manually on plugin load
	OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);

}
void UninitNativeGraphics()
{
	g_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);

	std::lock_guard<std::mutex> lock(GetNativeRenderingEventMutex());

	gEventMapMutex.lock();
	gNativeRenderingEventMap.clear(); 
	gEventMapMutex.unlock();


	gTextureMapMutex.lock();
	gNativeTextureMap.clear();
	gTextureMapMutex.unlock();


	gMeshMapMutex.lock();
	gNativeMeshMap.clear();
	gMeshMapMutex.unlock();

}

bool AddNativeRenderingEvent(std::string name, UnityRenderingEvent func,void* param)
{
	std::lock_guard<std::mutex> lock(gEventMapMutex);
	if (gNativeRenderingEventMap.find(name) == gNativeRenderingEventMap.end())
	{
		std::shared_ptr<NativeRenderEvent> spEvent = std::make_shared<NativeRenderEvent>();
		spEvent->func = func;
		spEvent->param = param;
		gNativeRenderingEventMap[name] = spEvent;

		return true;
	}
	else
	{
		return false;
	}
}
bool RemoveNativeRenderingEvent(std::string name)
{
	std::lock_guard<std::mutex> lock(gEventMapMutex);
	if (gNativeRenderingEventMap.find(name) != gNativeRenderingEventMap.end())
	{
		gNativeRenderingEventMap.erase(name);
		return true;
	}
	else
	{
		return false;
	}
}
bool GetNativeRenderingEvent(std::string name, std::shared_ptr<NativeRenderEvent>& spEvent) {

	std::lock_guard<std::mutex> lock(gEventMapMutex);
	if (gNativeRenderingEventMap.find(name) != gNativeRenderingEventMap.end())
	{		spEvent = gNativeRenderingEventMap[name];

		return true;
	}
	return false;
}

std::mutex& GetNativeRenderingEventMutex()
{
	return gEventRenderingMutex;
}

bool AddNativeTexture(std::string name, void* handle, int w, int h, int c)
{
	std::lock_guard<std::mutex> lock(gTextureMapMutex);
	if (gNativeTextureMap.find(name) == gNativeTextureMap.end())
	{
		std::shared_ptr<NativeTexture> spTex = std::make_shared<NativeTexture>(handle, w, h, c);
		gNativeTextureMap[name] = spTex;
		return true;
	}
	else
	{
		return false;
	}
}
bool RemoveNativeTexture(std::string name)
{
	std::lock_guard<std::mutex> lock(gTextureMapMutex);
	if (gNativeTextureMap.find(name) != gNativeTextureMap.end())
	{
		gNativeTextureMap.erase(name);
		return true;
	}
	else
	{
		return false;
	}
}
bool ModifyNativeTexture(std::string name, void* pData)
{
	std::lock_guard<std::mutex> lock(gTextureMapMutex);
	if (gNativeTextureMap.find(name) != gNativeTextureMap.end())
	{
		auto spTex = gNativeTextureMap[name];

		int len = spTex->mWidth * spTex->mHeight * spTex->mChannel;
		memcpy(spTex->mDataPtr, pData, len);


		int outRowPitch = spTex->mWidth * spTex->mChannel;

		g_CurrentAPI->ModifyTexture(spTex->mHandle, spTex->mWidth, spTex->mHeight, spTex->mChannel, outRowPitch, spTex->mDataPtr);


		return true;
	}
	return false;
}
bool GetNativeTexture(std::string name, std::shared_ptr<NativeTexture>& spTexture) 
{
	std::lock_guard<std::mutex> lock(gTextureMapMutex);
	if (gNativeTextureMap.find(name) != gNativeTextureMap.end())
	{
		auto spTex = gNativeTextureMap[name];

		g_CurrentAPI->GetTextureBuffer(spTex->mHandle, spTex->mWidth, spTex->mHeight, spTex->mChannel, spTex->mDataPtr);

		spTexture = spTex;
		return true;
	}
	return false;

}


bool AddNativeMesh(std::string name, void* vertexBufferHandle, int vertexCount, float* sourceVertices, float* sourceNormals, float* sourceUV)
{
	std::lock_guard<std::mutex> lock(gMeshMapMutex);
	if (gNativeMeshMap.find(name) == gNativeMeshMap.end())
	{
		std::shared_ptr<NativeMesh> spMesh = std::make_shared<NativeMesh>();

		spMesh->handle = vertexBufferHandle;
		spMesh->vertices.resize(vertexCount);

		for (int i = 0; i < vertexCount; ++i)
		{
			NativeMeshVertex& v = spMesh->vertices[i];
			v.pos[0] = sourceVertices[0];
			v.pos[1] = sourceVertices[1];
			v.pos[2] = sourceVertices[2];
			v.normal[0] = sourceNormals[0];
			v.normal[1] = sourceNormals[1];
			v.normal[2] = sourceNormals[2];
			v.uv[0] = sourceUV[0];
			v.uv[1] = sourceUV[1];
			sourceVertices += 3;
			sourceNormals += 3;
			sourceUV += 2;
		}
		return true;
	}
	else
	{
		return false;
	}
}
bool RemoveNativeMesh(std::string name)
{
	std::lock_guard<std::mutex> lock(gMeshMapMutex);
	if (gNativeMeshMap.find(name) != gNativeMeshMap.end())
	{
		gNativeMeshMap.erase(name);
		return true;
	}
	else
	{
		return false;
	}
}

