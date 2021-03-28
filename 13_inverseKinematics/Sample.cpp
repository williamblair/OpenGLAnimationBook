#include <Sample.h>
#include <vector>
#include <cstdarg>

#define GIZMO_SIZE 0.25f
    
static inline float deg2rad(float inDeg) {
    return inDeg * (M_PI/180.0f);
}

void GetScalarValues(std::vector<float>& outScalars,
                     unsigned int inComponentCount,
                     const cgltf_accessor& inAccessor)
{
    outScalars.resize(inAccessor.count * inComponentCount);
    for (cgltf_size i = 0; i < inAccessor.count; ++i) {
        cgltf_accessor_read_float(&inAccessor, 
                                  i, 
                                  &outScalars[i * inComponentCount], 
                                  inComponentCount);
    }
}

void StaticMeshFromAttribute(Mesh& outMesh, cgltf_attribute& attribute)
{
    cgltf_attribute_type attribType = attribute.type;
    cgltf_accessor& accessor = *attribute.data;

    unsigned int componentCount = 0;
    switch (accessor.type)
    {
    case cgltf_type_vec2: componentCount = 2; break;
    case cgltf_type_vec3: componentCount = 3; break;
    case cgltf_type_vec4: componentCount = 4; break;
    default:
        std::cout << __func__ << ": Unknown accessor type" << std::endl;
        return;
    }

    std::vector<float> values;
    GetScalarValues(values, componentCount, accessor);
    unsigned int accessorCount = (unsigned int)accessor.count;

    std::vector<Vec3>& positions = outMesh.GetPositions();
    std::vector<Vec3>& normals = outMesh.GetNormals();
    std::vector<Vec2>& texCoords = outMesh.GetTexCoords();

    for (unsigned int i = 0; i < accessorCount; ++i)
    {
        int index = i * componentCount;
        
        switch (attribType)
        {
        case cgltf_attribute_type_position:
            positions.push_back(Vec3(values[index + 0],
                                     values[index + 1],
                                     values[index + 2]));
            break;
        case cgltf_attribute_type_normal:
        {
            Vec3 normal = Vec3(values[index + 0],
                               values[index + 1],
                               values[index + 2]);
            if (lenSq(normal) < 0.000001f) {
                normal = Vec3(0, 1, 0);
            }
            normals.push_back(normal);
            break;
        }
        case cgltf_attribute_type_texcoord:
            texCoords.push_back(Vec2(values[index + 0],
                                     values[index + 1]));
            break;
        } // end switch (attribType)
    }
    
}

std::vector<Mesh> LoadStaticMeshes(cgltf_data* data)
{
    std::vector<Mesh> result;
    cgltf_node* nodes = data->nodes;
    unsigned int nodeCount = (unsigned int)data->nodes_count;

    for (unsigned int i = 0; i < nodeCount; ++i)
    {
        cgltf_node* node = &nodes[i];
        if (node->mesh == nullptr) {
            continue;
        }

        unsigned int numPrims = 
            (unsigned int)node->mesh->primitives_count;
        for (unsigned int j = 0; j < numPrims; ++j)
        {
            result.push_back(Mesh());
            Mesh& mesh = result[result.size()-1];

            cgltf_primitive* primitive = &node->mesh->primitives[j];
            unsigned int numAttributes = 
                (unsigned int)primitive->attributes_count;
            
            for (unsigned int k = 0; k < numAttributes; ++k) {
                cgltf_attribute* attribute = &primitive->attributes[k];
                StaticMeshFromAttribute(mesh, *attribute);
            }

            if (primitive->indices != nullptr) {
                unsigned int indexCount = 
                    (unsigned int)primitive->indices->count;

                std::vector<unsigned int>& indices = mesh.GetIndices();
                indices.resize(indexCount);

                for (unsigned int k = 0; k < indexCount; ++k) {
                    indices[k] = 
                        (unsigned int)cgltf_accessor_read_index(
                            primitive->indices, k);
                }
            } // end for j = 0 ... numPrims

            mesh.UpdateOpenGLBuffers();
        }
    }

    return result;
}

