#ifndef NativeGraphics_h__
#define NativeGraphics_h__
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "UnityNativeLoader.h"
#include <chrono>
#include <thread>
#include <mutex>

struct NativeMeshVertex
{
	float pos[3];
	float normal[3];
	float color[4];
	float uv[2];
};
struct NativeMesh
{
	void* handle;
	std::vector<NativeMeshVertex> vertices;

}; 
struct NativeTexture
{
	NativeTexture(void* handle, int width, int height, int channel);
	~NativeTexture();
	void* mHandle;
	int mWidth;
	int mHeight;
	int mChannel;
	void* mDataPtr;

};
struct NativeRenderEvent
{
	UnityRenderingEvent func;
	void* param;
};


void InitNativeGraphics();
void UninitNativeGraphics();

bool AddNativeRenderingEvent(std::string name, UnityRenderingEvent func,void* param);
bool RemoveNativeRenderingEvent(std::string name); 
bool GetNativeRenderingEvent(std::string name, std::shared_ptr<NativeRenderEvent>& spEvent);
std::mutex& GetNativeRenderingEventMutex();


bool AddNativeTexture(std::string name, void* handle,int w,int h,int c);
bool RemoveNativeTexture(std::string name);
bool ModifyNativeTexture(std::string name, void* pData);
bool GetNativeTexture(std::string name, std::shared_ptr<NativeTexture>& spTexture);


bool AddNativeMesh(std::string name, void* vertexBufferHandle, int vertexCount, float* sourceVertices, float* sourceNormals, float* sourceUV);
bool RemoveNativeMesh(std::string name);
#endif // NativeGraphics_h__
