#ifndef VECTOR_H
#define VECTOR_H
    
    #include <malloc.h>
    #include "types.hpp"


    namespace mystd
    {
        template <typename T>
        class Vector
        {
            private:
                uint64 _Size;
                T*     _Array;
                uint64 _Length;

            public:
                uint64 size;
                uint64 len;

            private:
                // Updates the public accounts of thhe vector
                void _Update(uint64 size, uint64 length)
                {
                    this->_Size   = size;
                    this->_Length = length;

                    this->size = this->_Size;
                    this->len  = this->_Length;
                }

                // Resizes the vector
                void _Resize(uint64 size)
                {
                    this->_Array = (T*)(realloc(this->_Array, size * sizeof(T)));
                    this->_Update(size, this->_Length);
                }

            public:
                // Initializes the vector with a size of 32
                Vector()
                {
                    this->_Update(32, 0);
                    this->_Array  = (T*)(malloc(this->_Size * sizeof(T)));
                }

                // initializes the vector with a specified size
                Vector(uint64 size)
                {
                    this->_Update(size, 0);
                    this->_Array  = (T*)(malloc(this->_Size * sizeof(T)));
                }

                // Pushes an element onto the vector
                void Push(T item)
                {
                    // If there's not enough space, it resizes the vector
                    if (this->_Length + 1 == this->_Size)
                        this->_Resize(this->_Size * 2);

                    // Pushes the item and updates
                    this->_Array[this->_Length] = item;
                    this->_Update(this->_Size, this->_Length + 1);
                }

                // Pops an item from the vector
                void Pop(uint64 index)
                {
                    // Moves back all elements past the specified index
                    for (uint64 i = index; i < this->_Length - 1; i++)
                        this->_Array[i] = this->_Array[i + 1];

                    // Removes the item and updates the vector
                    this->_Array[this->_Length - 1] = NULL;
                    this->_Update(this->_Size, this->_Length - 1);
                }

                // Clears the entire vector
                void Clear()
                {
                    // Sets all entries to NULL
                    for (uint64 i = 0; i < this->_Length; i++)
                        this->_Array[i] = NULL;

                    // Updates the vector
                    this->_Update(this->_Size, 0);
                }

                // Returns an item given an index
                T Get(uint64 index)
                {
                    return this->_Array[index];
                }

                // Sets the contents of a specified index
                void Set(uint64 index, T item)
                {
                    this->_Array[index] = item;
                }
                
                // Returns -1 if there's no such item and an index if there is
                uint64 Find(T item)
                {
                    for (uint64 i = 0; i < this->len; i++)
                        if (this->Get(i) == item)
                            return i;

                    return -1;
                }

                // Returns weather or not an item exists
                bool Contains(T item)
                {
                    return this->Find(item) != -1;
                }
        };
    }

#endif
