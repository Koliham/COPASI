/* Begin CVS Header
   $Source: /Volumes/Home/Users/shoops/cvs/copasi_dev/copasi/utilities/CCopasiTree.h,v $
   $Revision: 1.15 $
   $Name:  $
   $Author: shoops $ 
   $Date: 2003/11/04 15:22:37 $
   End CVS Header */

/**
 * CCopasiTree class.
 * The template class CCopasiTree describes a tree of Nodes.
 *
 * Created for Copasi by Stefan Hoops 2003
 */

#ifndef COPASI_CCopasiTree
#define COPASI_CCopasiTree

#include <iterator>
#include <set>

/**
 * The template class CCopasiTree takes as template argument a class
 * derived from the class CCopasiNode. It allows the construction of
 * a tree with four simple methods. It assures that each node in the
 * tree is unique.
 *
 * In addition it provides a forward iterator usefull to traverse the 
 * tree.
 *
 * Note: The tree takes ownership of all nodes. Therefore, you must not use
 *       pointers to automatically created nodes in the attachNode() Function
 *       unless you use the detach node function prior to the destruction of
 *       the tree.
 */
template < class _Node > class CCopasiTree
    {
    public:
      typedef _Node Node;

      // Attributes
    private:
      /**
       * The root of the tree
       */
      _Node * mpRoot;

      /**
       * The list of all nodes. This is used to keep the tree consistent
       * by avoiding multiple inserts of the same node.
       */
      std::set<_Node *> mList;

    public:
      /**
       * A forward iterator used to traverse the tree.
       */
#if (defined __GNUC__ && __GNUC__ < 3)
    class iterator: public std::forward_iterator< _Node, ptrdiff_t >
#else
    class iterator:
            public std::iterator< std::forward_iterator_tag, _Node, ptrdiff_t >
#endif

        {
        private:
          /**
           * A pointer to the current node.
           */
          _Node * mCurrent;

        public:
          /**
           * Default constructor.
           * Note: When no argument is given the iterator points to the end of 
           *       the tree.
           * @param Node * begin (default NULL)
           */
          iterator(_Node * begin = NULL):
              mCurrent(begin)
          {}

          /**
           * Copy constructor
           * @param const iterator & src
           */
          iterator(const iterator & src):
              mCurrent(src.mCurrent)
          {}

          /**
           * Destructor
           */
          ~iterator() {}

          /**
           * Dereference operator * returns the node the iterator points to.
           * @return Node &
           */
          _Node & operator*() const {return * mCurrent;}

          /**
           * Dereference operator * returns the node the iterator points to.
           * @return Node &
           */
          _Node * operator->() const {return mCurrent;}

          /**
           * Comparison operator !=
           * @param const iterator &rhs
           * @return bool not-equal
           */
          bool operator!=(const iterator &rhs)
          {return (mCurrent != rhs.mCurrent);}

          /**
           * Assignement operator from a node to an iterator
           * @param Node * pNode
           * @return iterator &
           */
          iterator & operator=(_Node * pNode)
          {
            mCurrent = pNode;
            return *this;
          }

          /**
           * Return the first node after the children of the current node.
           * This might be a sibling or an ancestor.
           * @return Node * pastChildren
           */
          _Node * pastChildren()
          {
            _Node * pastChildren = NULL;

            if (mCurrent->getSibling())
              pastChildren = (_Node *) mCurrent->getSibling();
            else
              {
                _Node * pTmp = (_Node *) mCurrent->getParent();

                while (pTmp)
                  {
                    if ((pastChildren = (_Node *) pTmp->getSibling()))
                      break;

                    pTmp = (_Node *) pTmp->getParent();
                  }
              }
            return pastChildren;
          }

          /**
           * Prefix increment operator ++
           * @return iterator &
           */
          iterator & operator++()
          {
            if (mCurrent->getChild())
              mCurrent = (_Node *) mCurrent->getChild();
            else
              mCurrent = pastChildren();

            return *this;
          }
        };

      // Operations
    public:
      /**
       * Default constructor
       */
      CCopasiTree():
          mpRoot(new _Node),
          mList()
    {mList.insert(mpRoot);}

      /**
       * Destructor
       */
      ~CCopasiTree() {pdelete(mpRoot);}

      /**
       * Retreive an iterator pointing to the beginning of the tree
       * @return iterator begin
       */
      iterator begin() const {return iterator(mpRoot);}

      /**
       * Retreive an iterator pointing beyond the end of the tree
       * @return iterator end
       */
      iterator end() const {return iterator(NULL);}

      /**
       * Retreive the root node of the tree
       * @return Node * root
       */
      _Node * getRoot() {return mpRoot;}

      /**
       * Retreive the data of the Tree.
       * @return Data data
       */
      typename _Node::Data getData() const {return mpRoot->getData();}

      /**
       * Attach a Node to the tree
       * Note: If pAfterChild == pParent then the child will be inserted as
       *       the first child 
       * @param Node * pNode
       * @param Node * pParent (default: NULL euivalent to the root of the tree)
       * @param Node * pAfterChild (default: NULL at the end of the children)
       * @return bool Success
       */
      bool attachNode(_Node * pNode,
                      _Node * pParent = NULL,
                      _Node * pAfterChild = NULL)
      {
        bool Success = true;

        if (mList.count(pNode))
          return false;                     // Node already in tree.
        if (pAfterChild && !mList.count(pAfterChild))
          return false;                     // Invalid insertion point.

        if (pParent)
          {
            if (mList.count(pParent))
              Success = pParent->addChild(pNode, pAfterChild);
            else
              Success = false;                 // Invalid parent.
          }
        else
          Success = mpRoot->addChild(pNode, pAfterChild);

        if (Success)
          {
            iterator it = pNode;
            iterator end = it.pastChildren();

            for (; it != end; ++it)
              mList.insert(&*it);
          }

        return Success;
      }

      /**
       * Remove the given node of the tree
       * @param Node * pNode
       * @return bool Success
       */
      bool removeNode(_Node * pNode)
      {
        if (!pNode) return false;          // Nothing to remove.
        if (pNode == mpRoot) return false; // Root must not be removed.

        iterator it = pNode;
        iterator end = it.pastChildren();

        for (; it != end; ++it)
          mList.erase(&*it);

        delete pNode;

        return true;
      }

      /**
       * Move a given node from its current place in the tree to the
       * one specified by pParent and pAfterChild. The insertion
       * behaviour is similar to addChild().
       * @param Node * pNode
       * @param Node * pParent (default: NULL euivalent to the root of the tree)
       * @param Node * pAfterChild (default: NULL at the end of the children)
       * @return bool Success
       */
      bool moveNode(_Node * pNode, _Node * pParent = NULL, _Node * pAfterChild = NULL)
      {
        detachNode(pNode);
        return attachNode(pNode, pParent, pAfterChild);
      }

      /**
       * Detach node.
       * Node: After detachment of a node the tree no longer has the ownership.
       * @param Node * pNode
       * @return bool Success
       */
      bool detachNode(_Node * pNode)
      {
        if (!pNode) return false;          // Nothing to do.
        if (pNode == mpRoot) return false; // Root must not be detached

        iterator it = pNode;
        iterator end = it.pastChildren();

        for (; it != end; ++it)
          mList.erase(&*it);

        return pNode->getParent()->removeChild(pNode);
      }

#ifdef WIN32
      friend std::ostream & operator << (std::ostream & os,
                                         const CCopasiTree< _Node > & A);
#else
      friend std::ostream & operator << <>(std::ostream & os,
                                           const CCopasiTree< _Node > & A);
#endif // WIN32
    };

template <class _Node>
std::ostream & operator<< (std::ostream & os,
                           const CCopasiTree< _Node > & A)
{
  typename CCopasiTree< _Node >::iterator it = A.begin();
  typename CCopasiTree< _Node >::iterator end = A.end();

  for (; it != end && &*it != NULL; ++it)
    os << &*it << ": parent: " << it->getParent()
    << ", child: " << it->getChild()
    << ", sibling: " << it->getSibling() << std::endl;

  os << std::endl;
  return os;
}

#endif // COPASI_CCopasiTree
