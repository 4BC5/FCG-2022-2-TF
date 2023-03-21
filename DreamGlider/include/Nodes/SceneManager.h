#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <Nodes/Node3D.h>
//#include <CollisionShape.h>
#include <vector>
#include <unordered_map>
#include <Mesh3D.h>
#include <Texture.h>
#include <Material.h>

//#include <PhysicsBody.h>

class CollisionShape;
class PhysicsBody;
class TriggerVolume;

class SceneManager
{
    public:
        SceneManager(Node* root);
        virtual ~SceneManager();

        Node* sceneRoot = nullptr;

        void applyTransforms();
        void applyPhysics(float deltaTime);
        std::vector<CollisionShape*> getNearbyColliders(PhysicsBody* body);
        std::vector<TriggerVolume*> getNearbyTriggers(AABB& aabb);
        void registerPhysicsNode(PhysicsBody* node);
        void registerTrigger(TriggerVolume* node);
        void unregisterPhysicsNode(PhysicsBody* node);
        void unregisterTrigger(TriggerVolume* node);
        static float getDeltaTime(){return deltaTime;}

        Node* loadSceneFromFile(std::string filePath);

    protected:
        static void setDeltaTime(float dt){deltaTime = dt;}
        static float deltaTime;

        std::vector<std::string> usedNames = {};

        std::unordered_map<std::string, std::string> resourcePaths;
        //                 path         key

        std::unordered_map<std::string, Mesh3D*> meshes;
        //                 key          value

        std::unordered_map<std::string, Material*> materials;
        //                 key          value

        std::unordered_map<std::string, Texture*> textures;
        //                 key          value

        std::unordered_map<std::string, std::string> aliases;
        //                 name         key

    private:
        std::vector<PhysicsBody*> physBodies;
        std::vector<CollisionShape*> collisionShapes;
        std::vector<int> dynamicBodies;
        std::vector<TriggerVolume*> triggers;

        int createMesh3D(std::string& name, const std::string& path);
        int createTexture(std::string& name, const std::string& texturePath, const int& anisoLevel = 1);
        int createMaterial(const int& mode, std::string& name, const std::string& albedoName = "", const std::string& normalName = "", const std::string& ormName = "",  const glm::vec4& color = glm::vec4(1.0));

        int CreateNode3D(std::string& name, std::unordered_map<std::string, Node*>& nodes);
        int CreateNodeMesh3D(std::string& name, const std::string& meshKey, const std::string& materialKey, std::unordered_map<std::string, Node*>& nodes);
        int CreateCollider(std::string& name, const std::string& meshKey, std::unordered_map<std::string, Node*>& nodes);
        int CreatePhysNode(std::string& name, std::string& pType, std::unordered_map<std::string, Node*>& nodes);

        void registerDynamicBody(int id);
};

#endif // SCENEMANAGER_H
