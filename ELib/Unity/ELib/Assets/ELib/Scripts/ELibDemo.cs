using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ELibDemo : MonoBehaviour
{
    public TangramBaseCamera m_TangramCamera;
    public Texture2D m_TemplateGraphImage;

    public Renderer m_ResultRenderer;
    Texture2D m_PaintedTexture;


    void Start()
    {
        m_TangramCamera.gameObject.SetActive(true);

        UnityInterface.Init();
        UnityInterface.SetTemplate(m_TemplateGraphImage);

        m_PaintedTexture = new Texture2D(m_TangramCamera.Width(), m_TangramCamera.Height(), TextureFormat.RGBA32, false); ;

        m_ResultRenderer.sharedMaterial = new Material(Shader.Find("Unlit/Texture"));
        m_ResultRenderer.sharedMaterial.SetTexture("_MainTex", m_PaintedTexture);
        Vector3 scale = new Vector3(m_TangramCamera.Width(), m_TangramCamera.Height(), 1);
        m_ResultRenderer.gameObject.transform.localScale = scale / 2;
        m_ResultRenderer.gameObject.transform.localPosition = new Vector3(m_TangramCamera.Width() / 4, -m_TangramCamera.Height() / 4, -1);

        StartCoroutine(Feed());
    }
    void OnDestroy()
    {
        StopAllCoroutines();
        UnityInterface.Destory();
    }

    
    IEnumerator Feed()
    {
        string ret = UnityInterface.SetResultTexture(m_PaintedTexture);

        Debug.Log("Feed SetResultTexture :" + ret);
        if (!string.IsNullOrEmpty(ret))
        {

            string eventName = UnityInterface.FeedTexture(m_TangramCamera.GetTexture());
            Debug.Log("Feed FeedTexture :" + eventName);
            if (!string.IsNullOrEmpty(eventName))
            {
                while (true)
                {
                    yield return new WaitForEndOfFrame();
                    UnityInterface.IssueEvent(eventName);
                }
            }
        }
    }
}
