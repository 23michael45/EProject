using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using UnityEngine;

public static class UnityInterface
{
    static IntPtr m_PluginDll;

#if UNITY_ANDROID && !UNITY_EDITOR

    const string m_ELibName = "native-lib";//not libnative-lib.so or libnative-lib
    const string m_UnityNativeLoaderName = "native-lib";//without .dll
  
#elif UNITY_IOS

#else
    const string m_ELibName = "ELib";
    const string m_UnityNativeLoaderName = "UnityNativeLoader";//without .dll
#endif
    static IntPtr m_Handle;
#if UNITY_EDITOR
    static class NativeMethods
    {
        [DllImport("kernel32", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool FreeLibrary(IntPtr hModule);

        [DllImport("kernel32", SetLastError = true, CharSet = CharSet.Unicode)]
        public static extern IntPtr LoadLibrary(string lpFileName);

        [DllImport("kernel32")]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);
    }

    static void GetProcAddress<T>(ref T func, string funcname) where T : Delegate
    {
        IntPtr pAddressOfFunctionToCall = NativeMethods.GetProcAddress(m_PluginDll, funcname);

        UnityEngine.Debug.Log(string.Format("Load Function: {0} Addr: {1}", funcname, pAddressOfFunctionToCall.ToInt64()));

        func = (T)Marshal.GetDelegateForFunctionPointer(pAddressOfFunctionToCall, typeof(T));

    }

    public static void LoadLibrary()
    {
        if (m_PluginDll != IntPtr.Zero)
        {
            UnityEngine.Debug.Log("Library alreadly Loaded");
            return;
        }

        string sCurrentPath = Directory.GetCurrentDirectory();
        UnityEngine.Debug.Log("sCurrentPath:" + sCurrentPath);

        UnityEngine.Debug.Log("Load Library");
        string sPluginPath = Path.Combine(Application.dataPath, @"ELib/Plugins/x86_64");
        string dllPath = Path.Combine(sPluginPath, m_ELibName + ".dll");
        if (File.Exists(dllPath))
        {
            Directory.SetCurrentDirectory(sPluginPath);
            m_PluginDll = NativeMethods.LoadLibrary(dllPath);

            if (m_PluginDll != IntPtr.Zero)
            {
                LoadFunctions();
            }
            else
            {
                UnityEngine.Debug.LogError("Load Library Failed : " + Marshal.GetLastWin32Error().ToString());
            }


            Directory.SetCurrentDirectory(sCurrentPath);
        }
        else
        {
            UnityEngine.Debug.LogError("Dll File Not Exist");

        }
    }

    public static void FreeLibrary()
    {
        if (m_PluginDll == IntPtr.Zero)
        {
            return;
        }
        bool result = NativeMethods.FreeLibrary(m_PluginDll);
        if (result)
        {

            m_PluginDll = IntPtr.Zero;
            UnityEngine.Debug.Log("Free Library");
        }
        else
        {

            UnityEngine.Debug.Log("Free Library Failed");
        }
    }



    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate IntPtr CreateELib_Delegate();
    static CreateELib_Delegate CreateELib;


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void DestroyELib_Delegate(IntPtr handle);
    static DestroyELib_Delegate DestroyELib;


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate IntPtr GetNativeRenderingEvent_Delegate(IntPtr handle,[MarshalAs(UnmanagedType.LPStr)]string eventName);
    static GetNativeRenderingEvent_Delegate GetNativeRenderingEvent;

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void SetTemplateGraph_Delegate(IntPtr handle, IntPtr texData, int width, int height, int channel);
    static SetTemplateGraph_Delegate SetTemplateGraph;


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void Feed_Delegate(IntPtr handle,IntPtr texData, int width, int height, int channel);
    static Feed_Delegate Feed;

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    private delegate string FeedNativeTexture_Delegate(IntPtr handle, IntPtr nativeTexPtr, int width, int height, int channel);
    static FeedNativeTexture_Delegate FeedNativeTexture;

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    private delegate string SetPaintedTexture_Delegate(IntPtr handle, IntPtr nativeTexPtr, int width, int height, int channel);
    static SetPaintedTexture_Delegate　SetPaintedTexture;

