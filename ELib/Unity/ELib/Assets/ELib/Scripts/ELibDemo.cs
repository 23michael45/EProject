using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ELibDemo : MonoBehaviour
{
    public Camera m_MainCamera;
    WebCamTexture m_WebCamera;
    public Renderer m_WebCameraRenderer;

    void Start()
    {
        m_WebCamera = new WebCamTexture();
        m_WebCamera.Play();

        Vector3 scale = new Vector3(m_WebCamera.width, m_WebCamera.height, 1);
        m_WebCameraRenderer.gameObject.transform.localScale = scale;

        float screenRate = (float)Screen.width / Screen.height;
        float cameraRate = (float)m_WebCamera.width / m_WebCamera.height;
        if (screenRate > cameraRate)
        {

            m_MainCamera.orthographicSize = (float)m_WebCamera.height / 2;

        }
        else
        {

            m_MainCamera.orthographicSize = (float)Screen.height / Screen.width * m_WebCamera.width / 2;

        }

        m_WebCameraRenderer.sharedMaterial.SetTexture("_MainTex", m_WebCamera);

        UnityInterface.Init();
        
    }
    void OnDestroy()
    {

        m_WebCamera.Stop();
        UnityInterface.Destory();
    }


    void Update()
    {
        UnityInterface.FeedFrame(m_WebCamera);
    }
}
