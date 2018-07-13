#ifndef __AOI_IMPL_QUADTREENODE_H__
#define __AOI_IMPL_QUADTREENODE_H__

#include "alloc.h"
#include "../rect.h"
#include <memory>

namespace aoi
{
    namespace impl
    {
        enum ENodeType
        {
            NodeTypeNormal = 0,  // 
            NodeTypeLeaf = 1,    // Leaf Node
        };

        const unsigned ChildrenNum = 4;

        template<typename TItem, unsigned NodeCapacity, unsigned LevelLimit>
        class QuadTreeNode
        {
        public:
            using TNode = QuadTreeNode<TItem, NodeCapacity, LevelLimit>;

            QuadTreeNode(unsigned level, MemBase<TNode>* alloc, ENodeType type, QuadTreeNode* parent, const Rect& bounds);
            ~QuadTreeNode();

            bool Insert(TItem* item);        //insert an item in quadtree 
            bool Remove(TItem* item);		 //remove an item from quadtree
            void Query(const Rect& area, TItem*& head, TItem*& tail); //query the agents with the rectangle
            unsigned GetItemCount(); //get item amount

        public:
            unsigned mLevel;                         // the level that the current node stand
            Rect mBounds;                            // the rectangle range of node
            QuadTreeNode* mParent;                   // parent node
            ENodeType mNodeType;                     // node type
            QuadTreeNode* mChildrens[ChildrenNum];   // children node
            unsigned mItemCount;                     // the item amount in leaf node
            TItem* mItems;                           // the items in leaf node

        private:
            MemBase<TNode>* mAlloc;                  // node allocator
            void split();  //item amount is above the limit
            void tryMerge();// item amount is under the limit
        };
    }
}

#endif
