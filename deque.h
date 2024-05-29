// FILE: deque.h
// PROVIDES: A class for deque (double ended queue)
// Belongs to namespace lab8GE
//
//
// TEMPLATE PARAMETER, TYPEDEFS and MEMBER CONSTANTS for the deque<Item> class:
//   The template parameter, Item, is the data type of the items in the deque,
//   also defined as deque<Item>::value_type. It may be any of the C++ built-in
//   types (int, char, etc.), or a class with a default constructor, a copy
//   constructor, and an assignment operator. The definition
//   deque<Item>::size_type is the data type of any variable that keeps track of
//   how many items are in a deque.
//
//   static const size_t BLOCK_SIZE = ____
//     deque::BLOCK_SIZE is the number of enteries in each data block
//
//   static const size_t BLOCKPOINTER_ARRAY_SIZE = ____
//     deque::BLOCKPOINTER_ARRAY_SIZE is the number of enteries in the array of block pointers
//
//   typedef ____ Item
//     sequence::Item is the data type of the items in the deque. It may be any of
//     the C++ built-in types (int, char, etc.), or a class with a default
//     constructor, an assignment operator, and a copy constructor.
//
//
// CONSTRUCTOR for the deque<Item> class:
//   deque(szie_type init_bp_array_size = BLOCKPOINTER_ARRAY_SIZE,
//         size_type initi_block_size = BLOCK_SIZE);
//     Postcondition: The deque contains an array of block pointers with BLOCKPOINTER_ARRAY_SIZE entries
//     NOTE: The default value for the init_bp_array_size is BLOCKPOINTER_ARRAY_SIZE.
//     The default value for the initi_block_size is BLOCK_SIZE.
//
//
//
// CONST MEMBER FUNCTIONS for the deque<Item> class:
//
//   bool isEmpty()
//     Postcondition: Returns false if the deque is empty.
//     Otherwise returns true.
//
//   value_type front()
//    Precondition: isItem() must return true.
//    Postcondition: Returns a copy of the front item.
//
//   value_type back()
//    Precondition: isItem() must return true.
//    Postcondition: Returns a copy of the back item.
//
//
// MEMBER FUNCTIONS for the deque<Item> class:
//   void clear()
//     Postcondition: All the entries of the deque are deleted.
//
//   void print()
//     Postcondition: Prints all the entries of the deque.
//
//   void reserve()
//    Postcondition: Increases the capacity of the deque by 20 x BLOCK_SIZE
//
//   void push_front(const value_type& entry)
//    Postcondition: Adds entry to the front of the deque
//
//   void push_back(const value_type& entry)
//    Postcondition: Adds entry to the back of the deque
//
//   void pop_front()
//    Precondition: isItem() must return true
//    Postcondition: Removes the front item of the deque
//
//   void pop_back()
//    Precondition: isItem() must return true
//    Postcondition: Removes the back item of the deque
//
//
// VALUE SEMANTICS for the deque<Item> class:
//    Assignments and the copy constructor may be used with deque objects.
//
//
// STANDARD ITERATOR MEMBER FUNCTIONS (provide a forward iterator):
//   iterator begin( )
//   iterator end( )



#ifndef COEN79_DEQUE
#define COEN79_DEQUE

//#define USEDEBUG

#ifdef USEDEBUG
#define Debug( x ) std::cout << x
#else
#define Debug( x )
#endif


#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <cassert>

#include "deque_iterator.h" // The iterator class for deque (refer to file deque_iterator.h)


namespace lab8GE
{
    template <class Item>
    class deque {
    public:
        
        // TYPEDEF
        static const size_t BLOCK_SIZE = 5; // Number of data items per data block
        static const size_t BLOCKPOINTER_ARRAY_SIZE = 5; // Number of entries in the array of block pointers (The minimum acceptable value is 2)
        
        typedef std::size_t size_type;
        typedef Item value_type; // The type of items stored in the deque
        
        typedef deque_iterator<Item> iterator; // A typedef to simplify the use of iterator class
        