bool Sample::Initialize(const char* title, const int width, const int height)
{
    if (!Application::Initialize(title, width, height)) {
        return false;
    }

#if (SAMPLE_1 || SAMPLE_2)
    solver.Resize(6);
    solver[0] = Transform(Vec3(), angleAxis(deg2rad(90.0f), Vec3(1,0,0)), Vec3(1,1,1));
    solver[1] = Transform(Vec3(0, 0, 1.0f), Quat(), Vec3(1,1,1));
    solver[2] = Transform(Vec3(0, 0, 1.5f), Quat(), Vec3(1,1,1));
    solver[3] = Transform(Vec3(0, 0, 0.5f), angleAxis(deg2rad(90.0f), Vec3(0,1,0)), Vec3(1,1,1));
    solver[4] = Transform(Vec3(0, 0, 0.5f), Quat(), Vec3(1,1,1));
    solver[5] = Transform(Vec3(0, 0, 0.5f), Quat(), Vec3(1,1,1));

    target.position = Vec3(1, -2, 0);

    VectorTrack& path = targetPath.GetPositionTrack();
    path.Resize(14);
    SetFrame(path, 0, 0.0f, Vec3(1, -2, 0) * 0.5f);
    SetFrame(path, 1, 1.0f, Vec3(1,  2, 0) * 0.5f);
    SetFrame(path, 2, 2.0f, Vec3(1, 4, 0) * 0.5f);
    SetFrame(path, 3, 3.0f, Vec3(3, 4, 0) * 0.5f);
    SetFrame(path, 4, 4.0f, Vec3(5, 4, 0) * 0.5f);
    SetFrame(path, 5, 5.0f, Vec3(5, 4, 2) * 0.5f);
    SetFrame(path, 6, 6.0f, Vec3(5, 4, 4) * 0.5f);
    SetFrame(path, 7, 7.0f, Vec3(3, 4, 4) * 0.5f);
    SetFrame(path, 8, 8.0f, Vec3(3, 2, 4) * 0.5f);
    SetFrame(path, 9, 9.0f, Vec3(3, 2, 2) * 0.5f);
    SetFrame(path, 10, 10.0f, Vec3(1, 2, 2) * 0.5f);
    SetFrame(path, 11, 11.0f, Vec3(1, 0, 2) * 0.5f);
    SetFrame(path, 12, 12.0f, Vec3(1, -2, 2) * 0.5f);
    SetFrame(path, 13, 13.0f, Vec3(1, -2, 0) * 0.5f);

    solverLines = new DebugDraw();
    solverPoints = new DebugDraw();
    targetVisual[0] = new DebugDraw(2);
    targetVisual[1] = new DebugDraw(2);
    targetVisual[2] = new DebugDraw(2);

    camPitch = 45.0f;
    camYaw = 60.0f;
    camDist = 7.0f;
    playTime = 0.0f;
#endif // SAMPLE_1 || SAMPLE_2

#if SAMPLE_3

    lastModelY = 0.0f;

    motionTrack.Resize(5);
    motionTrack.SetInterpolation(Interpolation::Linear);
#define SetMotionTrack(index, inTime, valX, valZ) \
    motionTrack[index].time = (inTime);           \
    motionTrack[index].value[0] = (valX);       \
    motionTrack[index].value[2] = (valZ)

    SetMotionTrack(0, 0.0f, 0, 1);
    SetMotionTrack(1, 1.0f, 0, 10);
    SetMotionTrack(2, 3.0f, 22, 10);
    SetMotionTrack(3, 4.0f, 22, 2);
    SetMotionTrack(4, 6.0f, 0, 1);

#undef SetMotionTrack

    cgltf_data* gltf = LoadGLTFFile("Assets/Woman.gltf");
    meshes = LoadMeshes(gltf);
    skeleton = LoadSkeleton(gltf);
    clips = LoadAnimationClips(gltf);
    FreeGLTFFile(gltf);

    gltf = LoadGLTFFile("Assets/IKCourse.gltf");
    IKCourse = LoadStaticMeshes(gltf);
    FreeGLTFFile(gltf);
    courseTexture = new Texture("Assets/uv.png");
    triangles = MeshesToTriangles(IKCourse);

    staticShader = new Shader("Shaders/static.vert", "Shaders/lit.frag");
    skinnedShader = new Shader("Shaders/skinned.vert", "Shaders/lit.frag");
    diffuseTexture = new Texture("Assets/Woman.png");

    leftLeg = new IKLeg(skeleton, 
                        "LeftUpLeg", 
                        "LeftLeg", 
                        "LeftFoot", 
                        "LeftToeBase");
    leftLeg->SetAnkleOffset(0.2f);
    
    rightLeg = new IKLeg(skeleton,
                         "RightUpLeg",
                         "RightLeg",
                         "RightFoot",
                         "RightToeBase");
    rightLeg->SetAnkleOffset(0.2f);

    ScalarTrack& rightTrack = rightLeg->GetTrack();
    rightTrack.SetInterpolation(Interpolation::Cubic);
    rightTrack.Resize(4);
    rightTrack[0].value[0] = 1;
    rightTrack[1].value[0] = 0;
    rightTrack[2].value[0] = 0;
    rightTrack[3].value[0] = 1;
    rightTrack[0].time = 0.0f;
    rightTrack[1].time = 0.3f;
    rightTrack[2].time = 0.7f;
    rightTrack[3].time = 1.0f;

    ScalarTrack& leftTrack = leftLeg->GetTrack();
    leftTrack.SetInterpolation(Interpolation::Cubic);
    leftTrack.Resize(4);
    leftTrack[0].value[0] = 0;
    leftTrack[1].value[0] = 1;
    leftTrack[2].value[0] = 1;
    leftTrack[3].value[0] = 0;
    leftTrack[0].time = 0.0f;
    leftTrack[1].time = 0.4f;
    leftTrack[2].time = 0.6f;
    leftTrack[3].time = 1.0f;

    currentClip = 0;
    currentPose = skeleton.GetRestPose();

    currentPoseVisual = new DebugDraw();
    currentPoseVisual->FromPose(currentPose);
    currentPoseVisual->UpdateOpenGLBuffers();

    // for the UI
    for (unsigned int i = 0, size = (unsigned int)clips.size(); 
         i < size; ++i)
    {
        std::string& clipName = clips[i].GetName();
        if (clipName == "Walking") {
            currentClip = i;
        }
    }

    showIKPose = false;
    showCurrentPose = false;

    showEnvironment = true;
    showMesh = true;
    depthTest = false;
    timeMod = 1.0f;
    sinkIntoGround = 0.15f;

    // start character clamped to ground and down a little
    Ray groundRay(Vec3(model.position.x,
                       11,
                       model.position.z));
    unsigned int numTriangles = (unsigned int)triangles.size();
    Vec3 hitPoint;
    for (unsigned int i = 0; i < numTriangles; ++i)
    {
        if (RaycastTriangle(groundRay, triangles[i], hitPoint)) {
            model.position = hitPoint;
            break;
        }
    }
    model.position.y -= sinkIntoGround;
    lastModelY = model.position.y;

    toeLength = 0.3f;

#endif // SAMPLE_3

    return true;
}

