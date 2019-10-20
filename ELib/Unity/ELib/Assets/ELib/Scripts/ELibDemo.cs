using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ELibDemo : MonoBehaviour
{
    public TangramBaseCamera m_TangramCamera;
    public Texture2D m_TemplateGraphImage;
    public RawImage m_PaintedImage;



    Texture2D m_PaintedTexture;

    void Start()
    {


        UnityInterface.Init();
        UnityInterface.SetTemplate(m_TemplateGraphImage);

        m_PaintedTexture = new Texture2D(m_TangramCamera.Width(), m_TangramCamera.Height(), TextureFormat.RGBA32, false); ;
        m_PaintedImage.texture = m_PaintedTexture;

    }
    void OnDestroy()
    {
        
        UnityInterface.Destory();
    }


    void Update()
    {
        UnityInterface.FeedFrame(m_TangramCamera.GetTexture());

        //UnityInterface.PaintTexture(ref m_PaintedTexture);
        //m_PaintedImage.texture = m_PaintedTexture;
    }
}
