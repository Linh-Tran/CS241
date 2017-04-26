#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

/***************************************************/
/* Read in a file and count the frequency occurence*/
/* of each symbol and the total number of symbols  */
/* in the file. Function doesn't return any value  */
/* and takes in the FILE pointer                   */
/***************************************************/
void frequencyCount (FILE * in)
{
  unsigned long symCount =0;
  unsigned long ascii[256]={0};//intialize all entires to 0
  int i;
	char c; 
  while((c=getc(in))!=EOF)
  {
    int asciiCharValue = c;//saves the char value as the ascii value 
    //checks if the asciiCharValue is a ascii value
    if( asciiCharValue>= 0 && asciiCharValue < 127)
    {  
      //increment the index value stored at that index
      //by 1. 
      ascii[asciiCharValue]++;
    }
    symCount++;//increm the symCount 
  }
  for( i = 0; i< 256; i++)
  {
    char c = i;//save index as its char value
    
    //if the index is out of range print as unprintable
    if( i < 32 || i>126) 
	  {
	    printf("=%d: %lu\n",i,ascii[i]);
	  }
	  //else print the the index, character, frequency
	  else{	
     	printf("%d) %c : %lu\n", i, c, ascii[i]);
    }  
 }
  printf("Total Count: %lu\n" , symCount);//print the total symbols
}
struct TreeNode
{
  unsigned long weight;
  struct TreeNode* left;
  struct TreeNode* right;
  unsigned char sym;
}
struct LeafNode
{
  unsigned long weight;
  unsigned char sym;
  struct LeafNode* next;
}
/* Alloc a new node with given data.*/
struct LeafNode* createNodeLeaves(unsigned char sym, unsigned long weight)
{
  //alloc memory for the node using the struct TreeNode
  struct TreeNode* node = malloc(sizeof(struct TreeNode));
  node->sym = sym;
  node->weight= weight; //store weight into the node 
  node->next = NULL;
  return node;
}

struct TreeNode* createNodeTree(unsigned char sym, unsigned long weight)
{
  //alloc memory for the node using the struct TreeNode
  struct TreeNode* node = malloc(sizeof(struct TreeNode));
  node->sym = sym;
  node->weight= weight; //store weight into the node 
  node->left = NULL; //set the left and right node to NULL
  node->right = NULL;//retrun node
  return node;
}
//leaf node and combine tree
//make leaf from 
//priorquity q remove and add and size
//encode is the 1's and 0's 
//decode is the trace through the tree to get the symbol

struct LeafNode* priorityQ (struct TreeNode* head, unsigned char sym, unsigned long weight)
{
  struct LeafNode* current = head;
  struct LeafNode* newNode = createNodeLeaves(sym,weight);
  if(head == NULL)
  {
    head = newNode;
    return head;
  }
  if(head->weight >weight)
  {
    newNode->next = head;
    head = newNode;
    return head;
  }
  else
  { 
    current = head;
    while( current->next != NULL && current->next->weight < weight)
    {
      current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
    return head;
  }
}

struct TreeNode* createTree( struct TreeNode* head, unsigned char sym, unsigned long weight)
{
  
}

struct TreeNode* removeQ(unsigned char sym, unsigned long weight)
{

}

/**************************************************************/
/* Huffman encode a file.                                     */
/* in -- File to encode.                                      */
/*       May be binary, so don't assume printable characters. */
/* out -- File where encoded data will be written.            */
/* printTable -- If not zero, print table of bytes,           */
/*               frequencies, and codes to standard output.   */
/**************************************************************/
void encodeFile(FILE* in, FILE* out, int printTable)
{

}

/***************************************************/
/* Decode a Huffman encoded file.                  */
/* in -- File to decode.                           */
/* out -- File where decoded data will be written. */
/***************************************************/
void decodeFile(FILE* in, FILE* out)
{
	
}