#if (SAMPLE_1 || SAMPLE_2)
void Sample::SetFrame(VectorTrack& track, int index, float time, const Vec3& value)
{
    track[index].value[0] = value.x;
    track[index].value[1] = value.y;
    track[index].value[2] = value.z;
    track[index].time = time;
}
#endif

void Sample::Update(float inDeltaTime)
{
    Application::Update(inDeltaTime);

#if (SAMPLE_1 || SAMPLE_2)
    playTime += inDeltaTime;
    if (playTime > targetPath.GetEndTime()) {
        playTime -= targetPath.GetEndTime();
    }
    target = targetPath.Sample(target, playTime, true);
    solver.Solve(target);
#endif


#if SAMPLE_3
    float rayHeight = 2.1f;
    unsigned int numTriangles = (unsigned int)triangles.size();
    inDeltaTime = inDeltaTime * timeMod;
    Vec3 hitPoint;

    // time for moving the model on the rails of the walking track
    walkingTime += inDeltaTime * 0.3f;
    while (walkingTime > 6.0f) { walkingTime -= 6.0f; }

    // calc X and Z position of the model in world space
    float lastYPosition = model.position.y;
    Vec3 currentPosition = motionTrack.Sample(walkingTime, true);
    Vec3 nextPosition = motionTrack.Sample(walkingTime + 0.1f, true);
    currentPosition.y = lastYPosition;
    nextPosition.y = lastYPosition;
    model.position = currentPosition;

    // calculate formward direction of the model in world space
    Vec3 direction = normalized(nextPosition - currentPosition);
    Quat newDirection = lookRotation(direction, Vec3(0, 1, 0));
    if (dot(model.rotation, newDirection) < 0.0f) {
        newDirection = newDirection * -1.0f;
    }
    model.rotation = nlerp(model.rotation, newDirection, inDeltaTime * 10.0f);
    Vec3 characterForward = model.rotation * Vec3(0, 0, 1);

    // Calculate Y position of model in world space
    Ray groundRay(Vec3(model.position.x, 11, model.position.z));
    for (unsigned int i = 0; i < numTriangles; ++i) {
        if (RaycastTriangle(groundRay, triangles[i], hitPoint)) {
            // Sink model a little bit into the ground to avoid hyper extending legs
            model.position = hitPoint - Vec3(0, sinkIntoGround, 0);
            break;
        }
    }

    // figure out left and right leg position within up/down cycle
    playbackTime = clips[currentClip].Sample(currentPose, playbackTime + inDeltaTime);
    currentPoseVisual->FromPose(currentPose);
    float normalizedTime = (playbackTime - clips[currentClip].GetStartTime()) / clips[currentClip].GetDuration();
    if (normalizedTime < 0.0f) { std::cout << "unexpected normalized time" << std::endl; normalizedTime = 0.0f; }
    if (normalizedTime > 1.0f) { std::cout << "unexpected normalized time" << std::endl; normalizedTime = 1.0f; }
    float leftMotion = leftLeg->GetTrack().Sample(normalizedTime, true);
    float rightMotion = rightLeg->GetTrack().Sample(normalizedTime, true);

    // calculate fallback left ankle position, in case raycasts next don't hit
    Vec3 worldLeftAnkle = combine(model,
                                  currentPose.GetGlobalTransform(leftLeg->Ankle())).position;
    Ray leftAnkleRay(worldLeftAnkle + Vec3(0, 2, 0));
    Vec3 predictiveLeftAnkle = worldLeftAnkle;

    // calculate fallback right ankle position, in case raycasts next don't hit
    Vec3 worldRightAnkle = combine(model,
                                   currentPose.GetGlobalTransform(rightLeg->Ankle())).position;
    Ray rightAnkleRay(worldRightAnkle + Vec3(0, 2, 0));
    Vec3 predictiveRightAnkle = worldRightAnkle;

    // Perform raycasts to define IK based target points
    // For each ankle, the target is between the current position and predictive position
    Vec3 groundReference = model.position;
    for (unsigned int i = 0; i < numTriangles; ++i)
    {
        // left ankle
        if (RaycastTriangle(leftAnkleRay, triangles[i], hitPoint)) {
            if (lenSq(hitPoint - leftAnkleRay.origin) < rayHeight * rayHeight) {
                worldLeftAnkle = hitPoint;

                if (hitPoint.y < groundReference.y) {
                    groundReference = hitPoint - Vec3(0, sinkIntoGround, 0);
                }
            }

            predictiveLeftAnkle = hitPoint;
        }        

        // right ankle
        if (RaycastTriangle(rightAnkleRay, triangles[i], hitPoint)) {
            if (lenSq(hitPoint - rightAnkleRay.origin) < rayHeight * rayHeight) {
                worldRightAnkle = hitPoint;

                if (hitPoint.y < groundReference.y) {
                    groundReference = hitPoint - Vec3(0, sinkIntoGround, 0);
                }
            }

            predictiveRightAnkle = hitPoint;
        }        
    }

    // Lerp the Y position of the model over a small period of time to avoid popping
    model.position.y = lastModelY;
    model.position = lerp(model.position, groundReference, inDeltaTime * 10.0f);
    lastModelY = model.position.y;

    // Lerp between fully clamped to the ground, and somewhat clamped based on the 
    // current phase of the walk cycle
    worldLeftAnkle = lerp(worldLeftAnkle, predictiveLeftAnkle, leftMotion);
    worldRightAnkle = lerp(worldRightAnkle, predictiveRightAnkle, rightMotion);

    // now that model position is set and target position of ankles, can use IK
    // to solve the feet
    leftLeg->SolveForLeg(model, currentPose, worldLeftAnkle);
    rightLeg->SolveForLeg(model, currentPose, worldRightAnkle);

    // apply solved feet
    Blend(currentPose, currentPose, leftLeg->GetAdjustedPose(), 1, leftLeg->Hip());
    Blend(currentPose, currentPose, rightLeg->GetAdjustedPose(), 1, rightLeg->Hip());

    // Solve toes
    Transform leftAnkleWorld = combine(model, currentPose.GetGlobalTransform(leftLeg->Ankle()));
    Transform rightAnkleWorld = combine(model, currentPose.GetGlobalTransform(rightLeg->Ankle()));

    Vec3 worldLeftToe = combine(model, currentPose.GetGlobalTransform(leftLeg->Toe())).position;
    Vec3 leftToeTarget = worldLeftToe;
    Vec3 predictiveLeftToe = worldLeftToe;

    Vec3 worldRightToe = combine(model, currentPose.GetGlobalTransform(rightLeg->Toe())).position;
    Vec3 rightToeTarget = worldRightToe;
    Vec3 predictiveRightToe = worldRightToe;

    Vec3 origin = leftAnkleWorld.position;
    origin.y = worldLeftToe.y;
    Ray leftToeRay(origin + 
                   characterForward * toeLength + 
                   Vec3(0, 1, 0));
    origin = rightAnkleWorld.position;
    origin.y = worldRightToe.y;
    Ray rightToeRay(origin +
                    characterForward * toeLength +
                    Vec3(0, 1, 0));

    float ankleRayHeight = 1.1f;
    for (unsigned int i = 0; i < numTriangles; ++i)
    {
        // Left toe
        if (RaycastTriangle(leftToeRay, triangles[i], hitPoint)) {
            if (lenSq(hitPoint - leftToeRay.origin) < ankleRayHeight * ankleRayHeight) {
                leftToeTarget = hitPoint;
            }
            predictiveLeftToe = hitPoint;
        }
        
        // Right toe
        if (RaycastTriangle(rightToeRay, triangles[i], hitPoint)) {
            if (lenSq(hitPoint - rightToeRay.origin) < ankleRayHeight * ankleRayHeight) {
                rightToeTarget = hitPoint;
            }
            predictiveRightToe = hitPoint;
        }
    }

    // Place the toe target at the correct location
    leftToeTarget = lerp(leftToeTarget, predictiveLeftToe, leftMotion);
    rightToeTarget = lerp(rightToeTarget, predictiveRightToe, rightMotion);

    // If the left or right toe hit, adjust the ankle
    Vec3 leftAnkleToCurrentToe = worldLeftToe - leftAnkleWorld.position;
    Vec3 leftAnkleToDesiredToe = leftToeTarget - leftAnkleWorld.position;
    if (dot(leftAnkleToCurrentToe, leftAnkleToDesiredToe) > 0.00001f) {
        Quat ankleRotator = fromTo(leftAnkleToCurrentToe, leftAnkleToDesiredToe);
        Transform ankleLocal = currentPose.GetLocalTransform(leftLeg->Ankle());

        Quat worldRotatedAnkle = leftAnkleWorld.rotation * ankleRotator;
        Quat localRotatedAnkle = worldRotatedAnkle * inverse(leftAnkleWorld.rotation);

        ankleLocal.rotation = localRotatedAnkle * ankleLocal.rotation;
        currentPose.SetLocalTransform(leftLeg->Ankle(), ankleLocal);
    }

    Vec3 rightAnkleToCurrentToe = worldRightToe - rightAnkleWorld.position;
    Vec3 rightAnkleToDesiredToe = rightToeTarget - rightAnkleWorld.position;
    if (dot(rightAnkleToCurrentToe, rightAnkleToDesiredToe) > 0.00001f) {
        Quat ankleRotator = fromTo(rightAnkleToCurrentToe, rightAnkleToDesiredToe);
        Transform ankleLocal = currentPose.GetLocalTransform(rightLeg->Ankle());

        Quat worldRotatedAnkle = rightAnkleWorld.rotation * ankleRotator;
        Quat localRotatedAnkle = worldRotatedAnkle * inverse(rightAnkleWorld.rotation);

        ankleLocal.rotation = localRotatedAnkle * ankleLocal.rotation;
        currentPose.SetLocalTransform(rightLeg->Ankle(), ankleLocal);
    }

    // update matrix palette for skinning
    currentPose.GetMatrixPalette(posePalette);

#endif

}

