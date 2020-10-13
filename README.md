# Cache 

data structure brainstorm...
-------------------------------

*  Hash - Not indexed , key value - buckets
*  map - indexed
*  linked list - begin, end
*  un_ordered map - un sorted..no beging/end index, begin/end markers
*  stack - LIFO - good for case 1..new entry 
*  Q - FIFO - case 3- delete LRU(least recently used)
*  DQ - double ended queue- popular data structure.
* 
* 

Better solution
-----------------

   So our Implementation of LRU cache will have HashMap and Doubly LinkedList.

   In Which HashMap will hold the keys and address of the Nodes of Doubly LinkedList .

   And Doubly LinkedList ( dll) will hold the values of keys  D Queue

    As We need to keep track of Recently used entries, We will use a clever approach.

    We will remove element from bottom and add element on start of LinkedList and whenever any entry is accessed , it will be moved to top.
    so that recently used entries will be on Top and Least used will be on Bottom.
