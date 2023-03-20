#include "SceneManager.h"
#include <PhysicsBody.h>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <NodeMesh3D.h>

float SceneManager::deltaTime = 0.0f;

SceneManager::SceneManager(Node* root)
{
    sceneRoot = root;
}

SceneManager::~SceneManager()
{
    //dtor
}

void applyTransformsRecursive(Node* object)
{
    if (object->type > 0)
    {
        Node3D* node = static_cast<Node3D*>(object);
        if (node->root == false)
            node->applyGlobalTransform();
    }
    object->unsetMoved();
    for (unsigned int i = 0; i < object->children.size(); i++)
    {
            applyTransformsRecursive(object->children[i]);
    }
}

void SceneManager::applyPhysics(float deltaTime)
{
    SceneManager::setDeltaTime(deltaTime);
    int dynamicBodiesCount = dynamicBodies.size();
    for (int i = 0; i < dynamicBodiesCount; i++)
    {
        int index = dynamicBodies[i];
        physBodies[index]->doMovement(deltaTime);
    }
}

void SceneManager::applyTransforms()
{
    applyTransformsRecursive(sceneRoot);
}

void SceneManager::registerPhysicsNode(PhysicsBody* node)
{
    std::cout << "Physics body: \"" << node->name << "\" registered\n";
    physBodies.push_back(node);
    if (node->getBodyType() == PHYS_BODY_KINEMATIC)
    {
        std::cout << "Registered as dynamic\n";
        registerDynamicBody(physBodies.size() - 1);
    }
}

void SceneManager::registerDynamicBody(int id)
{
    dynamicBodies.push_back(id);
}

void SceneManager::unregisterPhysicsNode(PhysicsBody* node)
{
    std::vector<PhysicsBody*>::iterator bodyPos = std::find(physBodies.begin(), physBodies.end(), node);
    physBodies.erase(bodyPos);
    if (node->getBodyType() == PHYS_BODY_KINEMATIC)
    {
        std::vector<int>::iterator dynamicPos = std::find(dynamicBodies.begin(), dynamicBodies.end(), std::distance(physBodies.begin(), bodyPos));
        dynamicBodies.erase(dynamicPos);
    }
}

std::vector<CollisionShape*> SceneManager::getNearbyColliders(PhysicsBody* body)
{
    std::vector<CollisionShape*> nearbyColliders;
    AABB thisAABB = body->getAABB();
    unsigned int bodyCount = physBodies.size();
    for (unsigned int i = 0; i < bodyCount; i++)
    {
        if (physBodies[i] != body && thisAABB.AABBtoAABBtest(physBodies[i]->getAABB()))
        {
            //std::cout << physBodies[i]->name << "\n";
            std::vector<CollisionShape*> found;
            for (unsigned int j = 0; j < physBodies[i]->getCollisionShapes().size(); j++)
            {
                CollisionShape* currentShape = physBodies[i]->getCollisionShapes()[j];
                if (thisAABB.AABBtoAABBtest(currentShape->getAABB()))
                    found.push_back(currentShape);
            }
            nearbyColliders.insert(nearbyColliders.end(), found.begin(), found.end());
        }
    }
    return nearbyColliders;
}

enum e_CreateCommands {C_CREATE_MESH3D, C_CREATE_MATERIAL, C_CREATE_TEXTURE, C_CREATE_COLLIDER, C_CREATE_PHYS_NODE, C_CREATE_NODE3D, C_CREATE_NODE_MESH3D};
const std::unordered_map<std::string, unsigned int> CreateCommandMap = {
                                                                  {"@Mesh3D", C_CREATE_MESH3D},
                                                                  {"@Material", C_CREATE_MATERIAL},
                                                                  {"@Texture", C_CREATE_TEXTURE},
                                                                  {"@CollisionShape", C_CREATE_COLLIDER},
                                                                  {"@PhysicsBody", C_CREATE_PHYS_NODE},
                                                                  {"@Node3D", C_CREATE_NODE3D},
                                                                  {"@NodeMesh3D", C_CREATE_NODE_MESH3D},
                                                                  {"@Mesh3D", C_CREATE_MESH3D}
                                                                  };

