using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class TangramMesh : MonoBehaviour
{
    public enum TangramTypeName
    {
        TTN_STRI_1,
        TTN_STRI_2,
        TTN_MTRI,
        TTN_LTRI_1,
        TTN_LTRI_2,
        TTN_SQR,
        TTN_PARA,
        TTN_NONE,
    };

    public float m_EdgeLen = 4;

    public Texture2D m_Texture;
    public Dictionary<TangramTypeName, GameObject> m_TangrameElements;


    void LTri1(out Vector3[] vertices, out Vector2[] uvs, out int[] tris)
    {
        float unit = m_EdgeLen / 6;

        vertices = new Vector3[3];
        vertices[0] = new Vector3(0, unit * 2, 0);
        vertices[1] = new Vector3(unit * 3, -unit, 0);
        vertices[2] = new Vector3(-unit * 3, -unit, 0);

        uvs = new Vector2[3];
        uvs[0] = new Vector2(0.5f, 0.5f);
        uvs[1] = new Vector2(0.0f, 1.0f);
        uvs[2] = new Vector2(0.0f, 0.0f);

        tris = new int[3];
        tris[0] = 0;
        tris[1] = 1;
        tris[2] = 2;
    }
    void LTri2(out Vector3[] vertices, out Vector2[] uvs, out int[] tris)
    {
        float unit = m_EdgeLen / 6;

        vertices = new Vector3[3];
        vertices[0] = new Vector3(0, unit * 2, 0);
        vertices[1] = new Vector3(unit * 3, -unit, 0);
        vertices[2] = new Vector3(-unit * 3, -unit, 0);

        uvs = new Vector2[3];
        uvs[0] = new Vector2(0.5f, 0.5f);
        uvs[1] = new Vector2(1.0f, 0.0f);
        uvs[2] = new Vector2(0.0f, 0.0f);

        tris = new int[3];
        tris[0] = 0;
        tris[1] = 1;
        tris[2] = 2;
    }

    void MTri(out Vector3[] vertices, out Vector2[] uvs, out int[] tris)
    {
        float unit = m_EdgeLen / 6 / Mathf.Sqrt(2);

        vertices = new Vector3[3];
        vertices[0] = new Vector3(0, unit * 2, 0);
        vertices[1] = new Vector3(unit * 3, -unit, 0);
        vertices[2] = new Vector3(-unit * 3, -unit, 0);

        uvs = new Vector2[3];
        uvs[0] = new Vector2(1.0f, 1.0f);
        uvs[1] = new Vector2(1.0f, 0.5f);
        uvs[2] = new Vector2(0.5f, 1.0f);

        tris = new int[3];
        tris[0] = 0;
        tris[1] = 1;
        tris[2] = 2;
    }

    void STri1(out Vector3[] vertices, out Vector2[] uvs, out int[] tris)
    {
        float unit = m_EdgeLen / 6 / 2;

        vertices = new Vector3[3];
        vertices[0] = new Vector3(0, unit * 2, 0);
        vertices[1] = new Vector3(unit * 3, -unit, 0);
        vertices[2] = new Vector3(-unit * 3, -unit, 0);

        uvs = new Vector2[3];
        uvs[0] = new Vector2(0.5f, 0.5f);
        uvs[1] = new Vector2(0.25f, 0.75f);
        uvs[2] = new Vector2(0.75f, 0.75f);

        tris = new int[3];
        tris[0] = 0;
        tris[1] = 1;
        tris[2] = 2;
    }
    void STri2(out Vector3[] vertices, out Vector2[] uvs, out int[] tris)
    {

        float unit = m_EdgeLen / 6 / 2;

        vertices = new Vector3[3];
        vertices[0] = new Vector3(0, unit * 2, 0);
        vertices[1] = new Vector3(unit * 3, -unit, 0);
        vertices[2] = new Vector3(-unit * 3, -unit, 0);

        uvs = new Vector2[3];
        uvs[0] = new Vector2(0.75f, 0.25f);
        uvs[1] = new Vector2(1.0f, 0.5f);
        uvs[2] = new Vector2(1.0f, 0.0f);

        tris = new int[3];
        tris[0] = 0;
        tris[1] = 1;
        tris[2] = 2;
    }

    void Square(out Vector3[] vertices, out Vector2[] uvs, out int[] tris)
    {
        float edge = m_EdgeLen * Mathf.Sqrt(2) / 4;

        vertices = new Vector3[4];
        vertices[0] = new Vector3(-edge / 2, -edge / 2, 0);
        vertices[1] = new Vector3(-edge / 2, edge / 2, 0);
        vertices[2] = new Vector3(edge / 2, edge / 2, 0);
        vertices[3] = new Vector3(edge / 2, -edge / 2, 0);

        uvs = new Vector2[4];
        uvs[0] = new Vector2(0.5f, 0.5f);
        uvs[1] = new Vector2(0.75f, 0.75f);
        uvs[2] = new Vector2(1.0f, 0.5f);
        uvs[3] = new Vector2(0.75f, 0.25f);

        tris = new int[6];
        tris[0] = 0;
        tris[1] = 1;
        tris[2] = 2;
        tris[3] = 0;
        tris[4] = 2;
        tris[5] = 3;
    }

    void Para(out Vector3[] vertices, out Vector2[] uvs, out int[] tris)
    {
        float longEdge = m_EdgeLen / 2;
        float h = longEdge / 2;
        float unit = h / 2;

        vertices = new Vector3[4];
        vertices[0] = new Vector3(-unit, -unit, 0);
        vertices[1] = new Vector3(-3 * unit, unit, 0);
        vertices[2] = new Vector3(unit, unit, 0);
        vertices[3] = new Vector3(3 * unit, -unit, 0);

        uvs = new Vector2[4];
        uvs[0] = new Vector2(0.25f, 0.75f);
        uvs[1] = new Vector2(0.0f, 1.0f);
        uvs[2] = new Vector2(0.5f, 1.0f);
        uvs[3] = new Vector2(0.75f, 0.75f);

        tris = new int[6];
        tris[0] = 0;
        tris[1] = 1;
        tris[2] = 2;
        tris[3] = 0;
        tris[4] = 2;
        tris[5] = 3;
    }

    [ExecuteInEditMode]
    private void Start()
    {
        Clear();




        Vector3[] vertices;
        Vector2[] uvs;
        int[] tris;
        
        LTri1(out vertices, out uvs, out tris);
        CreateMesh(TangramTypeName.TTN_LTRI_1,vertices,uvs,tris);


        LTri2(out vertices, out uvs, out tris);
        CreateMesh(TangramTypeName.TTN_LTRI_2, vertices, uvs, tris);


        MTri(out vertices, out uvs, out tris);
        CreateMesh(TangramTypeName.TTN_MTRI, vertices, uvs, tris);


        STri1(out vertices, out uvs, out tris);
        CreateMesh(TangramTypeName.TTN_STRI_1, vertices, uvs, tris);


        STri2(out vertices, out uvs, out tris);
        CreateMesh(TangramTypeName.TTN_STRI_2, vertices, uvs, tris);


        Square(out vertices, out uvs, out tris);
        CreateMesh(TangramTypeName.TTN_SQR, vertices, uvs, tris);


        Para(out vertices, out uvs, out tris);
        CreateMesh(TangramTypeName.TTN_PARA, vertices, uvs, tris);
    }

    void Clear()
    {
        List<GameObject> list = new List<GameObject>();
        for(int i = 0; i< transform.childCount;i++)
        {
            list.Add(transform.GetChild(i).gameObject);
        }
        foreach(GameObject go in list)
        {
            GameObject.DestroyImmediate(go);
        }
    }

    void CreateMesh(TangramTypeName eleName,Vector3[] vertices,Vector2[] uvs,int[] tris)
    {
        Mesh m = new Mesh();

        m.vertices = vertices;
        m.uv = uvs;
        m.triangles = tris;

        GameObject gonew = new GameObject();
        gonew.name = eleName.ToString();
        gonew.transform.parent = gameObject.transform;
        gonew.transform.localPosition = Vector3.zero;

        MeshRenderer r = gonew.AddComponent<MeshRenderer>();
        MeshFilter mf = gonew.AddComponent<MeshFilter>();
        mf.mesh = m;

        Shader shader = Shader.Find("Unlit/Transparent");
        r.sharedMaterial = new Material(shader);
        r.sharedMaterial.SetTexture("_MainTex", m_Texture);

    }
}