        // CONSTRUCTOR and DESTRUCTOR
        deque(size_type init_bp_array_size = BLOCKPOINTER_ARRAY_SIZE, size_type initi_block_size = BLOCK_SIZE);
        deque(const deque& source);
        ~deque();
        
        // CONST MEMBER FUNCTIONS
        bool isEmpty();
        value_type front();
        value_type back();
        
        // ITERATOR MEMBER FUNCTIONS
        iterator begin();
        iterator end();
        
        // MODIFICATION MEMBER FUNCTIONS
        void operator=(const deque& source);
        void clear();
        void reserve();
        void push_front(const value_type& entry);
        void push_back(const value_type& entry);
        void pop_back();
        void pop_front();
        
        
    private:
        
        value_type** block_pointers;      // A pointer to the array of block pointers
        value_type**  block_pointers_end; // A pointer to the final entry in the array of block pointers
        
        value_type**  first_bp;  // A pointer to the entry of the array of block pointers which points to the first data block
        value_type**  last_bp;   // A pointer to the entry of the array of block pointers which points to the last data block
        
        value_type* front_ptr;   // A pointer to the front element of the deque
        value_type* back_ptr;    // A pointer to the back element of the deque
        
        size_type bp_array_size; // Number of entries in the array of block pointers
        size_type block_size;    // Number of entries in each data block
    };
    
    
    
    
    // ------------------------------------------------------------------------------
    // IMPLEMENTATION
    // ------------------------------------------------------------------------------

    
    //CONSTRUCTOR
    template <class Item>
    deque<Item>::deque (size_type init_bp_array_size, size_type init_block_size) {
        
        bp_array_size = init_bp_array_size;
        block_size = init_block_size;
        
        // Create an an array of block pointers and assign its address to "block_pointers"
        block_pointers = new value_type* [bp_array_size];
        
        // Assign NULL to all the entries of the array of block pointers
        for (size_type index = 0; index < bp_array_size; ++index)
        {
            block_pointers[index] = NULL;
        }
        
        // Set block_pointers_end to point to the end of the array
        block_pointers_end = block_pointers + (bp_array_size - 1);
        
        first_bp = last_bp = NULL;
        front_ptr = back_ptr = NULL;
    }

    
    //COPY CONSTRUCTOR
    template <class Item>
    deque<Item>::deque (const deque<Item>& source) {
        
        first_bp = last_bp = NULL;
        front_ptr = back_ptr = NULL;
        block_pointers = block_pointers_end = NULL;
        
        // Use the assignment operator
        // (must have been already implemented, you cannot rely on the compiler)
        *this = source;
    }
    
    
    // ASSIGNMENT OPERATOR
    template <class Item>
    void deque<Item>::operator=(const deque<Item>& source) {
        
        // Invoke the clear() function
        clear();
        
        // Delete the array of block pointers
        delete [] block_pointers;
        block_pointers = NULL;
        block_pointers_end = NULL;
        
        // Copy the size variables from "source" object
        bp_array_size = source.bp_array_size;
        block_size = source.block_size;
        
        // Create a new array of block pointers
        block_pointers = new value_type* [bp_array_size];
        
        // Copy the data blocks of "source" object
        for (size_type bp_array_index = 0; bp_array_index < source.bp_array_size; ++bp_array_index)
        {
            // If an entry of source.block_pointers is NULL, that entry should be NULL in this object as well
            if (source.block_pointers[bp_array_index] == NULL)
            {
                block_pointers[bp_array_index] = NULL;
            }
            else
            {
                //If this is the first_bp of source, then set the first_bp of this deque
                if (source.first_bp == source.block_pointers + bp_array_index) {
                    first_bp = block_pointers + bp_array_index;
                }
                
                //If this is the last_bp of source, then set the last_bp of this deque
                if (source.last_bp == source.block_pointers + bp_array_index) {
                    last_bp = block_pointers + bp_array_index;
                }
                
                // Create a data block
                block_pointers[bp_array_index] = new value_type [block_size];
                
                // Copy the elements, and set "front_ptr" and "back_ptr" if appropriate
                for (size_type block_item_index = 0; block_item_index < block_size; ++block_item_index)
                {
                    block_pointers[bp_array_index][block_item_index] = source.block_pointers[bp_array_index][block_item_index];
                    
                    if (source.front_ptr == source.block_pointers[bp_array_index] + block_item_index) {
                        front_ptr = block_pointers[bp_array_index] + block_item_index;
                    }
                    
                    if (source.back_ptr == source.block_pointers[bp_array_index] + block_item_index) {
                        back_ptr = block_pointers[bp_array_index] + block_item_index;
                    }
                }
            }
        }
    }
    
    
    //DESTRUCTOR
    template <class Item>
    deque<Item>::~deque () {
        // Invoke the clear() function to delete all the entries of the deque
        clear();
        
        bp_array_size = 0;
        block_size = 0;
    }
    
    
    // isEmpty FUNCTION
    template <class Item>
    bool deque<Item>::isEmpty () {
        return (front_ptr == NULL);
    }
    
    
    // FRONT FUNCTION
    template <class Item>
    typename deque<Item>::value_type deque<Item>::front () {
        assert(front_ptr != NULL);
        return *front_ptr;
    }
    
    
    // BACK FUNCTION
    template <class Item>
    typename deque<Item>::value_type deque<Item>::back () {
        assert(back_ptr != NULL);
        return *back_ptr;
    }
    
    
    // CLEAR FUNCTION
    template <class Item>
    void deque<Item>::clear () {
        
        if (block_pointers == NULL)
            return;
        
        for (value_type** p = block_pointers; p != block_pointers_end + 1; ++p) {
            
            // "p" points to an entry in the array of block pointers
            // "*p" is the address of a data block
            
            // If the entry is not NULL
            if (*p != NULL) {
                delete [] *p;
                *p = NULL;
            }
        }
        
        first_bp = last_bp = NULL;
        front_ptr = back_ptr = NULL;
    }
    