enum e_ObjectCommands {OC_TRANSLATE, OC_ROTATE, OC_SCALE, OC_ADD_CHILD, OC_SET_AS_ROOT};

const std::unordered_map<std::string, unsigned int> objectCommandMap =  {
                                                                        {"translate", OC_TRANSLATE},
                                                                        {"rotate", OC_ROTATE},
                                                                        {"scale", OC_SCALE},
                                                                        {"addChild", OC_ADD_CHILD},
                                                                        {"setAsRoot", OC_SET_AS_ROOT}
                                                                        };

int SceneManager::createMesh3D(std::string& name, const std::string& path)
{
    auto fname = std::find(usedNames.begin(), usedNames.end(), name);
    if (fname != usedNames.end())
    {
        name += "_c";
    }

    usedNames.push_back(name);

    auto fpath = resourcePaths.find(path);
    if (fpath != resourcePaths.end())
    {
        aliases[name] = fpath->second;
        return 0;
    }

    Mesh3D* msh = new Mesh3D(path);
    meshes[name] = msh;
    resourcePaths[path] = name;
    aliases[name] = name;
    return 0;
}

int SceneManager::createTexture(std::string& name, const std::string& path, const int& anisoLevel)
{
    auto fname = std::find(usedNames.begin(), usedNames.end(), name);
    if (fname != usedNames.end())
    {
        name += "_c";
    }

    usedNames.push_back(name);

    auto fpath = resourcePaths.find(path);
    if (fpath != resourcePaths.end())
    {
        aliases[name] = fpath->second;
        return 0;
    }

    Texture* tex = new Texture(path, anisoLevel);
    textures[name] = tex;
    resourcePaths[path] = name;
    aliases[name] = name;
    return 0;
}

int SceneManager::createMaterial(const int& mode, std::string& name, const std::string& albedoName, const std::string& normalName, const std::string& ormName, const glm::vec4& color)
{
    auto fname = std::find(usedNames.begin(), usedNames.end(), name);
    if (fname != usedNames.end())
    {
        name += "_c";
    }

    usedNames.push_back(name);

    std::string path = std::to_string(color.x) + std::to_string(color.y) + std::to_string(color.z) + std::to_string(color.w) +
                        albedoName + normalName + ormName;

    auto fpath = resourcePaths.find(path);
    if (fpath != resourcePaths.end())
    {
        aliases[name] = fpath->second;
        return 0;
    }


    Texture* albedo;
    Texture* normal;
    Texture* orm;

    if (albedoName != "")
    {
        auto aliasNameAlb = aliases.find(albedoName);
        if (aliasNameAlb == aliases.end())
        {
            std::cerr<< "Could not find texture " << albedoName << "\n";
            return -1;
        }
        albedo = textures[aliasNameAlb->second];
    }
    if (normalName != "")
    {
        auto aliasNameNorm = aliases.find(normalName);
        if (aliasNameNorm == aliases.end())
        {
            std::cerr<< "Could not find texture " << normalName << "\n";
            return -1;
        }
        normal = textures[aliasNameNorm->second];
    }

    if (ormName != "")
    {
        auto aliasNameOrm = aliases.find(ormName);
        if (aliasNameOrm == aliases.end())
        {
            std::cerr<< "Could not find texture " << ormName << "\n";
            return -1;
        }
        orm = textures[aliasNameOrm->second];
    }


    Material* nMat;
    switch (mode)
    {
    case 0:
        nMat = new Material(color);
        break;
    case 1:
        nMat = new Material(albedo);
        break;
    case 2:
        nMat = new Material(albedo, normal);
        break;
    case 3:
        nMat = new Material(albedo, normal, orm);
        break;
    }
    materials[name] = nMat;
    resourcePaths[path] = name;
    aliases[name] = name;
    return 0;
}

int SceneManager::CreateNode3D(std::string& name, std::unordered_map<std::string, Node*>& nodes)
{
    auto fname = nodes.find(name);
    auto frsName = std::find(usedNames.begin(), usedNames.end(), name);
    if (frsName != usedNames.end())
    {
        std::cerr << "Node name \""<< name << "\" is already in use\n";
        return -1;
    }

    if (fname != nodes.end())
    {
        std::cerr << "Node name \""<< name << "\" is already in use\n";
        return -1;
    }

    Node3D* node = new Node3D(name);
    nodes[name] = static_cast<Node*>(node);
    return 0;
}

