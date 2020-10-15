// We can use stl container list as a double 
// ended queue to store the cache keys, with 
// the descending time of reference from front 
// to back and a set container to check presence 
// of a key. But to fetch the address of the key 
// in the list using find(), it takes O(N) time. 
// This can be optimized by storing a reference 
//     (iterator) to each key in a hash map. 

#include "stdafx.h"
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <sstream>

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


    <<<<<   RAVI : Modified code as per below comment                                             >>>>>
    <<<<<   NOTE: Its better, if we stored keys in the DLL, instead of values as we could have    >>>>>
    <<<<<   same value stored in differnt keys, hence storing values in DLL is not recommended    >>>>>
    <<<<<   Algorithm considered in this LRU cache implementaiton                                 >>>>>
    <<<<<   LRU node will be at the end of DQ and MRU node will be at the start of the DEQ        >>>>>
    <<<<<   case 1 - always add the node at the begining of the DLL                               >>>>>
    <<<<<   case 2 - move a node to the begining, when referred                                   >>>>>
    <<<<<   case 3 - remove LRU node and make space, if max cache size reached                    >>>>>

 */

class PLRUCache {    
   
private:

    struct PageData
    {
        int key;                    // changed to store key instead of value
        struct PageData * left;
        struct PageData * right;
    };

    unordered_map<int,string> pHashTable;   // changed to store <key,value> instead of <key,node_address>

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
        while (end != NULL) RemoveLRU();
        start = end = NULL;
        pHashTable.clear();
    }

    /*
    *  this will remove the last node in the DLL (Double Ended Queue)
    */
    int RemoveLRU()
    {
        int key = -1;
        if (end) {
            PageData* temp = end->left;
            if(temp) temp->right = NULL;
            key = end->key;
            delete end;
            end = temp;
        }
        return key;
    }
    
    /*
    *  this will add a new key in the hash table and a new node in DLL
    */
    void Add(int key, string value)
    {               
        if (start == NULL)  //case 1 - always add at the begining of the DLL
        {
            //first page
            PageData * node=new PageData();
            node->key = key;

            //update the Table
            pHashTable.insert(pair<int, string>(key, value));
            //update dll
            start = node;
            end = node;
            node->left = NULL; // prev points back to head
            node->right = NULL;
        }
        else 
        {
            //check if we already have this key in the Table first
            unordered_map<int, string>::iterator pHashTableItr = pHashTable.find(key);

            if (pHashTableItr != pHashTable.end()) {    //if found
                cout << "key alredy exists";
                return;
            }

            //case 3 - remove LRU and make space

            //check if max size reached
            if (pHashTable.size() >= cachesize) {
                //remove the last node (LRU) in DLL and make space
                int ikey = RemoveLRU();

                //now remove the pair(key-value) from Table
                pHashTable.erase(ikey);
            }

            //case 1 - always add at the begining of the DLL

            if (pHashTableItr == pHashTable.end()) {    //if not found

                // add to front

                //2nd....nth page
                PageData* node = new PageData();
                node->key = key;
                pHashTable.insert(pair<int, string>(key, value));

                node->right = start;
                node->right->left = node;   //dont forget to set this.
                node->left = NULL;
                start = node;
            }
        }              
    }
    
    /*
    *  case 2 - move a node to the begining, when referred
    */
    void Refer(int key)
    {
        cout << endl;

        cout << "refer : key:" << key << endl;

        unordered_map<int, string>::iterator pHashTableItr = pHashTable.find(key);

        if (pHashTableItr == pHashTable.end())
            cout << "key not found";
        else
        {
            cout << "key:" << pHashTableItr->first << '\t';
            cout << "value:" << pHashTableItr->second << endl;

            //simply find the key in DLL and move it to the start accordingly 
            SwitchPositionInDLL(key);
        }
    }

    /*
    *  this moves the nodes to the starting of the DLL when referred
    */
    void SwitchPositionInDLL(int key)
    {        
        PageData* temp;

        if (end->key == key)    //if key is at the end
        {
            temp = end->left;
            //swap actual changes
            end->left = NULL;
            end->right = start;
            start->left = end; 
            temp->right = NULL;
            //swap start & end
            start = end;
            end = temp;
        }
        else   //if key is in the middle
        {
            //traverse to the key node
            PageData* node = end->left;
            while (node->key != key) node = node->left;

            //swap actual changes
            node->right->left = node->left;
            node->left->right = node->right;

            node->left = NULL;
            node->right = start;

            start->left = node;
            //swap just the start
            start = node;
        }
    }

    void PrintcacheDLL()
    {
        cout << endl;
        cout << "keys in DLL:";

        PageData* node = start;
        while (node != NULL)
        {
            //_itoa(node->key, buffer, 10);
            //str = '\t' + buffer + str ;
            cout << '\t' << node->key;
            node = node->right;
        }        
    }

    void printcache()
    {
        cout << endl;

        unordered_map<int,string>::iterator pHashTableItr;

        for (pHashTableItr = pHashTable.begin(); pHashTableItr != pHashTable.end(); pHashTableItr++)
        {
            cout << "key:" << pHashTableItr->first <<'\t';
            cout << "value:" << pHashTableItr->second << endl;
        }
    }
};


// Driver Code 

int main()
{
    PLRUCache pCache(4);
    pCache.Add(1, "Google");
    pCache.PrintcacheDLL();     //  1
    pCache.Add(2, "Microsoft");
    pCache.PrintcacheDLL();     //  2   1
    pCache.Add(3, "Amazon");
    pCache.PrintcacheDLL();     //  3   2   1

    pCache.Refer(1);
    pCache.PrintcacheDLL();     //  1   3   2
    pCache.Refer(2);
    pCache.PrintcacheDLL();     //  2   1   3
    pCache.Refer(3);
    pCache.PrintcacheDLL();     //  3   2   1

    pCache.Add(4, "Tesla");
    pCache.PrintcacheDLL();     //  4   3   2   1
    pCache.Add(5, "Roku");
    pCache.PrintcacheDLL();     //  5   4   3   2   //1 removed

    pCache.Refer(1);            //  key not found
    pCache.Refer(2);
    pCache.PrintcacheDLL();     //  2   5   4   3
    pCache.Refer(4);
    pCache.PrintcacheDLL();     //  4   2   5   3

    pCache.printcache();

    return 0;
}