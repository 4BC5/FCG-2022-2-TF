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
class PointLight;
class NodeMesh3D;


//O node que gerencia a cena
//Ele aplica física, acha nodes próximos um do outro, entre outros
class SceneManager
{
    public:
        SceneManager(Node* root);
        virtual ~SceneManager();

        Node* sceneRoot = nullptr;

        void applyTransforms();//Aplica o transform dos Node3D, atualizando suas matrizes globais
        void applyPhysics(float deltaTime);//Aplica física com delta time
        std::vector<CollisionShape*> getNearbyColliders(PhysicsBody* body);//Acha os colliders próximos ao corpo "body"
        std::vector<TriggerVolume*> getNearbyTriggers(AABB& aabb);//Acha os triggers perto da AABB providenciada
        void registerPhysicsNode(PhysicsBody* node);//Registra um physics node (é chamado no construtor de PhysicsBody)
        void registerTrigger(TriggerVolume* node);//Registra um trigger volume (é chamado no construtor de TriggerVolume)
        void registerPointLight(PointLight* node);//Registra uma point light (é chamado no construtor de PointLight)

        void unregisterPhysicsNode(PhysicsBody* node);//Desregistra um physics node em sua destruição
        void unregisterTrigger(TriggerVolume* node);//Desregistra um trigger em sua destruição
        void unregisterPointLight(PointLight* node);//Desregistra uma point light em sua destruição

        static float getDeltaTime(){return deltaTime;}//Modo de nodes pegarem delta time fora do applyPhysics

        std::vector<PointLight*> getNearbyPointLights(NodeMesh3D* node);//Acha as point lights perto de um NodeMesh3D

        Node* loadSceneFromFile(std::string filePath, int depth = 0);//Carrega cena de um arquivo

    protected:
        static void setDeltaTime(float dt){deltaTime = dt;}
        static float deltaTime;

        std::vector<std::string> usedNames = {};

        std::unordered_map<std::string, std::string> resourcePaths;//Caminho dos recursos carregados (evita carregar o mesmo recurso multiplas vezes em cenas)
        //                 path         key

        std::unordered_map<std::string, Mesh3D*> meshes;//Mesh3D carregadas
        //                 key          value

        std::unordered_map<std::string, Material*> materials;//Materiais carregados (apenas é redundante se tem mesmo nome)
        //                 key          value

        std::unordered_map<std::string, Texture*> textures;//Texturas carregadas
        //                 key          value

        std::unordered_map<std::string, std::string> aliases;//Pseudonimos dos resources carregados
        //                 name         key

    private:
        std::vector<PhysicsBody*> physBodies;//Lista de PhysicsBody carregados
        std::vector<CollisionShape*> collisionShapes;//Lista de CollisionShape carregados
        std::vector<int> dynamicBodies;//Lista de indices dos PhysicsBody dinâmicos, que vão receber update de física
        std::vector<TriggerVolume*> triggers;//Triggers carregados
        std::vector<PointLight*> pointLights;//Point lights carregados


        //Criação de resources para o carregamento de cenas
        int createMesh3D(std::string& name, const std::string& path);
        int createTexture(std::string& name, const std::string& texturePath, const int& anisoLevel = 1);
        int createMaterial(const int& mode, std::string& name, const std::string& albedoName = "", const std::string& normalName = "", const std::string& ormName = "",  const glm::vec4& color = glm::vec4(1.0), bool createUnique = false);


        //Criação de nodes para carregamento de cenas
        int CreateNode3D(std::string& name, std::unordered_map<std::string, Node*>& nodes);
        int CreateNodeMesh3D(std::string& name, const std::string& meshKey, const std::string& materialKey, std::unordered_map<std::string, Node*>& nodes);
        int CreateCollider(std::string& name, const std::string& meshKey, std::unordered_map<std::string, Node*>& nodes);
        int CreatePhysNode(std::string& name, std::string& pType, std::unordered_map<std::string, Node*>& nodes);

        void registerDynamicBody(int id);
};

#endif // SCENEMANAGER_H
