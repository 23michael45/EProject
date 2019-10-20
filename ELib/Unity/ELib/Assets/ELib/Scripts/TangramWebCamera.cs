using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TangramWebCamera : TangramBaseCamera
{
    public Camera m_MainCamera;
    public Renderer m_WebCameraRenderer;
    WebCamTexture m_WebCameraTexture;

    public override int Width()
    {
        return m_WebCameraTexture.width;
    }
    public override int Height()
    {
        return m_WebCameraTexture.height;
    }
    void Start()
    {
        m_WebCameraTexture = new WebCamTexture();
        m_WebCameraTexture.Play();

        Vector3 scale = new Vector3(m_WebCameraTexture.width, m_WebCameraTexture.height, 1);
        m_WebCameraRenderer.gameObject.transform.localScale = scale;

        float screenRate = (float)Screen.width / Screen.height;
        float cameraRate = (float)m_WebCameraTexture.width / m_WebCameraTexture.height;
        if (screenRate > cameraRate)
        {

            m_MainCamera.orthographicSize = (float)m_WebCameraTexture.height / 2;

        }
        else
        {

            m_MainCamera.orthographicSize = (float)Screen.height / Screen.width * m_WebCameraTexture.width / 2;

        }

        m_WebCameraRenderer.sharedMaterial.SetTexture("_MainTex", m_WebCameraTexture);
    }

    void OnDestroy()
    {
        m_WebCameraTexture.Stop();
    }
    public override Texture GetTexture()
    {
        return m_WebCameraTexture;
    }
}
