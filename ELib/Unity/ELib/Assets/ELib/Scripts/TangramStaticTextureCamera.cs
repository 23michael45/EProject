using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TangramStaticTextureCamera : TangramBaseCamera
{
    public Texture2D m_Texture;
    private void Awake()
    {
        Debug.Log("TangramStaticTextureCamera texture is readable : " + m_Texture.isReadable);
    }

    public override int Width()
    {
        return m_Texture.width;
    }
    public override int Height()
    {
        return m_Texture.height;
    }
    public override int Channel()
    {
        int channel = 1;
        if (m_Texture.format == TextureFormat.RGBA32)
        {
            channel = 4;
        }
        else if (m_Texture.format == TextureFormat.RGB24)
        {
            channel = 3;

        }

        return channel;
    }
    public override Texture GetTexture()
    {
        return m_Texture;
    }
}
