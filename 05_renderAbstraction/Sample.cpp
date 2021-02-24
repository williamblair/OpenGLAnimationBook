#include <Sample.h>
#include <vector>
    
#define DEG2RAD 0.0174533f

bool Sample::Initialize(const char* title, const int width, const int height)
{
    shader = nullptr;
    displayTexture = nullptr;
    vertexPositions = nullptr;
    vertexNormals = nullptr;
    vertexTexCoords = nullptr;
    indexBuffer = nullptr;
    
    if (!Application::Initialize(title, width, height)) {
        return false;
    }

    rotation = 0.0f;
    shader = new Shader("shaders/static.vert", "shaders/lit.frag");
    displayTexture = new Texture("assets/uv.png");

    vertexPositions = new Attribute<Vec3>();
    vertexNormals   = new Attribute<Vec3>();
    vertexTexCoords = new Attribute<Vec2>();
    indexBuffer     = new IndexBuffer();

    // flat quad
    std::vector<Vec3> positions;
    positions.push_back(Vec3(-1, -1, 0));
    positions.push_back(Vec3(-1,  1, 0));
    positions.push_back(Vec3( 1, -1, 0));
    positions.push_back(Vec3( 1,  1, 0));
    vertexPositions->Set(positions);

    // normals
    std::vector<Vec3> normals;
    normals.resize(4, Vec3(0, 0, 1));
    vertexNormals->Set(normals);

    // flat quad texture coords
    std::vector<Vec2> uvs;
    uvs.push_back(Vec2(0,0));
    uvs.push_back(Vec2(0,1));
    uvs.push_back(Vec2(1,0));
    uvs.push_back(Vec2(1,1));
    vertexTexCoords->Set(uvs);

    // face indices - two triangles
    std::vector<unsigned int> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(3);
    indexBuffer->Set(indices);

    return true;
}

void Sample::Update(float inDeltaTime)
{
    rotation += inDeltaTime * 45.0f;
    while (rotation >= 360.0f) {
        rotation -= 360.0f;
    }

    Application::Update(inDeltaTime);
}

void Sample::Render(float inAspectRatio)
{
    // args - fov, aspect, near clip, far clip
    Mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
    // args - eye position, eye target, up
    Mat4 view = lookAt(Vec3(0, 0, -5), Vec3(0, 0, 0), Vec3(0, 1, 0));
    // rotation about the Z axis
    Mat4 model = quatToMat4(angleAxis(rotation * DEG2RAD, Vec3(0, 0, 1)));

    shader->Bind();

    vertexPositions->BindTo(shader->GetAttribute("position"));
    vertexNormals->BindTo(shader->GetAttribute("normal"));
    vertexTexCoords->BindTo(shader->GetAttribute("texCoord"));

    // transformation matrices
    Uniform<Mat4>::Set(shader->GetUniform("model"), model);
    Uniform<Mat4>::Set(shader->GetUniform("view"), view);
    Uniform<Mat4>::Set(shader->GetUniform("projection"), projection);

    // light position
    Uniform<Vec3>::Set(shader->GetUniform("light"), Vec3(0, 0, 1));

    // Draw with GL texture 0
    displayTexture->Set(shader->GetUniform("tex0"), 0);
        Draw(*indexBuffer, DrawMode::Triangles);
    displayTexture->Unset(0); // unbind GL texture 0

    vertexPositions->UnbindFrom(shader->GetAttribute("position"));
    vertexNormals->UnbindFrom(shader->GetAttribute("normal"));
    vertexTexCoords->UnbindFrom(shader->GetAttribute("texCoord"));

    shader->UnBind();
}

void Sample::Shutdown()
{
    delete shader;
    delete displayTexture;
    delete vertexPositions;
    delete vertexNormals;
    delete vertexTexCoords;
    delete indexBuffer;

    Application::Shutdown();
}

