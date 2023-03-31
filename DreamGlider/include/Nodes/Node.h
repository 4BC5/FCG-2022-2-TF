#ifndef NODE_H
#define NODE_H
#include <stdlib.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <matrices.h>
#include <string>
enum e_NodeType {NODE_TYPE_NODE = 0, NODE_TYPE_NODE_3D = 1, NODE_TYPE_MESH_3D = 2, NODE_TYPE_PHYSICS_BODY = 3, NODE_TYPE_COLLISION_SHAPE = 4, NODE_TYPE_TRIGGER_VOLUME = 5, NODE_TYPE_POINT_LIGHT = 6};

class SceneManager;

// Essa é a classe base de nodes
// Ela tem o básico que todos os node precisam
// Ela está mal organizada porque foi criada no começo do projeto e nunca foi refatorada

class Node
{
    public:
        Node(std::string name);
        virtual ~Node();

        int type = NODE_TYPE_NODE;//O tipo especializado de node que é o node atual

        std::string name = "Node";//O nome do node
        bool root = false;//Se o node é a raiz da cena
        Node* parent = nullptr;//O pai do node
        std::vector<Node*> children;//Os filhos do node
        bool visible = true;//Se o node é visivel para renderização. A visibilidade se propaga, então é necessário que todos os nodes tenham

        virtual Node* removeChildAtIndex(std::vector<Node*>::iterator childIndex);//Remover filhos no indice
        Node* removeChild(Node* child);//Remover filho por ponteiro
        virtual void addChild(Node* newChild);//Adicionar filho
        void queueFree();//Entrar no queue de deletamento (evita conflitos entre threads)
        std::vector<Node*>::iterator getChildIndex(Node* child);//Pegar indice de filho

        Node* getParent();//Pegar o pai do node
        std::vector<Node*> getChildren();//Pegar os filhos do node

        virtual void onSceneSetup();//Chamado quando a cena começa
        bool receiveTick = false;//Se o node recebe ticks
        virtual void onTick(float deltaTime){};//onTick é chamado cada frame de física, caso receiveTick esteja habilitado
        //Filhos podem implementar seu próprio onTick

        virtual void onReceiveMessage(std::string message, std::string argumentType, void* valuePtr){}//Função para nodes passarem menságens um pro outro sem saber o tipo especifico

        //Node 3D methods
        virtual glm::mat4 getGlobalTransform()//O transform é propagado pela árvore, então deve existir no node base
        {
            if (!root)
                return parent->getGlobalTransform();
            else
                return mop::Matrix_Identity();
        }

        void unsetMoved(){moved = false;}
        static SceneManager* sceneManager;
        static void setSceneManager(SceneManager* snMngr);

    protected:
        bool moved = true;//Se o node foi movido. Serve para fazer update da AABB
        void setMoved();

    private:
};


#endif // NODE_H
