//  File AKvector.h
//  Template definitions for AKvectors
//  Modeled after STL vector
//  Created by AlexKinder on 6/12/20.

#ifndef AKvector_h
#define AKvector_h


// ---------------------- AKvector Prototype --------------------------
template <typename T>
class AKvector {
private:
   int mSize;
   int mCapacity;
   T *mT;

   static const int m_SPARE_CAPACITY = 16;

public:
   AKvector(int initSize = 0); // default constructor
   AKvector(const AKvector& rhs); // copy constructor
   
   ~AKvector () { delete[] mT; } // destructor

   const AKvector& operator= (const AKvector& rhs);
   void resize(int newSize);
   void reserve(int newCapacity);

   T& operator[](int index);
   const T& operator[] (int index) const;

   // thrown in for stl vector compatibility
   T& at(int index) { return (*this)[index]; }
   const T& at(int index) const  { return (*this)[index]; }

   bool empty() const { return mSize == 0; }
   int size() const { return mSize; }
   int capacity() const { return mCapacity; }

   void push_back(const T& x);
   void pop_back();
   const T& back() const;
   const T& front() const;
   void clear() { *this = AKvector<T>(); }

   typedef T *iterator;
   typedef const T *const_iterator;

   iterator begin() { return &mT[0]; }
   const_iterator begin() const { return &mT[0]; }
   iterator end() { return &mT[ size() ]; }
   const_iterator end() const { return &mT[ size() ]; }

   iterator erase(iterator first, iterator last);
   iterator erase(iterator itemToErase);

private:
   void setSize(int size);
   void setCapacity(int capacity);

public:
   // for exception throwing
   class OutOfBoundsException { };
   class VectorEmptyException { };
};

// AKvector method definitions -------------------
// private utilities for member methods
template <typename T>
void AKvector<T>::setSize(int size) {
   mSize = (size < 0) ? 0 : size;
}

template <typename T>
void AKvector<T>::setCapacity(int capacity) {
   mCapacity = (capacity <= mSize)? mSize + m_SPARE_CAPACITY : capacity;
}

// public interface
template <typename T>
AKvector<T>::AKvector(int initSize) {
   setSize(initSize);
   setCapacity(mSize + m_SPARE_CAPACITY);
   mT = new T[mCapacity];
}

template <typename T>
AKvector<T>::AKvector(const AKvector<T>& rhs) {
   mT = NULL;   // needed for following
   operator=(rhs);
}

template <typename T>
const AKvector<T>& AKvector<T>::operator= (const AKvector& rhs) {
   int k;

   if (this == &rhs)
      return *this;
   delete[] mT;
   setSize(rhs.mSize);
   setCapacity(rhs.mCapacity);
   mT = new T[mCapacity];
   for (k = 0; k < mSize; k++)
      mT[k] = rhs.mT[k];
   return *this;
}

template <typename T>
void AKvector<T>::resize(int newSize) {
   if (newSize <= mCapacity)
   {
      setSize(newSize);
      return;
   }
   reserve(2*newSize + 1);
   setSize(newSize);
}

template <typename T>
void AKvector<T>::reserve(int newCapacity) {
   T *oldT;
   int k;

   if (newCapacity < mSize)
      return;
 
   setCapacity(newCapacity);
   oldT = mT;
   mT = new T[mCapacity];

   for (k = 0; k < mSize; k++)
      mT[k] = oldT[k];
   delete[] oldT;
}

template <typename T>
T& AKvector<T>::operator[](int index) {
   if (index < 0 || index >= mSize)
      throw OutOfBoundsException();
   return mT[index];
}

template <typename T>
const T& AKvector<T>::operator[] (int index) const {
   if (index < 0 || index >= mSize)
      throw OutOfBoundsException();
   return mT[index];
}

template <typename T>
void AKvector<T>::push_back(const T& x) {
   if (mSize == mCapacity)
      reserve(2*mCapacity + 1);
   mT[mSize++] = x;
}

template <typename T>
void AKvector<T>::pop_back() {
   if (mSize > 0)
      mSize--;
}

template <typename T>
const T& AKvector<T>::back() const {
  if (mSize <= 0)
      throw VectorEmptyException();
  return mT[mSize - 1];
}

template <typename T>
const T& AKvector<T>::front() const {
  if (mSize <= 0)
      throw VectorEmptyException();
  return mT[0];
}

template <typename T>
T* AKvector<T>::erase(iterator first, iterator last) {
   iterator iter1, iter2, retVal, endVal;

   if (first < begin() || last > end() || first >= last)
      return NULL;
   
   retVal = first; // prepare a return value (first element after erase block)
   endVal = end(); // for faster looping

   for (iter1 = first, iter2 = last;  iter2 != endVal; )
      *iter1++ = *iter2++;   // objects must have good deep copy management

   setSize(mSize - (last - first));
   return retVal;  // points to first element not erased after block
}

template <typename T>
T* AKvector<T>::erase(iterator itemToErase) {
   return erase(itemToErase, itemToErase + 1);
}

#endif /* AKvector_h */
