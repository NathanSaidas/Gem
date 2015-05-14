#ifndef GEM_GAME_OBJECT_TREE_H
#define GEM_GAME_OBJECT_TREE_H

#include <vector>

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Math/Math.h"
#include "Bounds.h"

namespace Gem
{
    class GameObject;

	class GameObjectTree;
	template class GEM_API Reflection::MetaObject<GameObjectTree>;

    class GEM_API GameObjectTree : public object
    {
        RDECLARE_CLASS(GameObjectTree)
    public:
        GameObjectTree();
        ~GameObjectTree();
    
    
    
    private:
        
        class Node
        {
        public:
            Node(GameObject * aGameObject);
            ~Node();
            
            Vector3 GetPosition();
            Bounds GetBounds();

            void Split();
            void Collapse();

            void AddNode(Node * aNode);
            void RemoveNode(Node * aNode);

            bool IsLeafNode();
            
        private:
            const static int MAX_NODES_PER_LEAF = 8;
            Node();
    
            GameObject * m_GameObject;
            Node * m_FTopLeft; //
            Node * m_FTopRight; //
            Node * m_FBottomLeft; //
            Node * m_FBottomRight; //

            Node * m_BTopLeft; //
            Node * m_BTopRight; //
            Node * m_BBottomLeft;//
            Node * m_BBottomRight; //
            
            std::vector<Node *> m_Nodes;
            Vector3 m_Center;
            Vector3 m_MinCorner;
            Vector3 m_MaxCorner;

        };
    
    };

    TYPE_DEFINE(GameObjectTree)
}

#endif