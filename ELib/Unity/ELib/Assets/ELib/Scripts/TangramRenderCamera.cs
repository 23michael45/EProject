using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TangramRenderCamera : TangramBaseCamera
{
    public Camera m_MainCamera;
    RenderTexture m_RenderTexture;
    public int m_Width;
    public int m_Height;
    // Start is called before the first frame update
    void Awake()
    {
        m_RenderTexture = new RenderTexture(m_Width, m_Height, 8);
        m_MainCamera.targetTexture = m_RenderTexture;
    }
    public override int Width()
    {
        return m_RenderTexture.width;
    }
    public override int Height()
    {
        return m_RenderTexture.height;
    }
    public override Texture GetTexture()
    {
        return m_RenderTexture;
    }
}
