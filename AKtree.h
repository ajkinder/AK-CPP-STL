//
//  AKtree.h
//  Created by AlexKinder on 5/12/20.
//

#ifndef AKtree_h
#define AKtree_h

// advanced prototype for the AKtreeNode to use to declare a friend
template <typename T>
class AKtree;

// ---------------------- AKtreeNode Prototype --------------------------
template <typename T>
class AKtreeNode {
   friend class AKtree<T>;

protected:
   AKtreeNode *firstChild, *sib, *prev;
   T data;
   AKtreeNode *myRoot;  // needed to test for certain error

public:
   AKtreeNode(const T& d = T(),
      AKtreeNode *sb = NULL, AKtreeNode *chld = NULL, AKtreeNode *prv = NULL)
      : firstChild(chld), sib(sb), prev(prv), data(d), myRoot(NULL) { }
    
   T GetData() const { return data; }

protected:
   // for use only by AKtree
   AKtreeNode(const T&  d,
      AKtreeNode *sb, AKtreeNode *chld, AKtreeNode *prv,
      AKtreeNode *root)
      : firstChild(chld), sib(sb), prev(prv), data(d), myRoot(root) { }
};
// ---------------------- AKtree Prototype --------------------------
template <typename T>
class AKtree {
protected:
   int mSize;
   AKtreeNode<T> *mRoot;

public:
   AKtree() { mSize = 0; mRoot = NULL; }
   AKtree(const AKtree &rhs) { mRoot = NULL; mSize = 0; *this = rhs; }
   virtual ~AKtree() { clear(); }
   bool empty() const { return (mSize == 0); }
   int size() const { return mSize; }
   void clear() { removeNode(mRoot); }
   const AKtree & operator=(const AKtree &rhs);

   AKtreeNode<T> *addChild(AKtreeNode<T> *treeNode, const T& x);

   AKtreeNode<T> *find(const T& x) { return find(mRoot, x); }
   AKtreeNode<T> *find(AKtreeNode<T> *root,
      const T& x, int level = 0);

   bool remove(const T& x) { return remove(mRoot, x); }
   bool remove(AKtreeNode<T> *root, const T& x);
   void removeNode(AKtreeNode<T> *nodeToDelete);

   void display(AKtreeNode<T> *treeNode = NULL, int level = 0) const;

   template <class Processor>
   void traverse(Processor func, AKtreeNode<T> *treeNode = NULL) const;

protected:
   AKtreeNode<T> *clone(AKtreeNode<T> *root) const;
   void setMyRoots(AKtreeNode<T> *treeNode);
};

// public interface methods of AKtree ------------------------
template <typename T>
AKtreeNode<T>* AKtree<T>::find(AKtreeNode<T> *root,
   const T& x, int level) {
   AKtreeNode<T> *retval;
 
   if (mSize == 0 || root == NULL)
      return NULL;

   if (root->data == x)
      return root;

   // otherwise, recurse.  don't process sibs if this was the original call
   if (level > 0 && (retval = find(root->sib, x, level)))
      return retval;
   return find(root->firstChild, x, ++level);
}

template <typename T>
bool AKtree<T>::remove(AKtreeNode<T> *root, const T& x) {
   AKtreeNode<T> *tn = NULL;

   if (mSize == 0 || root == NULL)
      return false;
  
   if ((tn = find(root, x)) != NULL)
   {
      removeNode(tn);
      return true;
   }
   return false;
}

template <typename T>
const AKtree<T> &AKtree<T>::operator= (const AKtree &rhs) {
   if (&rhs != this) {
      clear();
      mRoot = clone(rhs.mRoot);
      mSize = rhs.mSize;
      setMyRoots(mRoot);
   }
   return *this;
}

