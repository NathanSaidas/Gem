#include "SceneGraph.h"
#include "../Engine.h"
#include <stack>

namespace Gem
{
	RDEFINE_CLASS(SceneGraph,object)
	SceneGraph::SceneGraph() : object()
	{
		m_Top = MEM_POOL_ALLOC_T(GameObject, -1);
	}
	SceneGraph::~SceneGraph()
	{
		MEM_POOL_DEALLOC_T(m_Top,GameObject);
	}


	void SceneGraph::Insert(GraphNode aGameObject)
	{
		if (aGameObject == nullptr || m_Top == nullptr)
		{
			return;
		}
		if (aGameObject->GetParent() == nullptr)
		{
			m_Top->AddChild(aGameObject);
		}

	}
	void SceneGraph::Remove(GraphNode aGameObject)
	{
		if (aGameObject == nullptr || m_Top == nullptr)
		{
			return;
		}

		if (aGameObject->GetParent() == nullptr)
		{
			m_Top->RemoveChild(aGameObject);
		}
	}

	SceneGraph::GraphNode SceneGraph::Find(const std::string & aName)
	{
		
		std::vector<GraphNode> visited;
		std::stack<GraphNode> stack;


		visited.push_back(m_Top);
		stack.push(m_Top);

		while (true)
		{
			if (stack.size() == 0)
			{
				return nullptr;
			}

			GraphNode top = stack.top();
			GraphNodes children = top->m_Children;
			bool next = false;
			for (GraphNodes::iterator it = children.begin(); it != children.end(); it++)
			{
				GraphNodes::iterator existingChild = std::find(visited.begin(), visited.end(), *it);
				if (existingChild == visited.end())
				{
					continue;
				}

				//Condition met.
				if ((*it)->GetName() == aName)
				{
					return *it;
				}

				stack.push(*it);
				visited.push_back(*it);
				next = true;
				break;
			}

			if (!next)
			{
				stack.pop();
			}
		}
		return nullptr;
	}
	SceneGraph::GraphNodes SceneGraph::FindAll(const std::string & aName)
	{
		GraphNodes gameobjects;

		std::vector<GraphNode> visited;
		std::stack<GraphNode> stack;


		visited.push_back(m_Top);
		stack.push(m_Top);

		while (true)
		{
			if (stack.size() == 0)
			{
				return gameobjects;
			}

			GraphNode top = stack.top();
			GraphNodes children = top->m_Children;
			bool next = false;
			for (GraphNodes::iterator it = children.begin(); it != children.end(); it++)
			{
				GraphNodes::iterator existingChild = std::find(visited.begin(), visited.end(), *it);
				if (existingChild == visited.end())
				{
					continue;
				}

				//Condition met.
				if ((*it)->GetName() == aName)
				{
					gameobjects.push_back(*it);
				}

				stack.push(*it);
				visited.push_back(*it);
				next = true;
				break;
			}

			if (!next)
			{
				stack.pop();
			}
		}
		return gameobjects;
	}


	SceneGraph::GraphNode SceneGraph::FindOfType(const Type & aComponentType)
	{
		std::vector<GraphNode> visited;
		std::stack<GraphNode> stack;


		visited.push_back(m_Top);
		stack.push(m_Top);

		while (true)
		{
			if (stack.size() == 0)
			{
				return nullptr;
			}

			GraphNode top = stack.top();
			GraphNodes children = top->m_Children;
			bool next = false;
			for (GraphNodes::iterator it = children.begin(); it != children.end(); it++)
			{
				GraphNodes::iterator existingChild = std::find(visited.begin(), visited.end(), *it);
				if (existingChild == visited.end())
				{
					continue;
				}

				//Condition met.
				if ((*it)->GetComponent(aComponentType) != nullptr)
				{
					return *it;
				}

				stack.push(*it);
				visited.push_back(*it);
				next = true;
				break;
			}

			if (!next)
			{
				stack.pop();
			}
		}
		return nullptr;
	}
	SceneGraph::GraphNodes SceneGraph::FindAllOfType(const Type & aComponentType)
	{
		GraphNodes gameobjects;

		std::vector<GraphNode> visited;
		std::stack<GraphNode> stack;


		visited.push_back(m_Top);
		stack.push(m_Top);

		while (true)
		{
			if (stack.size() == 0)
			{
				return gameobjects;
			}

			GraphNode top = stack.top();
			GraphNodes children = top->m_Children;
			bool next = false;
			for (GraphNodes::iterator it = children.begin(); it != children.end(); it++)
			{
				GraphNodes::iterator existingChild = std::find(visited.begin(), visited.end(), *it);
				if (existingChild == visited.end())
				{
					continue;
				}

				//Condition met.
				if ((*it)->GetComponent(aComponentType) != nullptr)
				{
					gameobjects.push_back(*it);
				}

				stack.push(*it);
				visited.push_back(*it);
				next = true;
				break;
			}

			if (!next)
			{
				stack.pop();
			}
		}
		return gameobjects;
	}

	SceneGraph::GraphNode SceneGraph::Find(const GraphCompare & aCompareFunc)
	{
		std::vector<GraphNode> visited;
		std::stack<GraphNode> stack;


		visited.push_back(m_Top);
		stack.push(m_Top);

		while (true)
		{
			if (stack.size() == 0)
			{
				return nullptr;
			}

			GraphNode top = stack.top();
			GraphNodes children = top->m_Children;
			bool next = false;
			for (GraphNodes::const_iterator it = children.begin(); it != children.end(); it++)
			{
				GraphNodes::const_iterator existingChild = std::find(visited.begin(), visited.end(), *it);
				if (existingChild == visited.end())
				{
					continue;
				}

				//Condition met.
				if (aCompareFunc.Invoke(*it))
				{
					return *it;
				}

				stack.push(*it);
				visited.push_back(*it);
				next = true;
				break;
			}

			if (!next)
			{
				stack.pop();
			}
		}
		return nullptr;
	}
	SceneGraph::GraphNodes SceneGraph::FindAll(const GraphCompare & aCompareFunc)
	{
		GraphNodes gameobjects;

		std::vector<GraphNode> visited;
		std::stack<GraphNode> stack;


		visited.push_back(m_Top);
		stack.push(m_Top);

		while (true)
		{
			if (stack.size() == 0)
			{
				return gameobjects;
			}

			GraphNode top = stack.top();
			GraphNodes children = top->m_Children;
			bool next = false;
			for (GraphNodes::const_iterator it = children.begin(); it != children.end(); it++)
			{
				GraphNodes::const_iterator existingChild = std::find(visited.begin(), visited.end(), *it);
				if (existingChild == visited.end())
				{
					continue;
				}

				//Condition met.
				if (aCompareFunc.Invoke(*it))
				{
					gameobjects.push_back(*it);
				}

				stack.push(*it);
				visited.push_back(*it);
				next = true;
				break;
			}

			if (!next)
			{
				stack.pop();
			}
		}
		return gameobjects;
	}
}