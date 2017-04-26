#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

#define CODE_SIZE 256

unsigned long symCount = 0;
unsigned long ascii[256]={0};//intialize all entires to 0
//unsigned long code[256]={0};
char* huffTable;
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

struct HuffmanCode
{
  struct HuffmanCode* next;
  struct TreeNode* Tnode;
};
//functions declarations
void frequencyCount (FILE * in);
struct PriorityQ* createListNode(struct TreeNode* Tnode);
struct TreeNode* createNodeTree(unsigned char sym, unsigned long weight);
struct PriorityQ* setLeafNode(struct PriorityQ* head);
struct PriorityQ* insertSorted(struct PriorityQ* head, struct PriorityQ* newNode);
int isLeaf(struct TreeNode* head);
int compareTreeNodes(struct TreeNode* newNode, struct TreeNode* head);
void printList(struct PriorityQ* head);
void printTree(struct TreeNode* root);
struct PriorityQ* pop (struct PriorityQ* head);
struct PriorityQ* callBuildHuffTree(struct PriorityQ* head);
struct PriorityQ* buildHuffTree (struct PriorityQ* head);
int listLength (struct PriorityQ* head);
unsigned long sumTNodes (struct PriorityQ* head);
struct HuffmanCode* createCode (struct TreeNode* treeNode);
char* treeTraverse(struct TreeNode* root, unsigned long code[], int loc);
void printCodes(struct TreeNode* root,unsigned long code[], int loc);
void printTable(struct TreeNode* root, unsigned long code[], int loc);
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
  // printf("Symbol       Freq       Code\n");
  // for( i = 0; i< 256; i++)
  // {
  //   char c = i;//save index as its char value
    
  //   //if the index is out of range print as unprintable
  //   if(ascii[i]!=0)
  //   {
  //     if( i <= 32 || i>126) 
  //     {
  //       printf("=%d          %lu\n",i,ascii[i]);
  //     }
  //     //else print the the index, character, frequency
  //     else
  //     { 
  //       printf("%c            %lu\n", c, ascii[i]);
  //     } 
  //   }
     
  // } 
  // printf("Total Count: %lu\n" , symCount);//print the total symbols 
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

struct HuffmanCode* createCodeNode(struct TreeNode* Tnode)
{
  struct HuffmanCode* codeNode = malloc(sizeof(struct HuffmanCode));
  codeNode->Tnode = Tnode;
  return codeNode; 
}
struct PriorityQ* setLeafNode(struct PriorityQ* head)
{
  //start with empty priority quequej
  struct PriorityQ* node = head; 
  // int countTreeNode = 0;
  int i;
  for(i= 0; i<256; i++)
  {
    unsigned long weight = ascii[i];
    unsigned char sym = i;
    //look to see if you need to create a listNode for that symbol
    if(ascii[i]!=0)
    {
      //set the head to the new created list node
      struct TreeNode* newTNode = createNodeTree(sym, weight);
      struct PriorityQ* newNode = createListNode(newTNode);
      head = insertSorted(head,newNode);
      //printf(" line 123 head: %c , %lu \n", head->Tnode->sym, head->Tnode->weight);
      // countTreeNode++;
    }
  }
  return head;
  
}
struct PriorityQ* insertSorted(struct PriorityQ* head, struct PriorityQ* newNode)
{
  //if the head is null or the weight greater then the current weight then
  if( head == NULL || (head!= NULL && compareTreeNodes(newNode->Tnode,head->Tnode)))
  {
    newNode->next= head;
    //printf(" line 143 head: %c , %lu \n", newNode->Tnode->sym, newNode->Tnode->weight);
    return newNode;
  }
  else
  {
    //else weight of the head is less then symbol
    //then shift and find next spot to place the newNode
    head->next = insertSorted(head->next,newNode);
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
      if(newNode->sym > head->left->sym) return 1;
    }
    if(!isLeaf(newNode) && isLeaf(head))
    { //if the head is a leaf and newNode is not
      if(newNode->left->sym > head->sym) return 1;
    }
    if(!isLeaf(newNode) && !isLeaf(head))
    { //if both are not leaves compare their left children
      if(newNode->left->sym > head->left->sym) return 1;
    }
    if(newNode->sym > head->sym) return 1;
  }
  //if both are leaves just compare their symbols
  return 0;
}
void printList(struct PriorityQ* head)
{
  while (head!= NULL)
  {
    printf("  %c          ", head->Tnode->sym);
    printf("%lu           ", head->Tnode->weight);
    printf("\n");
    printTree(head->Tnode);
    head = head -> next;
  }
}