template <typename T>
void AKtree<T>::removeNode(AKtreeNode<T> *nodeToDelete) {
   if (nodeToDelete == NULL || mRoot == NULL)
      return;
   if (nodeToDelete->myRoot != mRoot)
      return;  // silent error, node does not belong to this tree

   // remove all the children of this node
   while (nodeToDelete->firstChild)
      removeNode(nodeToDelete->firstChild);

   if (nodeToDelete->prev == NULL)
      mRoot = NULL;  // last node in tree
   else if (nodeToDelete->prev->sib == nodeToDelete)
      nodeToDelete->prev->sib = nodeToDelete->sib; // adjust left sibling
   else
      nodeToDelete->prev->firstChild = nodeToDelete->sib;  // adjust parent

   // adjust the successor sib's prev pointer
   if (nodeToDelete->sib != NULL)
      nodeToDelete->sib->prev = nodeToDelete->prev;

   delete nodeToDelete;
  --mSize;
}

template <typename T>
AKtreeNode<T> *AKtree<T>::addChild(
   AKtreeNode<T> *treeNode, const T& x) {
   // empty tree? - create a root node if user passes in NULL
   if (mSize == 0) {
      if (treeNode != NULL)
         return NULL; // silent error something's fishy.  treeNode can't right
      mRoot = new AKtreeNode<T>(x, NULL, NULL, NULL);
      mRoot->myRoot = mRoot;
      mSize = 1;
      return mRoot;
   }
   if (treeNode == NULL)
      return NULL; // silent error inserting into a non_null tree with a null parent
   if (treeNode->myRoot != mRoot)
      return NULL;  // silent error, node does not belong to this tree

   // push this node into the head of the sibling list; adjust prev pointers
   AKtreeNode<T> *newNode = new AKtreeNode<T>(x,
      treeNode->firstChild, NULL, treeNode, mRoot);  // sib, child, prev, root
   treeNode->firstChild = newNode;
   if (newNode->sib != NULL)
      newNode->sib->prev = newNode;
   ++mSize;
   return newNode;
}

template <typename T>
void AKtree<T>::display(AKtreeNode<T> *treeNode, int level) const {
   AKtreeNode<T> *child;

   // this will be static and so will be shared by all calls
   static string blankString = "                                    ";
   string indent;

   // stop runaway indentation/recursion
   if  (level > (int)blankString.length() - 1) {
      cout << blankString << " ... " << endl;
      return;
   }

   indent = blankString.substr(0, level);

   if (mRoot == NULL)
      return;
   if (treeNode == NULL) {
      display(mRoot);
      return;
   }

   cout << indent << treeNode->data  << endl;
   for (child = treeNode->firstChild; child != NULL; child = child->sib)
      display(child, level+1);
}

template <typename T>
template <class Processor>
void AKtree<T>::traverse(Processor func, AKtreeNode<T> *treeNode) const {
   AKtreeNode<T> *child;

   if (mRoot == NULL)
      return;
   if (treeNode == NULL) {
      traverse(func, mRoot);
      return;
   }

   func(treeNode->data);

   for (child = treeNode->firstChild; child != NULL; child = child->sib)
      traverse(func, child);
}

// FHsearchTree protected method definitions -----------------------------
template <typename T>
AKtreeNode<T> *AKtree<T>::clone(
   AKtreeNode<T> *root) const {
   AKtreeNode<T> *newNode;
   if (root == NULL)
      return NULL;

   // does not set myRoot which must be done by caller
   newNode = new AKtreeNode<T>(
      root->data,
      clone(root->sib), clone(root->firstChild));
   if (newNode->sib)
      newNode->sib->prev = newNode;
   if (newNode->firstChild)
      newNode->firstChild->prev = newNode;
   return newNode;
}

template <typename T>
void AKtree<T>::setMyRoots(AKtreeNode<T> *treeNode) {
   AKtreeNode<T> *child;

   if (mRoot == NULL)
      return;

   treeNode->myRoot = mRoot;
   for (child = treeNode->firstChild; child != NULL; child = child->sib)
      setMyRoots(child);
}

#endif /* AKtree_h */
