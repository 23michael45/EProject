#ifndef Interface_h__
#define Interface_h__

#ifdef _WINDOWS


#ifdef CPP_USE_DLL
#define CPP_INTERFACE_API __declspec(dllimport) 
#else
#define CPP_INTERFACE_API __declspec(dllexport) 
#endif


#ifdef UNITY_USE_DLL
#define UNITY_EXPORT_INTERFACE_API __declspec(dllimport) 
#else
#define UNITY_EXPORT_INTERFACE_API __declspec(dllexport) 
#endif


#else

#define CPP_INTERFACE_API  
#define UNITY_EXPORT_INTERFACE_API




#endif // _WINDOWS


#endif // Interface_h__
