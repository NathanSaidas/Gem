#include "GameObjectTree.h"
#include "GameObject.h"

namespace Gem
{
    RDEFINE_CLASS(GameObjectTree, object)

    GameObjectTree::GameObjectTree()
    {

    }

    GameObjectTree::~GameObjectTree()
    {

    }

    GameObjectTree::Node::Node(GameObject * aGameObject)
    {
        if (aGameObject == nullptr)
        {
            //DEBUG_LOG("Creating a bad node");
        }
        m_GameObject = aGameObject;
    }
    GameObjectTree::Node::~Node()
    {

    }
    Vector3 GameObjectTree::Node::GetPosition()
    {
        if (m_GameObject == nullptr)
        {
            return Vector3::Zero();
        }
        Matrix4x4 mat = m_GameObject->GetLocalToWorldMatrix();
        Vector4 pos = mat[3];
        return Vector3(pos.x, pos.y, pos.z);
    }
    Bounds GameObjectTree::Node::GetBounds()
    {
        if (m_GameObject == nullptr)
        {
            return Bounds();
        }
        return m_GameObject->GetBounds();
    }
    GameObjectTree::Node::Node()
    {
        m_GameObject = nullptr;
    }

    void GameObjectTree::Node::Split()
    {

    }
    void GameObjectTree::Node::Collapse()
    {

    }

    void GameObjectTree::Node::AddNode(Node * aNode)
    {
        if (IsLeafNode())
        {
            m_Nodes.push_back(aNode);

            if (m_Nodes.size() > MAX_NODES_PER_LEAF)
            {
                Split();
            }
        }
        else
        {

            Vector3 centerDifference = aNode->GetPosition() - m_Center;
            Bounds bounds = aNode->GetBounds();
            //To the right
            if (centerDifference.x > bounds.GetMax().x)
            {
                //To the top
                if (centerDifference.y > bounds.GetMax().y)
                {
                    //To the forward
                    if (centerDifference.z > bounds.GetMax().z)
                    {
                        m_FTopRight->AddNode(aNode);
                    }
                    else if (centerDifference.z < bounds.GetMin().z)
                    {
                        m_BTopRight->AddNode(aNode);
                    }
                    else
                    {
                        m_Nodes.push_back(aNode);
                    }
                }
                //To the bottom
                else if (centerDifference.y < bounds.GetMin().y)
                {
                    if (centerDifference.z > bounds.GetMax().z)
                    {
                        m_FBottomRight->AddNode(aNode);
                    }
                    else if (centerDifference.z < bounds.GetMin().z)
                    {
                        m_BBottomRight->AddNode(aNode);
                    }
                    else
                    {
                        m_Nodes.push_back(aNode);
                    }
                }
                else
                {
                    m_Nodes.push_back(aNode);
                }
            }
            //To the left
            else if (centerDifference.x < bounds.GetMin().x)
            {
                //Top
                if (centerDifference.y > bounds.GetMax().y)
                {
                    //Forward
                    if (centerDifference.z > bounds.GetMax().z)
                    {
                        m_FTopLeft->AddNode(aNode);
                    }
                    //Backward
                    else if (centerDifference.z < bounds.GetMin().z)
                    {
                        m_BTopLeft->AddNode(aNode);
                    }
                    else
                    {
                        m_Nodes.push_back(aNode);
                    }
                }
                //Bottom
                else if (centerDifference.y < bounds.GetMin().y)
                {
                    //Forward
                    if (centerDifference.z > bounds.GetMax().z)
                    {
                        m_FBottomLeft->AddNode(aNode);
                    }
                    //Backward
                    else if (centerDifference.z < bounds.GetMin().z)
                    {
                        m_BBottomLeft->AddNode(aNode);
                    }
                    else
                    {
                        m_Nodes.push_back(aNode);
                    }
                }
                else
                {
                    m_Nodes.push_back(aNode);
                }
            }
            else
            {
                m_Nodes.push_back(aNode);
            }
        }
    }

    void GameObjectTree::Node::RemoveNode(Node * aNode)
    {
        if (IsLeafNode())
        {
            Utilities::Remove<Node*>(m_Nodes, aNode);
        }
        else
        {
            Vector3 centerDifference = aNode->GetPosition() - m_Center;
            Bounds bounds = aNode->GetBounds();
            //To the right
            if (centerDifference.x > bounds.GetMax().x)
            {
                //To the top
                if (centerDifference.y > bounds.GetMax().y)
                {
                    //To the forward
                    if (centerDifference.z > bounds.GetMax().z)
                    {
                        m_FTopRight->RemoveNode(aNode);
                    }
                    else if (centerDifference.z < bounds.GetMin().z)
                    {
                        m_BTopRight->RemoveNode(aNode);
                    }
                    else
                    {
                        m_Nodes.push_back(aNode);
                    }
                }
                //To the bottom
                else if (centerDifference.y < bounds.GetMin().y)
                {
                    if (centerDifference.z > bounds.GetMax().z)
                    {
                        m_FBottomRight->RemoveNode(aNode);
                    }
                    else if (centerDifference.z < bounds.GetMin().z)
                    {
                        m_BBottomRight->RemoveNode(aNode);
                    }
                    else
                    {
                        m_Nodes.push_back(aNode);
                    }
                }
                else
                {
                    m_Nodes.push_back(aNode);
                }
            }
            //To the left
            else if (centerDifference.x < bounds.GetMin().x)
            {
                //Top
                if (centerDifference.y > bounds.GetMax().y)
                {
                    //Forward
                    if (centerDifference.z > bounds.GetMax().z)
                    {
                        m_FTopLeft->RemoveNode(aNode);
                    }
                    //Backward
                    else if (centerDifference.z < bounds.GetMin().z)
                    {
                        m_BTopLeft->RemoveNode(aNode);
                    }
                    else
                    {
                        m_Nodes.push_back(aNode);
                    }
                }
                //Bottom
                else if (centerDifference.y < bounds.GetMin().y)
                {
                    //Forward
                    if (centerDifference.z > bounds.GetMax().z)
                    {
                        m_FBottomLeft->RemoveNode(aNode);
                    }
                    //Backward
                    else if (centerDifference.z < bounds.GetMin().z)
                    {
                        m_BBottomLeft->RemoveNode(aNode);
                    }
                    else
                    {
                        m_Nodes.push_back(aNode);
                    }
                }
                else
                {
                    m_Nodes.push_back(aNode);
                }
            }
            else
            {
                m_Nodes.push_back(aNode);
            }

            Collapse();
        }
    }

    bool GameObjectTree::Node::IsLeafNode()
    {
        return m_FTopLeft == nullptr && m_FTopRight == nullptr && m_FBottomLeft == nullptr && m_FBottomRight
            && m_BBottomLeft == nullptr && m_BBottomRight == nullptr && m_BTopLeft == nullptr && m_BTopRight == nullptr;
    }
}