    static void LoadFunctions()
    {
        GetProcAddress(ref CreateELib, "CreateELib");
        GetProcAddress(ref DestroyELib, "DestroyELib");
        GetProcAddress(ref GetNativeRenderingEvent, "GetNativeRenderingEvent"); 

        GetProcAddress(ref Feed, "Feed");
        GetProcAddress(ref FeedNativeTexture, "FeedNativeTexture");

        GetProcAddress(ref SetTemplateGraph, "SetTemplateGraph");
        GetProcAddress(ref SetPaintedTexture, "SetPaintedTexture");
    }
#else
    
    [DllImport(m_ELibName)]
    private static extern IntPtr CreateELib();
    
    [DllImport(m_ELibName)]
    private static extern void DestroyELib(IntPtr handle);

    [DllImport(m_ELibName)]
    private static extern IntPtr GetNativeRenderingEvent(IntPtr handle,[MarshalAs(UnmanagedType.LPStr)]string eventName);
    
    [DllImport(m_ELibName)]
    private static extern void Feed(IntPtr handle,IntPtr texData,int width,int height,int channel);
    
    [DllImport(m_ELibName)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    private static extern string FeedNativeTexture(IntPtr handle,IntPtr nativeTexPtr,int width,int height,int channel,int channel);

    [DllImport(m_ELibName)]
    private static extern void SetTemplateGraph(IntPtr handle,IntPtr texData,int width,int height,int channel);
    
    [DllImport(m_ELibName)]
    [return: MarshalAs(UnmanagedType.LPStr)]
    private static extern string SetPaintedTexture(IntPtr handle, IntPtr nativeTexPtr, int width, int height,int channel);

#endif

    [DllImport(m_UnityNativeLoaderName)]
    private static extern void UnityNativeLoader();

    public static void Init()
    {
        //pass unity interfaces to native
        UnityNativeLoader();

#if UNITY_EDITOR
        LoadLibrary();
#endif
        m_Handle = CreateELib();


        IssueEvent("InitGraphicEvent");
    }
    public static void Destory()
    {
        if(m_Handle != IntPtr.Zero)
        {
            DestroyELib(m_Handle);
        }
#if UNITY_EDITOR
        FreeLibrary();
#endif
    }
    public static void IssueEvent(string eventName)
    {
        var func = UnityInterface.GetNativeRenderingEvent(m_Handle,eventName);
        if (func != IntPtr.Zero)
        {
            GL.IssuePluginEvent(func, 1);
        }
    }

    public static unsafe void SetTemplate(Texture2D tex)
    {
        //whatever channel is 3 or 4 ,getpixels32 will get a 4 channel data
        Color32[] texDataColor = tex.GetPixels32();
        fixed (Color32* p = (texDataColor))
        {
            SetTemplateGraph(m_Handle, (IntPtr)p, tex.width, tex.height, 4);
        }

    }

    public static unsafe void FeedFrame(WebCamTexture tex, int channel)
    {
        Color32[] texDataColor = tex.GetPixels32();
        fixed (Color32* p = (texDataColor))
        {
            Feed(m_Handle, (IntPtr)p,tex.width,tex.height, channel);
        }
    }

    public static unsafe string FeedTexture(Texture tex, int channel)
    {
        IntPtr nativeTexPtr = tex.GetNativeTexturePtr();
        UnityEngine.Debug.Log(nativeTexPtr);

        string eventName = "";
        eventName = FeedNativeTexture(m_Handle, nativeTexPtr, tex.width, tex.height, channel);

        return eventName;
    }

    public static unsafe string SetResultTexture(Texture2D tex, int channel)
    {
        IntPtr nativeTexPtr = tex.GetNativeTexturePtr();
        string eventName = SetPaintedTexture(m_Handle, nativeTexPtr, tex.width, tex.height, channel);

        return eventName;
    }
}
