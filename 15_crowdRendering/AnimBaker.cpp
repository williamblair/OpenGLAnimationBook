#include <AnimBaker.h>

void BakeAnimationTexture(Skeleton& skel, Clip& clip, AnimTexture& outTex)
{
    Pose& bindPose = skel.GetBindPose();
    Pose pose = bindPose;
    unsigned int texWidth = outTex.GetSize();
    for (unsigned int x = 0; x < texWidth; ++x)
    {
        float t = (float)x / (float)(texWidth - 1);
        float start = clip.GetStartTime();
        float time = start + clip.GetDuration() * t;
        clip.Sample(pose, time);

        for (unsigned int y = 0; y < pose.GetSize() * 3; y += 3)
        {
            Transform node = pose.GetGlobalTransform(y / 3);
            outTex.SetTexel(x, y+0, node.position);
            outTex.SetTexel(x, y+1, node.rotation);
            outTex.SetTexel(x, y+2, node.scale);
        }
    }

    outTex.UploadToGPU();
}