int SceneManager::CreateNodeMesh3D(std::string& name, const std::string& meshKey, const std::string& materialKey, std::unordered_map<std::string, Node*>& nodes)
{
    auto fname = nodes.find(name);
    auto frsName = std::find(usedNames.begin(), usedNames.end(), name);
    if (frsName != usedNames.end())
    {
        std::cerr << "Node name \""<< name << "\" is already in use\n";
        return -1;
    }

    if (fname != nodes.end())
    {
        std::cerr << "Node name \""<< name << "\" is already in use\n";
        return -1;
    }

    Mesh3D* mesh;
    Material* material;

    auto meshAlias = aliases.find(meshKey);
    if (meshAlias == aliases.end())
    {
        std::cerr << "Could not find mesh \"" << meshKey << "\"\n";
        return -1;
    }
    mesh = meshes[meshAlias->second];

    auto materialAlias = aliases.find(materialKey);
    if (materialAlias == aliases.end())
    {
        std::cerr << "Could not find material \"" << materialKey << "\"\n";
        return -1;
    }
    material = materials[materialAlias->second];

    NodeMesh3D* node = new NodeMesh3D(name, mesh, material);
    nodes[name] = static_cast<Node*>(node);
    return 0;
}

int SceneManager::CreateCollider(std::string& name, const std::string& meshKey, std::unordered_map<std::string, Node*>& nodes)
{
    auto fname = nodes.find(name);
    auto frsName = std::find(usedNames.begin(), usedNames.end(), name);
    if (frsName != usedNames.end())
    {
        std::cerr << "Node name \""<< name << "\" is already in use\n";
        return -1;
    }

    if (fname != nodes.end())
    {
        std::cerr << "Node name \""<< name << "\" is already in use\n";
        return -1;
    }

    auto meshAlias = aliases.find(meshKey);
    if (meshAlias == aliases.end())
    {
        std::cerr << "Could not find mesh \"" << meshKey << "\"\n";
        return -1;
    }
    Mesh3D* msh = meshes[meshAlias->second];

    CollisionShape* col = new CollisionShape(name);
    col->setCollisionType(COLLISION_TRIANGLE);
    col->setMesh(msh);
    nodes[name] = static_cast<Node*>(col);
    return 0;
}

int SceneManager::CreatePhysNode(std::string& name, std::string& pType, std::unordered_map<std::string, Node*>& nodes)
{
    auto fname = nodes.find(name);
    auto frsName = std::find(usedNames.begin(), usedNames.end(), name);
    if (frsName != usedNames.end())
    {
        std::cerr << "Node name \""<< name << "\" is already in use\n";
        return -1;
    }

    if (fname != nodes.end())
    {
        std::cerr << "Node name \""<< name << "\" is already in use\n";
        return -1;
    }

    int type;
    if (pType == "kinematic")
    {
        type = PHYS_BODY_KINEMATIC;
    }
    else if (pType == "static")
    {
        type = PHYS_BODY_STATIC;
    }
    else
    {
        std::cerr << "Physics types can only be \"kinematic\" or \"static\"\n";
        return -1;
    }

    PhysicsBody* Pb = new PhysicsBody(name, type);
    nodes[name] = Pb;
    return 0;
}

typedef struct
{
    int state = 1;
    glm::vec3 vec;
} st_v3;

st_v3 strToV3(std::string st)
{
    std::vector<float> components;
    std::stringstream cmpSs(st);
    std::string tok;
    while (std::getline(cmpSs, tok, ','))
    {
        components.push_back(std::stof(tok));
    }
     st_v3 vec;
     vec.state = -1;
     vec.vec = glm::vec3(0.0f);
    if (components.size() != 3)
    {
        std::cerr<< "Malformed vector\n";
        return vec;
    }

    vec.state = 0;
    vec.vec = glm::vec3(components[0], components[1], components[2]);
    return vec;
}

