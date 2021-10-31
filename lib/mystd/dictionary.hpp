#ifndef DICTIONARY_H
#define DICTIONARY_H

    #include "vector.hpp"
    #include <stdio.h>


    namespace mystd
    {
        template <typename key_t, typename item_t>
        class Dictionary
        {
            private:
                Vector<key_t> _Keys;
                Vector<item_t> _Items;

            public:
                Dictionary()
                {
                    _Keys  = Vector<key_t>(32);
                    _Items = Vector<item_t>(32);
                }

                // Returns a list of all keys
                Vector<key_t> GetKeys()
                {
                    return this->_Keys;
                }

                // Returns a listo of all items
                Vector<item_t> GetItems()
                {
                    return this->_Items;
                }

                // Returns an item given a key
                item_t GetItem(key_t key)
                {
                    return this->_Items.Get(this->_Keys.Find(key));
                }

                // Returns a key given an item
                key_t GetKey(item_t item)
                {
                    return this->_Keys.Get(this->_Items.Find(item));
                }

                // Pops an item from the dictionary given a key
                void Pop(key_t key)
                {
                    this->_Items.Pop(this->_Keys.Find(key));
                    this->_Keys.Pop(this->_Keys.Find(key));
                }

                // It pushes an item into the dictionary given a key
                void SetItem(key_t key, item_t item)
                {
                    // Looks for the key in the keys vector
                    int64 index = this->_Keys.Find(key);
                    
                    // If the key already exists
                    if (index != -1)
                    {
                        // It replaces the item with the specified one
                        this->_Items.Set(index, item);
                        return;
                    }

                    // Otherwise, it creates a key and item in the respective vectors
                    this->_Keys.Push(key);
                    this->_Items.Push(item);
                }
        };
    }
    
#endif

