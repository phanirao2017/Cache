// We can use stl container list as a double 
// ended queue to store the cache keys, with 
// the descending time of reference from front 
// to back and a set container to check presence 
// of a key. But to fetch the address of the key 
// in the list using find(), it takes O(N) time. 
// This can be optimized by storing a reference 
//     (iterator) to each key in a hash map. 

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <unordered_map>

using namespace std;

/*
* 
*  Hash - Not indexed , key value - buckets
*  map - indexed
*  linked list - begin, end
*  un_ordered map - un sorted..no beging/end index, begin/end markers
*  stack - LIFO - good for case 1..new entry 
*  Q - FIFO - case 3- delete LRU(least recently used)
*  DQ - double ended queue- popular data structure.
* 
* 
   Author: PHANI

   So our Implementation of LRU cache will have HashMap and Doubly LinkedList.

   In Which HashMap will hold the keys and address of the Nodes of Doubly LinkedList .

   And Doubly LinkedList ( dll) will hold the values of keys  D Queue

    As We need to keep track of Recently used entries, We will use a clever approach.

    We will remove element from bottom and add element on start of LinkedList and whenever any entry is accessed , it will be moved to top.
    so that recently used entries will be on Top and Least used will be on Bottom.

   */

class PLRUCache {
    
   
private:

    struct PageData
    {
        string page;
        struct PageData * left;
        struct PageData * right;
    };

    unordered_map<int,PageData*> pHashTable;

    PageData* start, *end; // Dll  

    int cachesize;
    
public:
    PLRUCache(int size)
    {
        cachesize = size;
        start = NULL;
        end = NULL;
    }
    ~PLRUCache()
    {

    }

    //case 1
    void Add(int key,string value)
    { 
              
        if (start == NULL)
        {
            //first page
            PageData * node=new PageData();
            node->page = value;

            //update the Table
            pHashTable.insert(pair<int, PageData*>(key, node));
            //update dll
            start = node;
            node->left = start; // prev points back to head
            node->right = node;
            end = node;
        }
        
        // add to front

        //2nd....nth page
        PageData* node = new PageData();
        node->page = value;
        pHashTable.insert(pair<int, PageData*>(key, node));

        node->right = start;
        node->left = NULL;
        start = node;
        
    }

    // case 2

    void Refer(int key)
    {
        cout << endl;

        cout << "refer : key:" << key << endl;

        unordered_map<int, PageData*>::iterator pHashTableItr = pHashTable.find(key);


        if (pHashTableItr == pHashTable.end())
            cout << "key not found";
        else
        {
            cout << "key:" << pHashTableItr->first << '\t';
            cout << "value:" << pHashTableItr->second->page << endl;

             ////make it start, first link the prev to next

            //pHashTableItr->second->left->right = pHashTableItr->second->right;
            //pHashTableItr->second->right->left = pHashTableItr->second->left;

            ////fresh start node

            start = pHashTableItr->second;



        }
    }

    void printcache()
    {
        cout << endl;

        unordered_map<int,PageData*>::iterator pHashTableItr;

        for (pHashTableItr = pHashTable.begin(); pHashTableItr != pHashTable.end(); pHashTableItr++)
        {
            cout << "key:" << pHashTableItr->first <<'\t';
            cout << "value:" << pHashTableItr->second->page << endl;
        }
    }
};


// Driver Code 

int main()
{
    PLRUCache pCache(100);
    pCache.Add(1, "google");
    pCache.Add(2, "MS");
    pCache.Add(3, "Amazon");
    pCache.Add(4, "Tesla");
    pCache.Add(5, "roku");

     pCache.Refer(31);

    pCache.printcache();

    pCache.Refer(3);

    pCache.printcache();

    return 0;
}