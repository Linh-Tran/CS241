#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
unsigned long symCount = 0;
unsigned long ascii[256]={0};//intialize all entires to 0

//structs declarations
struct TreeNode
{
  unsigned long weight;
  unsigned char sym;
  struct TreeNode* left;
  struct TreeNode* right; 
};

struct PriorityQ
{
  struct PriorityQ* next;
  struct TreeNode* Tnode;

};

//functions declarations
void frequencyCount (FILE * in);
struct PriorityQ* createListNode(struct TreeNode* Tnode);
struct TreeNode* createNodeTree(unsigned char sym, unsigned long weight);
struct PriorityQ* setLeafNode(struct PriorityQ* head);
struct PriorityQ* insertSorted(struct PriorityQ* head, unsigned char sym, 
  unsigned long weight);
void printList(struct PriorityQ* head);
void encodeFile(FILE* in, FILE* out, int printTable);
void decodeFile(FILE* in, FILE* out);

/***************************************************/
/* Read in a file and count the frequency occurence*/
/* of each symbol and the total number of symbols  */
/* in the file. Function doesn't return any value  */
/* and takes in the FILE pointer                   */
/***************************************************/
void frequencyCount (FILE * in)
{
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
  printf("Symbol       Freq       Weight\n");
  for( i = 0; i< 256; i++)
  {
    char c = i;//save index as its char value
    
    //if the index is out of range print as unprintable
    if( i < 32 || i>126) 
	  {
	    //printf("=%d: %lu\n",i,ascii[i]);
	  }
	  //else print the the index, character, frequency
	  else
    {	
     	//printf("%d) %c : %lu\n", i, c, ascii[i]);
    }  
  } 
  //printf("Total Count: %lu\n" , symCount);//print the total symbols 
}

struct PriorityQ* createListNode(struct TreeNode* Tnode)
{
  //alloc memory for the node using the struct listNode
  struct PriorityQ* listNode = malloc(sizeof(struct PriorityQ));
  listNode->Tnode = Tnode;
  return listNode;
}
struct TreeNode* createNodeTree(unsigned char sym, unsigned long weight)
{
  //alloc memory for the node using the struct treeNode
  struct TreeNode* treeNode = malloc(sizeof(struct TreeNode));
  treeNode->sym = sym;
  treeNode->weight = weight;
  treeNode->left = NULL;
  treeNode->right = NULL;
  return treeNode;
}

struct PriorityQ* setLeafNode(struct PriorityQ* head)
{
  //start with empty priority quequej
  struct PriorityQ* node = head; 
  int countTreeNode = 0;
  int i;
  for(i= 0; i<256; i++)
  {
    unsigned long weight = ascii[i];
    unsigned char sym = i;
    //look to see if you need to create a listNode for that symbol
    if(ascii[i]!=0)
    {
      //set the head to the new created list node
      head = insertSorted(head,sym,weight);
      countTreeNode++;
    }
  }
  printList(head);  
  return head;
  
}

struct PriorityQ* insertSorted(struct PriorityQ* head, unsigned char sym, 
  unsigned long weight)
{
  //access the treeNode that points to the head of the queque.
  struct TreeNode* newTNode = createNodeTree(sym, weight);
  struct PriorityQ* newNode; 
  //if the head is null 
  if( head == NULL)
  {
    //create new listNode that points to a tree node
    newNode = createListNode(newTNode);
    newNode->next = NULL;//set the newNode's next to null
    return newNode;//return the newNode
  }
  else if (weight < head->Tnode->weight)
  {
     //create new listNode that points to a tree node
    newNode = createListNode(newTNode);
    newNode->next = head;
    return newNode;
  }
  //if the weight is equal to the head weight then
  //check if the sym greater than the head sym
  //create a newNode and place it in front of the list.
  else if(weight == head->Tnode->weight)
  {

    if(compareTreeNodes(newTNode,head->Tnode))
    {
      newNode = createListNode(newTNode);
      newNode->next = head;
      return newNode;
    }
    else
    { //1st symbol is less then 2nd symbol
      //newNode = createListNode(newTNode);
       return head->next = insertSorted(head->next,sym,weight);
    }
  }
  else
  {
    //else weight of the head is less then symbol
    //then shift and find next spot to place the newNode
    head->next = insertSorted(head->next,sym,weight);
    return head;
  }
}

int compareTreeNodes(struct TreeNode* newNode, struct TreeNode* head)
{
  //if the new Node is less then the head sym 
  //want to place the newNode
  if(newNode->sym > head->sym)
  {
     return 1;
  }
  else return 0;
}

void printList(struct PriorityQ* head)
{
//  unsigned long weight =  head->Tnode->weight;
//  unsigned char sym = head->Tnode->sym;
  while (head!= NULL)
  {
    printf("%lu- ", head->Tnode->weight);
    printf("%c- ", head->Tnode->sym);
    printf("\n");
    head = head->next;
  }
  printf("\n");
  //printf("%d\n, ")
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
  frequencyCount(in);
  struct PriorityQ* node = NULL;
  setLeafNode(node);
}

/***************************************************/
/* Decode a Huffman encoded file.                  */
/* in -- File to decode.                           */
/* out -- File where decoded data will be written. */
/***************************************************/
void decodeFile(FILE* in, FILE* out)
{
	
}

// int main(int argc, char* argv[])
// {
//   FILE* inputfile = fopen(argv[1], "r");
//   FILE* outputfile = fopen(argv[2], "w");
//   if(argc !=3) printf("Incorrect numbers of Arguements\n");
//   else frequencyCount(in);
//   return 0;
// }
