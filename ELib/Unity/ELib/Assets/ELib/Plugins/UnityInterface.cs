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
    const string m_DllName = "ELib.dll";
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
        string sPluginPath = Path.Combine(Application.dataPath, @"ELib/Plugins");
        string dllPath = Path.Combine(sPluginPath, m_DllName);
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
    private delegate void SetTemplateGraph_Delegate(IntPtr handle, IntPtr texData, int width, int height);
    static SetTemplateGraph_Delegate SetTemplateGraph;


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void Feed_Delegate(IntPtr handle,IntPtr texData, int width, int height);
    static Feed_Delegate Feed;

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate bool GetPaintedTexture_Delegate(IntPtr handle, ref IntPtr texData, ref int width, ref int height);
    static GetPaintedTexture_Delegate GetPaintedTexture;

    static void LoadFunctions()
    {
        GetProcAddress(ref CreateELib, "CreateELib");
        GetProcAddress(ref DestroyELib, "DestroyELib");
        GetProcAddress(ref Feed, "Feed");
        GetProcAddress(ref SetTemplateGraph, "SetTemplateGraph");
        GetProcAddress(ref GetPaintedTexture, "GetPaintedTexture");
    }
#else
    
    [DllImport(m_DllName)]
    private static extern IntPtr CreateELib();
    
    [DllImport(m_DllName)]
    private static extern void DestroyELib(IntPtr handle);

    [DllImport(m_DllName)]
    private static extern void Feed(IntPtr handle,IntPtr texData,int width,int height);

    [DllImport(m_DllName)]
    private static extern void SetTemplateGraph(IntPtr handle,IntPtr texData,int width,int height);
    
    [DllImport(m_DllName)]
    private static extern bool GetPaintedTexture(IntPtr handle, ref IntPtr texData, ref int width, ref int height);

#endif


    public static void Init()
    {
        LoadLibrary();
        m_Handle = CreateELib();
    }
    public static void Destory()
    {
        if(m_Handle != IntPtr.Zero)
        {
            DestroyELib(m_Handle);
        }
        FreeLibrary();
    }

    public static unsafe void SetTemplate(Texture2D tex)
    {
        Color32[] texDataColor = tex.GetPixels32();
        fixed (Color32* p = (texDataColor))
        {
            SetTemplateGraph(m_Handle, (IntPtr)p, tex.width, tex.height);
        }

    }

    public static unsafe void FeedFrame(WebCamTexture tex)
    {
        Color32[] texDataColor = tex.GetPixels32();
        fixed (Color32* p = (texDataColor))
        {
            Feed(m_Handle, (IntPtr)p,tex.width,tex.height);
        }
    }

    public static unsafe void PaintTexture(ref Texture2D tex)
    {
        IntPtr texBuffer = IntPtr.Zero;
        int width = 0;
        int height = 0;

        if(GetPaintedTexture(m_Handle,ref texBuffer,ref width,ref height))
        {
            int piexlSize = width * height;
            Color32[] colors = new Color32[piexlSize];
            int offset = 0;
            int vecSize = Marshal.SizeOf(typeof(byte)) * 3;

            for (int i = 0; i < piexlSize; i++)
            {
                var c = new Color32();
                c.r = (byte)Marshal.PtrToStructure(new IntPtr(texBuffer.ToInt64() + offset + 0), typeof(byte));
                c.g = (byte)Marshal.PtrToStructure(new IntPtr(texBuffer.ToInt64() + offset + 1), typeof(byte));
                c.b = (byte)Marshal.PtrToStructure(new IntPtr(texBuffer.ToInt64() + offset + 2), typeof(byte));
                c.a = 255;
                colors[i] = c;
                offset += vecSize;
            }

            tex.SetPixels32(colors);
            tex.Apply();
        }
    }
}
