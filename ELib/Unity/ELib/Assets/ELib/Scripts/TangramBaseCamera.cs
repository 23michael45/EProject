using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TangramBaseCamera : MonoBehaviour
{
    public virtual int Width()
    {
        return 0;
    }
    public virtual int Height()
    {
        return 0;
    }

    public virtual int Channel()
    {
        return 0;
    }

    public virtual Texture GetTexture()
    {
        return null;
    }
}
