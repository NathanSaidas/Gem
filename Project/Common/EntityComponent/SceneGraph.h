#ifndef GEM_SCENE_GRAPH_H
#define GEM_SCENE_GRAPH_H

//============================================================
// Date:			May,	20,		2015
// Description:		Manages gameobjects in a graph structure.
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- 
#pragma endregion

#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"
#include "../Core/Func.h"
#include "../Core/Method.h"
#include <vector>




namespace Gem
{
	class GameObject;

	FORCE_EXPORT_META(SceneGraph);

	class GEM_API SceneGraph : public object
	{
		RDECLARE_CLASS(SceneGraph)
	public:
		typedef GameObject* GraphNode;
		typedef std::vector<GameObject*> GraphNodes;
		typedef Func<bool, GraphNode> GraphCompare;
		SceneGraph();
		~SceneGraph();


		void Insert(GraphNode aGameObject);
		void Remove(GraphNode aGameObject);

		GraphNode Find(const std::string & aName);
		GraphNodes FindAll(const std::string & aName);

		GraphNode FindOfType(const Type & aComponentType);
		GraphNodes FindAllOfType(const Type & aComponentType);

		GraphNode Find(const GraphCompare & aCompareFunc);
		GraphNodes FindAll(const GraphCompare & aCompareFunc);

	private:
		GraphNode m_Top;

		friend class Scene;
	};
}


#endif // GEM_SCENE_GRAPH_H
