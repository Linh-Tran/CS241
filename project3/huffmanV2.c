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
int isLeaf(struct TreeNode* head);
int compareTreeNodes(struct TreeNode* newNode, struct TreeNode* head);
void printList(struct PriorityQ* head);
struct PriorityQ* pop (struct PriorityQ* head);
void encodeFile(FILE* in, FILE* out, int printTable);
void decodeFile(FILE* in, FILE* out);
void buildHuffTree (struct PriorityQ* head);
unsigned long sumTNodes (struct PriorityQ* head);
//struct TreeNode* sumTNodes (struct TreeNode* n1, struct TreeNode* n2);
int listLength (struct PriorityQ* head);

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
  printf("Symbol       Freq       Code\n");
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
  buildHuffTree(head); 
  return head;
  
}

struct PriorityQ* insertSorted(struct PriorityQ* head, unsigned char sym, 
  unsigned long weight)
{
  //access the treeNode that points to the head of the queque
  struct TreeNode* newTNode = createNodeTree(sym, weight);
  struct PriorityQ* newNode; 
  //if the head is null or the weight greater then the current weight then
  if( head == NULL || (head!= NULL && compareTreeNodes(newTNode,head->Tnode))
  {
    //create new listNode that points to a tree node
    newNode = createListNode(newTNode);
    newNode->next = head;
    return newNode;
  }
  else
  {
    //else weight of the head is less then symbol
    //then shift and find next spot to place the newNode
    head->next = insertSorted(head->next,sym,weight);
    return head;
  }
}
int isLeaf(struct TreeNode* head)
{
  if(head->left == NULL && head->right == NULL) return 1;
  return 0;
}
//compare two tree nodes return 1 if the newNode > then head node
int compareTreeNodes(struct TreeNode* newNode, struct TreeNode* head)
{
  if(newNode->weight < head->weight) return 1; //replace in front Q
  if(newNode->weight > head->weight) return 0; //not Q
  //weights are same
  else
  {
    //when one of them don't have a sym, go to the left and compare Q
    if(isLeaf(newNode) && !isLeaf(head))
    { //if the newNode is a leaf and head is not
      //compare the head's left node is greater than the newNode
      //set inside q
      if(newNode->sym > head->left->sym ) return 1;
      return 0;
    }

    if(!isLeaf(newNode) && isLeaf(head))
    { //if the head is a leaf and newNode is not
      if(newNode->left->sym > head->sym) return 1;
      return 0;
    }
    if(!isLeaf(newNode) && !isLeaf(head))
    { //if both are not leaves compare their left children
      if(newNode->left->sym > head->left->sym) return 1;
      return 0;
    }
  }
  //if both are leaves just compare their symbols
  if(newNode->sym > head->sym) return 1;
  return 0;
}

void printList(struct PriorityQ* head)
{
//  unsigned long weight =  head->Tnode->weight;
//  unsigned char sym = head->Tnode->sym;
  while (head!= NULL)
  {
    printf("  %c          ", head->Tnode->sym);
    printf("%lu           ", head->Tnode->weight);
    printf("\n");
    head = head->next;
  }
  printf("\n");
}

//pop the first two treeNodes off the 
struct PriorityQ* pop (struct PriorityQ* head)
{
  struct PriorityQ* node1 = head;
  struct PriorityQ* node2 = head->next;
  free(node1->Tnode); 
  free(node1);
  head =  node2->next;
  free(node2->Tnode);
  free(node2);
  return head;
}

void buildHuffTree (struct PriorityQ* head)
{
  while((head->next)!= NULL)
  {  
    if(listLength(head)<= 1) break;
    struct TreeNode* n1 = head->Tnode;
    struct TreeNode* n2 = head->next->Tnode;
    unsigned long sum = n1->weight + n2->weight;
    struct TreeNode* newTNode = createNodeTree('\0',sum);
    newTNode->left = n1;
    newTNode->right = n2; 
    //struct TreeNode* newTNode = struct TreeNode*(sumTreeNodes(head->Tnode, head->next->Tnode));
    //struct PriorityQ* newLNode= createListNode(newTNode);
    pop(head);
    insertSorted(head, newTNode->sym,newTNode->weight);
  }
    printList(head);
}

int listLength (struct PriorityQ* head)
{
  struct PriorityQ* current = head;
  int length = 0;
  while (current!= NULL)
  {
    length++;
    current = current->next;
  } 
  return length;
}

unsigned long sumTNodes (struct PriorityQ* head)
{
  unsigned long sum = 0;
  sum += head->Tnode->weight;
  sum += head->next->Tnode->weight;
  return sum;
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