Node* SceneManager::loadSceneFromFile(std::string filePath)
{
    std::string baseDir = "./";
    std::fstream file(filePath);
    std::unordered_map<std::string, Node*> nodes;
    //                 name        node

    if (!file.is_open())
    {
        std::cerr << "Could not open scene file\n";
        return nullptr;
    }
    Node* root = nullptr;

    std::string line;
    unsigned int currentLine = 0;
    while (std::getline(file, line))
    {
        currentLine++;
        std::stringstream lineStream(line);
        std::vector<std::string> tokens;
        std::string ctok;
        while (std::getline(lineStream, ctok, ' '))
        {
            tokens.push_back(ctok);
        }

        const unsigned int numTokens = tokens.size();

        if (numTokens == 0)
            continue;

        if (std::isalpha(tokens[0][0]))//Object function
        {
            bool found = false;
            auto cNodeEntry = nodes.find(tokens[0]);
            if (cNodeEntry != nodes.end())
            {
                found = true;
                Node* currentNode = cNodeEntry->second;

                auto commandEntry = objectCommandMap.find(tokens[1]);
                if (commandEntry == objectCommandMap.end())
                {
                    std::cerr << "Cound not find command " << tokens[1] << " at line" << currentLine << "\n";
                    continue;
                }

                unsigned int command = commandEntry->second;

                switch (command)
                {
                case OC_TRANSLATE:
                    {
                        if (tokens.size() != 3)
                        {
                            std::cerr << "Incorrect number of arguments on line " << currentLine << "\n";
                            break;
                        }
                        st_v3 trsRes = strToV3(tokens[2]);
                        if (trsRes.state == -1)
                        {
                            break;
                        }

                        if (currentNode->type < NODE_TYPE_NODE_3D)
                        {
                            std::cerr << "Command \"" << tokens[1] << "\" Incompatible with node type at line " << currentLine << "\n";
                            break;
                        }
                        Node3D* n3d = static_cast<Node3D*>(currentNode);

                        n3d->translate(trsRes.vec);

                        break;
                    }
                case OC_ROTATE:
                    {
                        if (tokens.size() == 4)
                        {
                            if (currentNode->type < NODE_TYPE_NODE_3D)
                            {
                                std::cerr << "Command \"" << tokens[1] << "\" Incompatible with node type at line " << currentLine << "\n";
                                break;
                            }
                            Node3D* n3d = static_cast<Node3D*>(currentNode);

                            if (std::tolower(tokens[2][0]) == 'x')
                            {
                                n3d->rotateGlobalX(std::stof(tokens[3]));
                            }
                            else if (std::tolower(tokens[2][0]) == 'y')
                            {
                                n3d->rotateGlobalY(std::stof(tokens[3]));
                            }
                            else if (std::tolower(tokens[2][0]) == 'z')
                            {
                                n3d->rotateGlobalZ(std::stof(tokens[3]));
                            }
                            else
                            {
                                std::cerr << "Invalid axis for rotation on line " << currentLine << "\n";
                                break;
                            }
                        }
                        else
                        {
                            std::cerr << "Incorrect number of arguments on line " << currentLine << "\n";
                            break;
                        }
                        break;
                    }
                case OC_SCALE:
                    {
                        break;
                    }
                case OC_ADD_CHILD:
                    {
                        if (tokens.size() != 3)
                        {
                            std::cerr << "Incorrect number of arguments on line " << currentLine << "\n";
                            break;
                        }
                        auto otherNodeEntry = nodes.find(tokens[2]);
                        if (otherNodeEntry == nodes.end())
                        {
                            std::cerr << "Could not find node \"" << tokens[2] << "\"\n";
                            break;
                        }

                        Node* otherNode = otherNodeEntry->second;
                        currentNode->addChild(otherNode);
                        break;
                    }
                case OC_SET_AS_ROOT:
                    {
                        root = currentNode;
                        break;
                    }
                }

                continue;
            }

            auto matAlias = aliases.find(tokens[0]);
            if (matAlias != aliases.end())
            {
                auto matEntry = materials.find(matAlias->second);
                if (matEntry != materials.end())
                {
                    found = true;
                    Material* currentMat = matEntry->second;
                }
            }
            if (!found)
            {
                std::cerr << "Cound not find node or resource \"" << tokens[0] << "\" at line" << currentLine << "\n";
            }
        }
        else if (tokens[0][0] == '@')//Create functions
        {
            auto command = CreateCommandMap.find(tokens[0]);
            if (command == CreateCommandMap.end())
            {
                std::cerr << "Cound not find command " << tokens[0] << " at line" << currentLine << "\n";
                break;
            }

            unsigned int aCommand = command->second;

            switch (aCommand)
            {
                case C_CREATE_MESH3D:
                    {
                        if (tokens.size() != 3)
                        {
                            std::cerr << "Not enough arguments at line " << currentLine << "\n";
                            break;
                        }
                        createMesh3D(tokens[1], baseDir + tokens[2]);
                        break;
                    }
                case C_CREATE_TEXTURE:
                    {
                        if (tokens.size() < 3)
                        {
                            std::cerr << "Not enough arguments at line " << currentLine << "\n";
                            break;
                        }
                        if (tokens.size() > 4)
                        {
                            std::cerr << "Too many arguments at line " << currentLine << "\n";
                            break;
                        }

                        if (tokens.size() == 4)
                            createTexture(tokens[1], baseDir + tokens[2], std::stoi(tokens[3]));
                        else
                            createTexture(tokens[1], baseDir + tokens[2]);
                        break;
                    }
                case C_CREATE_MATERIAL:
                    {
                        switch (tokens.size())
                        {
                        case 1:
                        case 2:
                            std::cerr << "Not enough arguments at line " << currentLine << "\n";
                            break;
                        case 3:
                            {
                                glm::vec4 col;
                                if (std::isdigit(tokens[2][0]))
                                {
                                    std::stringstream clr(tokens[2]);
                                    std::string channel;
                                    std::vector<float> channels;
                                    while (std::getline(clr, channel, ','))
                                    {
                                        channels.push_back(std::stof(channel));
                                    }
                                    if (channels.size() == 4)
                                    {
                                        col = glm::vec4(channels[0], channels[1], channels[2], channels[3]);
                                        createMaterial(0, tokens[1], "", "", "", col);
                                    }
                                    else
                                    {
                                        std::cerr << "Malformed color string at line " << currentLine << "\n";
                                    }
                                }
                                else
                                {
                                    createMaterial(1, tokens[1], tokens[2]);
                                }
                                break;
                            }
                        case 4:
                            {
                                createMaterial(2, tokens[1], tokens[2], tokens[3]);
                                break;
                            }
                        case 5:
                            {
                                createMaterial(3, tokens[1], tokens[2], tokens[3], tokens[4]);
                                break;
                            }
                            break;

                        }
                    }
                case C_CREATE_NODE3D:
                    {
                        CreateNode3D(tokens[1], nodes);
                        break;
                    }
                case C_CREATE_NODE_MESH3D:
                    {
                        if (tokens.size() == 4)
                            CreateNodeMesh3D(tokens[1], tokens[2], tokens[3], nodes);
                        else
                            std::cerr << "Not enough arguments on line " << currentLine << "\n";
                        break;
                    }
                case C_CREATE_COLLIDER:
                    {
                        if (tokens.size() < 3)
                        {
                            std::cerr << "Not enough arguments on line " << currentLine << "\n";
                            break;
                        }
                        if (tokens.size() > 3)
                        {
                            std::cerr << "Too many arguments on line " << currentLine << "\n";
                            break;
                        }
                        CreateCollider(tokens[1], tokens[2], nodes);
                        break;
                    }
                case C_CREATE_PHYS_NODE:
                    {
                        if (tokens.size() < 3)
                        {
                            std::cerr << "Not enough arguments on line " << currentLine << "\n";
                            break;
                        }
                        if (tokens.size() > 3)
                        {
                            std::cerr << "Too many arguments on line " << currentLine << "\n";
                            break;
                        }
                        CreatePhysNode(tokens[1], tokens[2], nodes);
                        break;
                    }
            }
        }
        else if (tokens[0][0] == '$')
        {
            if (tokens.size() == 2)
            {
                baseDir = tokens[1];
                std::cout << "Set base dir as " << tokens[1] << "\n";
            }
        }
    }

    if (root == nullptr)
    {
        std::cerr << "Scene root not set\n";
    }
    return root;
}

