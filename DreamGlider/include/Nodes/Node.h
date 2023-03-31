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

// Essa � a classe base de nodes
// Ela tem o b�sico que todos os node precisam
// Ela est� mal organizada porque foi criada no come�o do projeto e nunca foi refatorada

class Node
{
    public:
        Node(std::string name);
        virtual ~Node();

        int type = NODE_TYPE_NODE;//O tipo especializado de node que � o node atual

        std::string name = "Node";//O nome do node
        bool root = false;//Se o node � a raiz da cena
        Node* parent = nullptr;//O pai do node
        std::vector<Node*> children;//Os filhos do node
        bool visible = true;//Se o node � visivel para renderiza��o. A visibilidade se propaga, ent�o � necess�rio que todos os nodes tenham

        virtual Node* removeChildAtIndex(std::vector<Node*>::iterator childIndex);//Remover filhos no indice
        Node* removeChild(Node* child);//Remover filho por ponteiro
        virtual void addChild(Node* newChild);//Adicionar filho
        void queueFree();//Entrar no queue de deletamento (evita conflitos entre threads)
        std::vector<Node*>::iterator getChildIndex(Node* child);//Pegar indice de filho

        Node* getParent();//Pegar o pai do node
        std::vector<Node*> getChildren();//Pegar os filhos do node

        virtual void onSceneSetup();//Chamado quando a cena come�a
        bool receiveTick = false;//Se o node recebe ticks
        virtual void onTick(float deltaTime){};//onTick � chamado cada frame de f�sica, caso receiveTick esteja habilitado
        //Filhos podem implementar seu pr�prio onTick

        virtual void onReceiveMessage(std::string message, std::string argumentType, void* valuePtr){}//Fun��o para nodes passarem mens�gens um pro outro sem saber o tipo especifico

        //Node 3D methods
        virtual glm::mat4 getGlobalTransform()//O transform � propagado pela �rvore, ent�o deve existir no node base
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