    // RESERVE FUNCTION
    template <class Item>
    void deque<Item>::reserve () {
        
        size_type old_size = bp_array_size;
        size_type new_size = old_size + 20;
        
        value_type** new_block_pointers = new value_type* [new_size];
        
        value_type** p1 = block_pointers;
        value_type** p2 = new_block_pointers;
        
        while (p1 != (block_pointers_end + 1))
        {
            *p2++ = *p1++;
        }
        
        delete [] block_pointers;
        
        block_pointers = new_block_pointers;
        bp_array_size = new_size;
        
        block_pointers_end = block_pointers + (bp_array_size - 1);
        
        first_bp = block_pointers;
        last_bp = block_pointers + (old_size - 1);
    }
    
    
    // PUSH_FRONT FUNCTION
    template <class Item>
    void deque<Item>::push_front (const value_type& entry) {
        
        if (isEmpty())
        {
            first_bp = last_bp = block_pointers + (bp_array_size / 2);
            *first_bp = new value_type [block_size];
            front_ptr = back_ptr = *first_bp;
            *front_ptr = entry;
            return;
        }
        
        if (front_ptr == *first_bp)
        {
            if (first_bp == block_pointers)
            {
                reserve();
            }
            
            --first_bp;
            
            if (*first_bp == NULL)
            {
                *first_bp = new value_type [block_size];
            }
            
            front_ptr = *first_bp + (block_size - 1);
            *front_ptr = entry;
            return;
        }
        
        --front_ptr;
        *front_ptr = entry;

        std::cout << first_bp;
        std::cout << *first_bp;
    }
    
    
    // PUSH_BACK FUNCTION
    template <class Item>
    void deque<Item>::push_back (const value_type& entry) {

        /*
        // Only the array of block pointers exists (and no data block exists)
        if (last_bp == NULL)
        {
            assert(bp_array_size > 1);
            size_t bp_mid = floor(bp_array_size/2); // Get the mid point of the array of block pointers
            
            last_bp = first_bp = block_pointers + bp_mid  - 1;

            *first_bp = new value_type[block_size]
            front_ptr = back_ptr = *first_bp + block_size / 2;
            *back_ptr = entry;
            return;

        }
        
        // There is at least one empty slot after the entry
        // that back_ptr points to (in the same data block)
        else if (back_ptr != ((*last_bp) + (block_size - 1)))
        {
            // STUDENT WORK...

        }
        
        // Data block has no room left after back_ptr;
        // however, the array of block pointers has at least one available slot
        // below last_bp to allocate a new data block
        else if ((back_ptr == ((*last_bp) + (block_size - 1))) && (last_bp != block_pointers_end))
        {
            // STUDENT WORK...

        }
        
        // Data block has no room left after back_ptr;
        // and the array of block pointers has no available slot after last_bp
        else if ((back_ptr == ((*last_bp) + (block_size - 1))) && (last_bp == block_pointers_end))
        {
            // STUDENT WORK...

        }

        */
        
        if (isEmpty())
        {
            first_bp = last_bp = block_pointers + (bp_array_size / 2);
            *first_bp = new value_type [block_size];
            front_ptr = back_ptr = *first_bp;
            *back_ptr = entry;
            return;
        }
        
        if (back_ptr == (*last_bp + block_size - 1))
        {
            if (last_bp == block_pointers_end)
            {
                reserve();
            }
            
            ++last_bp;
            
            if (*last_bp == NULL)
            {
                *last_bp = new value_type [block_size];
            }
            
            back_ptr = *last_bp;
            *back_ptr = entry;
            return;
        }
        
        ++back_ptr;
        *back_ptr = entry;        
    }
    
    
    // POP_FRONT FUNCTION
    template <class Item>
    void deque<Item>::pop_front () {
        
        assert(!isEmpty());
        
        if (front_ptr == back_ptr)
        {
            *first_bp = NULL;
            front_ptr = back_ptr = NULL;
            return;
        }
        
        if (front_ptr == (*first_bp + block_size - 1))
        {
            *first_bp = NULL;
            ++first_bp;
            front_ptr = *first_bp;
            return;
        }
        
        ++front_ptr;
    }
    
    
    // POP_BACK FUNCTION
    template <class Item>
    void deque<Item>::pop_back () {
        
        assert(!isEmpty());
        
        if (front_ptr == back_ptr)
        {
            *last_bp = NULL;
            front_ptr = back_ptr = NULL;
            return;
        }
        
        if (back_ptr == *last_bp)
        {
            *last_bp = NULL;
            --last_bp;
            back_ptr = *last_bp + (block_size - 1);
            return;
        }
        
        --back_ptr;
    }
    
    
    // Constructs an itertor which points to the
    // first element of the deque
    template <class Item>
    typename deque<Item>::iterator deque<Item>::begin()
    {
        value_type* tmp_cursor = NULL;
        value_type* tmp_current_boundary = NULL;
        value_type** tmp_current_block_pointer = NULL;
        if (front_ptr != NULL)
        {
            tmp_cursor = front_ptr;
            tmp_current_block_pointer = first_bp;
            tmp_current_boundary = (*first_bp) + (block_size - 1);
            std::cout << first_bp << std::endl;
            std::cout << *first_bp << std::endl;
            std::cout << front_ptr << std::endl;
            std::cout << tmp_current_boundary << std::endl;
        }
        return iterator(block_pointers, block_pointers_end, first_bp, last_bp,
            front_ptr, back_ptr,
            bp_array_size, block_size,
            tmp_cursor, tmp_current_boundary,
            tmp_current_block_pointer);
    }

    // Constructs an itertor which points to the past the
    // last element of the deque
    template <class Item>
    typename deque<Item>::iterator deque<Item>::end()
    {
        value_type* tmp_cursor = NULL;
        value_type* tmp_current_boundary = NULL;
        value_type** tmp_current_block_pointer = NULL;
        return iterator(block_pointers, block_pointers_end, first_bp, last_bp,
        front_ptr, back_ptr,
        bp_array_size, block_size,
        tmp_cursor, tmp_current_boundary,
        tmp_current_block_pointer);
    }
}

#endif
