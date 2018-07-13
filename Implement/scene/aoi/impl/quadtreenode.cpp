#include "../point.h"
#include <cassert>
#include <string.h>
#include "quadtree.h"

namespace aoi
{
    namespace impl
    {
        template<typename TItem, unsigned NodeCapacity, unsigned LevelLimit>
        QuadTreeNode<TItem, NodeCapacity, LevelLimit>::QuadTreeNode(unsigned level, MemBase<TNode>* alloc, ENodeType type, QuadTreeNode* parent, const Rect& bounds)
            : mLevel(level)
            , mAlloc(alloc)
            , mParent(parent)
            , mNodeType(type)
            , mBounds(bounds)
            , mItemCount(0)
            , mItems(nullptr)
        {
            memset(mChildrens, 0, sizeof(mChildrens));
        }

        template<typename TItem, unsigned NodeCapacity, unsigned LevelLimit>
        QuadTreeNode<TItem, NodeCapacity, LevelLimit>::~QuadTreeNode()
        {

        }

		/*
			
		*/ 
        template<typename TItem, unsigned NodeCapacity, unsigned LevelLimit>
        bool QuadTreeNode<TItem, NodeCapacity, LevelLimit>::Insert(TItem* item)
        {
            if (mNodeType == NodeTypeNormal)
            {
            LABLE_NORMAL:
                int index = mBounds.GetQuadrant(item) - 1;
                //recursive insert
                return index >= 0 ? mChildrens[index]->Insert(item) : false;
            }
            else
            {
                if (mItemCount < NodeCapacity)
                {
                    if (mBounds.Contains(item))
                    {
                        mItemCount++;
                        item->mItemNext = mItems;
                        mItems = item;
                        item->mNode = this;
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    if (mLevel + 1 >= LevelLimit)
                    {
                        return false;
                    }
                    split();
                    goto LABLE_NORMAL;
                }
            }
        }

		/*
		
		*/ 
        template<typename TItem, unsigned NodeCapacity, unsigned LevelLimit>
        void QuadTreeNode<TItem, NodeCapacity, LevelLimit>::split()
        {
            assert(mNodeType == NodeTypeLeaf);
            mNodeType = NodeTypeNormal;

            // first quartile
            Rect rect0(mBounds.MidX(), mBounds.Right(), mBounds.MidY(), mBounds.Top());

            // second quartile
            Rect rect1(mBounds.Left(), mBounds.MidX(), mBounds.MidY(), mBounds.Top());

            // third quartile
            Rect rect2(mBounds.Left(), mBounds.MidX(), mBounds.Bottom(), mBounds.MidY());

            // fourth quartile
            Rect rect3(mBounds.MidX(), mBounds.Right(), mBounds.Bottom(), mBounds.MidY());

            mChildrens[0] = mAlloc->New(mLevel + 1, mAlloc, NodeTypeLeaf, this, rect0);
            mChildrens[1] = mAlloc->New(mLevel + 1, mAlloc, NodeTypeLeaf, this, rect1);
            mChildrens[2] = mAlloc->New(mLevel + 1, mAlloc, NodeTypeLeaf, this, rect2);
            mChildrens[3] = mAlloc->New(mLevel + 1, mAlloc, NodeTypeLeaf, this, rect3);

			//split
            for (TItem* it = mItems; it;)
            {
                TItem* head = (TItem*)(it->mItemNext);
                int index = mBounds.GetQuadrant2(it) - 1;
                mChildrens[index]->Insert(it);
                it = head;
            }
            mItemCount = 0;
            mItems = nullptr;
    }

        template<typename TItem, unsigned NodeCapacity, unsigned LevelLimit>
        bool QuadTreeNode<TItem, NodeCapacity, LevelLimit>::Remove(TItem* item)
        {
            assert(mNodeType == NodeTypeLeaf);
            assert(mItems);
            TItem* pre = nullptr;
            for (TItem* it = mItems; it;)
            {
                TItem* head = (TItem*)(it->mItemNext);
                if (it == item)
                {
                    --mItemCount;
                    pre ? pre->mItemNext = it->mItemNext : mItems = (TItem*)(mItems->mItemNext);
                    tryMerge();
                    return true;
                }
                else
                {
                    pre = it;
                }
                it = head;
            }
            return false;
        }

        template<typename TItem, unsigned NodeCapacity, unsigned LevelLimit>
        void QuadTreeNode<TItem, NodeCapacity, LevelLimit>::tryMerge()
        {
            TNode* node = mParent;
            while (node) {
                assert(node->mNodeType == NodeTypeNormal);
                unsigned count = 0;
                auto& childrens = node->mChildrens;
                for (size_t i = 0; i < ChildrenNum; i++)
                {
                    if (childrens[i]->mNodeType != NodeTypeLeaf) {
                        return;
                    }
                    count += childrens[i]->mItemCount;
                }

                if (count <= NodeCapacity)
                {
                    node->mNodeType = NodeTypeLeaf;
                    node->mItemCount = 0;
                    node->mItems = nullptr;
                    for (size_t i = 0; i < ChildrenNum; i++)
                    {
                        for (TItem* it = childrens[i]->mItems; it;)
                        {
                            TItem* head = (TItem*)(it->mItemNext);
                            node->mItemCount++;
                            it->mItemNext = node->mItems;
                            node->mItems = it;
                            it->mNode = node;
                            it = head;
                        }
                        mAlloc->Delete(childrens[i]);
                    }
                    node = node->mParent;
                }
                else
                {
                    break;
                }
                }
            }

        template<typename TItem, unsigned NodeCapacity, unsigned LevelLimit>
        void QuadTreeNode<TItem, NodeCapacity, LevelLimit>::Query(const Rect& area, TItem*& head, TItem*& tail)
        {
            if (mNodeType == NodeTypeNormal)
            {
                for (size_t i = 0; i < ChildrenNum; i++)
                {
                    if (area.Intersects(mChildrens[i]->mBounds))
                    {
                        mChildrens[i]->Query(area, head, tail);
                    }
                }
            }
            else
            {
                for (TItem* it = mItems; it; it = (TItem*)(it->mItemNext))
                {
                    if (area.Contains(it))
                    {  //The query result, stored in a link 
                        head ? (tail->mQueryNext = it, tail = it) : head = tail = it;
                    }
                }
            }
        }

        template<typename TItem, unsigned NodeCapacity, unsigned LevelLimit>
        unsigned QuadTreeNode<TItem, NodeCapacity, LevelLimit>::GetItemCount()
        {
            unsigned count = 0;
            if (mNodeType == NodeTypeNormal)
            {
                for (size_t i = 0; i < ChildrenNum; i++)
                {
                    count += mChildrens[i]->GetItemCount();
                }
            }
            else
            {
                count += mItemCount;
            }
            return count;
        }
        }
}