void printTree(struct TreeNode* root)
{
  if(!isLeaf(root))
  {
    printf("Head root: %c , %lu \n", root->sym, root->weight);
    printf("Left: %c, %lu \n", root->left->sym, root->left->weight);
    printf("Right: %c, %lu \n", root->right->sym, root->right->weight);
    printTree(root->left);
    printTree(root->right);
  }
}
struct PriorityQ* pop(struct PriorityQ* head)
{
  struct PriorityQ* node1 = head;
  struct PriorityQ* node2 = head->next;
  //free(node1->Tnode); 
  free(node1);
  head = node2->next;
  //free(node2->Tnode);
  free(node2);
  return head;
}

struct PriorityQ* callBuildHuffTree(struct PriorityQ* head)
{
  while(head->next!= NULL)
  {
    if(listLength(head)<= 1) break;
    head = buildHuffTree(head);
  }
  return head;
}

struct PriorityQ* buildHuffTree (struct PriorityQ* head)
{
  struct TreeNode* n1 = head->Tnode;
  struct TreeNode* n2 = head->next->Tnode;
  struct TreeNode* newTNode = createNodeTree('\0',sumTNodes(head));
  struct PriorityQ* newLNode = createListNode(newTNode);
  newTNode->left = n1;
  newTNode->right = n2; 
  head = pop(head);
  return insertSorted(head, newLNode);
}

int listLength (struct PriorityQ* head)
{
  struct PriorityQ* current = head;
  int length = 0;
  while (current!= NULL)
  {
    length++;
    current = current->next;
    //printf("length %d\n", length);
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

void treeTraverse(struct TreeNode* root, unsigned long code[], int loc)
{
  if(root->left)
  {
    code[loc] = 0;
    treeTraverse(root->left,code,loc+1);
  }
  if(root->right)
  {
    code[loc]=1;
    treeTraverse(root->right,code,loc+1);
  }
  if(isLeaf(root))
  {
    //printf("%c: ", root->sym);
    printTable(root,code,loc);
    //return code;
    //printCodes(root,code,loc);
  }
}

void printCodes(struct TreeNode* root,unsigned long code[], int loc)
{ 
  int i;
  for(i = 0; i <loc; i++)
  {
    printf("%lu", code[i]);
  }
  printf("\n");
}

void printTable(struct TreeNode* root, unsigned long code[], int loc)
{
  int i;
  for( i = 0; i< 256; i++)
  {
    char c = i;//save index as its char value

    //if the index is out of range print as unprintable
    if(ascii[i]!=0 && c == root->sym)
    {
      if( i <= 32 || i>126) 
      {
        printf("=%d          %lu          ",i,ascii[i]);
      }
      //else print the the index, character, frequency
      else  
      { 
        printf("%c            %lu          ", c, ascii[i]);
      } 
      printCodes(root,code,loc);
    } 
  }   
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
  struct PriorityQ* head = NULL;
  head = setLeafNode(head);
  //printList(head); 
  head = callBuildHuffTree(head);
  //printList(head); 
  printf("Symbol       Freq       Code\n");
  treeTraverse(head->Tnode,code,0);
  printf("Total Count: %lu\n" , symCount);//print the total symbols 
}

/***************************************************/
/* Decode a Huffman encoded file.                  */
/* in -- File to decode.                           */
/* out -- File where decoded data will be written. */
/***************************************************/
void decodeFile(FILE* in, FILE* out)
{
	
}