void Sample::Render(float inAspectRatio)
{

#if (SAMPLE_1 || SAMPLE_2)
    Vec3 cameraPose(
        camDist * cosf(deg2rad(camYaw)) * sinf(deg2rad(camPitch)),
        camDist * cosf(deg2rad(camPitch)),
        camDist * sinf(deg2rad(camYaw)) * sinf(deg2rad(camPitch))
    );

    Mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    Mat4 view = lookAt(cameraPose, Vec3(0,0,0), Vec3(0,1,0));
    Mat4 mvp = projection * view; // no model

    solverLines->LinesFromIKSolver(solver);
    solverPoints->PointsFromIKSolver(solver);

    solverLines->UpdateOpenGLBuffers();
    solverPoints->UpdateOpenGLBuffers();

    solverLines->Draw(DebugDrawMode::Lines, Vec3(1,0,1), mvp);
    solverPoints->Draw(DebugDrawMode::Points, Vec3(1,0,1), mvp);

    (*targetVisual[0])[0] = target.position + Vec3(GIZMO_SIZE, 0, 0);
    (*targetVisual[1])[0] = target.position + Vec3(0, GIZMO_SIZE, 0);
    (*targetVisual[2])[0] = target.position + Vec3(0, 0, GIZMO_SIZE);
    (*targetVisual[0])[1] = target.position - Vec3(GIZMO_SIZE, 0, 0);
    (*targetVisual[1])[1] = target.position - Vec3(0, GIZMO_SIZE, 0);
    (*targetVisual[2])[1] = target.position - Vec3(0, 0, GIZMO_SIZE);
    
    targetVisual[0]->UpdateOpenGLBuffers();
    targetVisual[1]->UpdateOpenGLBuffers();
    targetVisual[2]->UpdateOpenGLBuffers();

    targetVisual[0]->Draw(DebugDrawMode::Lines, Vec3(1,0,0), mvp);
    targetVisual[1]->Draw(DebugDrawMode::Lines, Vec3(0,1,0), mvp);
    targetVisual[2]->Draw(DebugDrawMode::Lines, Vec3(0,0,1), mvp);
#endif

#if SAMPLE_3

    Mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    Mat4 view = lookAt(Vec3(model.position.x, 0, model.position.z) + Vec3(0, 5, 10),
                       Vec3(model.position.x, 0, model.position.z) + Vec3(0, 3, 0),
                       Vec3(0, 1, 0));
    Mat4 modelMat = transformToMat4(model);
    Mat4 mvp = projection * view * modelMat;
    Mat4 vp = projection * view;

    if (showMesh)
    {
        Shader* characterShader = skinnedShader;

        characterShader->Bind();
        Uniform<Mat4>::Set(characterShader->GetUniform("model"), modelMat);
        Uniform<Mat4>::Set(characterShader->GetUniform("view"), view);
        Uniform<Mat4>::Set(characterShader->GetUniform("projection"), projection);
        Uniform<Vec3>::Set(characterShader->GetUniform("light"), Vec3(1,1,1));
        Uniform<Mat4>::Set(characterShader->GetUniform("pose"), posePalette);
        Uniform<Mat4>::Set(characterShader->GetUniform("invBindPose"), skeleton.GetInvBindPose());

        diffuseTexture->Set(characterShader->GetUniform("tex0"), 0);
        for (unsigned int i = 0, size = (unsigned int)meshes.size(); i < size; ++i)
        {
            meshes[i].Bind(characterShader->GetAttribute("position"),
                           characterShader->GetAttribute("normal"),
                           characterShader->GetAttribute("texCoord"),
                           characterShader->GetAttribute("weights"),
                           characterShader->GetAttribute("joints"));
            meshes[i].Draw();
            meshes[i].Unbind(characterShader->GetAttribute("position"),
                             characterShader->GetAttribute("normal"),
                             characterShader->GetAttribute("texCoord"),
                             characterShader->GetAttribute("weights"),
                             characterShader->GetAttribute("joints"));
        }

        diffuseTexture->Unset(0);
        characterShader->UnBind();
    }

    if (showEnvironment)
    {
        Shader* environmentShader = staticShader;
        environmentShader->Bind();

        Uniform<Mat4>::Set(environmentShader->GetUniform("model"), Mat4());
        Uniform<Mat4>::Set(environmentShader->GetUniform("view"), view);
        Uniform<Mat4>::Set(environmentShader->GetUniform("projection"), projection);
        Uniform<Vec3>::Set(environmentShader->GetUniform("light"), Vec3(1, 1, 1));
        courseTexture->Set(environmentShader->GetUniform("tex0"), 0);
        for (unsigned int i = 0, size = (unsigned int)IKCourse.size(); i < size; ++i) {\
            IKCourse[i].Bind(environmentShader->GetAttribute("position"),
                             environmentShader->GetAttribute("normal"),
                             environmentShader->GetAttribute("texCoord"),
                             -1,
                             -1);
            IKCourse[i].Draw();
            IKCourse[i].Unbind(environmentShader->GetAttribute("position"),
                             environmentShader->GetAttribute("normal"),
                             environmentShader->GetAttribute("texCoord"),
                             -1,
                             -1);
        }
        courseTexture->Unset(0);
        environmentShader->UnBind();
    }

    if (!depthTest) {
        glDisable(GL_DEPTH_TEST);
    }

    if (showCurrentPose) {
        currentPoseVisual->UpdateOpenGLBuffers();
        currentPoseVisual->Draw(DebugDrawMode::Lines, Vec3(0, 0, 1), mvp);
    }

    if (showIKPose) {
        leftLeg->Draw(vp, Vec3(1, 0, 0));
        rightLeg->Draw(vp, Vec3(0, 1, 0));
    }

    if (!depthTest) {
        glEnable(GL_DEPTH_TEST);
    }

#endif

}

void Sample::Shutdown()
{
    Application::Shutdown();
    
#if (SAMPLE_1 || SAMPLE_2)
    delete solverLines;
    delete solverPoints;
    delete targetVisual[0];
    delete targetVisual[1];
    delete targetVisual[2];
#endif

#if SAMPLE_3
    delete currentPoseVisual;
    delete staticShader;
    delete diffuseTexture;
    delete skinnedShader;
    delete courseTexture;
    delete leftLeg;
    delete rightLeg;
    clips.clear();
    meshes.clear();
#endif
}

