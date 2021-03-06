#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

#define CODE_SIZE 256

unsigned long symCount = 0;
unsigned long ascii[256]={0};//intialize all entires to 0
char code[256] ={'\0'};
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
struct PriorityQ* insertSorted(struct PriorityQ* head, struct PriorityQ* newNode);
int isLeaf(struct TreeNode* head);
unsigned char leftMostSym(struct TreeNode* head);
int compareTreeNodes(struct TreeNode* newNode, struct TreeNode* head);
void printList(struct PriorityQ* head);
void printTree(struct TreeNode* root);
struct PriorityQ* pop (struct PriorityQ* head);
struct PriorityQ* callBuildHuffTree(struct PriorityQ* head);
struct PriorityQ* buildHuffTree (struct PriorityQ* head);
int listLength (struct PriorityQ* head);
unsigned long sumTNodes (struct PriorityQ* head);
void treeTraverse(struct TreeNode* root, char code[256],int loc);
//void printCodes(struct TreeNode* root, char code[], char huffCode[256][256], int loc);
void printTable(struct TreeNode* root, char huffCode[256][256]);
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
    if( asciiCharValue>= 0 && asciiCharValue < 256)
    {  
      //increment the index value stored at that index
      //by 1. 
      ascii[asciiCharValue]++;
    }
    symCount++;//increm the symCount 
  }
}

/***************************************************/
/* Create a list node for PriorityQ using Tnode    */
/***************************************************/
struct PriorityQ* createListNode(struct TreeNode* Tnode)
{
  //alloc memory for the node using the struct listNode
  struct PriorityQ* listNode = malloc(sizeof(struct PriorityQ));
  listNode->Tnode = Tnode;
  return listNode;
}

/***************************************************/
/* Create a tree node for Binary tree using a      */
/* symbol and its weight                           */
/***************************************************/
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

/***************************************************/
/* Give the head of the ProrityQ loop throught the */
/* ascii array that holds the symbols and freq     */ 
/* to create the treenode and list node holding the*/
/* treeNode and insert that node into the PriorityQ*/
/***************************************************/
struct PriorityQ* setLeafNode(struct PriorityQ* head)
{
  //start with empty priorityQ
  struct PriorityQ* node = head; 
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
    }
  }
  return head;
  
}

/***************************************************/
/* Given the head of the ProrityQ and the new list */
/* node find the place to insert by adding the     */
/* new Node only if the head is NULL or if head is */ 
/* not NULL and compareTreeNodes returns 1.        */
/* else continue to recursively call insertSored   */
/* with the next next of the PriorityQ.            */
/***************************************************/
struct PriorityQ* insertSorted(struct PriorityQ* head, struct PriorityQ* newNode)
{
  //if the head is NULL or the if head is not NULL and compareTreeNodes 
  //returns 1. Insert the newNode.
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

/***************************************************/
/* Helper to indentify if a treeNode is a leaf     */
/* by checking if both its children are NULL.      */
/* returns 1 if is Leaf otherwise returns a 0      */
/***************************************************/
int isLeaf(struct TreeNode* head)
{
  if(head->left == NULL && head->right == NULL) return 1;
  return 0;
}
/***************************************************/
/* Helper that identify leftmost symbol of the tree*/
/* returns the leftmost symbol                     */
/***************************************************/
unsigned char leftMostSym(struct TreeNode* head)
{
  if(head->left)
  {
    leftMostSym(head->left);
  }
  return head->sym; 
}

/***************************************************/
/* Compare the head and newNode returns a 1        */
/* if the weight if the newNode< head. or          */
/* if the weights are the same check if the        */
/* either of the nodes are trees then get compare  */
/* the greater symbol. Whenever the newNode        */
/* is greater than the head return 1.              */
/***************************************************/

//compare two tree nodes return 1 if the newNode > then head node
int compareTreeNodes(struct TreeNode* newNode, struct TreeNode* head)
{
  if(newNode->weight < head->weight) return 1; //replace in front Q
  if(newNode->weight > head->weight) return 0; //not Q
 
  //weights are same
  else
  {
    //when one of them don't have a sym, go to the left and compare Q

    // if(isLeaf(newNode) && !isLeaf(head) )
    // { //if the newNode is a leaf and head is not
    //   //compare the head's left node is greater than the newNode
    //   //set inside q
    //   if(newNode->sym > leftMostSym(head)) return 1;
    // }
    // if(!isLeaf(newNode) && isLeaf(head))
    // { //if the head is a leaf and newNode is not
    //   if(leftMostSym (newNode) > head->sym) return 1;
    // }
    // if(!isLeaf(newNode) && !isLeaf(head))
    // { //if both are not leaves compare their left children
    //   if(leftMostSym(newNode) > leftMostSym(head)) return 1;
    // }
    // if(newNode->sym > head->sym) return 1;
    if(isLeaf(newNode) && !isLeaf(head) )
    { //if the newNode is a leaf and head is not
      //compare the head's left node is greater than the newNode
      //set inside q
      if(newNode->sym >head->left->sym) return 1;
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

/***************************************************/
/* Helper that pops off two treeNodes from         */
/* the list at a time and free the newNodes        */
/* and set the head to point to the next item on   */
/* the list                                        */
/***************************************************/
struct PriorityQ* pop(struct PriorityQ* head)
{
  struct PriorityQ* node1 = head;
  struct PriorityQ* node2 = head->next;
  free(node1->Tnode); 
  free(node1);
  head = node2->next;
  free(node2->Tnode);
  free(node2);
  return head;
}

/***************************************************/
/* Helper that loops through as the tree is built  */
/* until the list lenght is one, then return head  */
/* method calls buildHuffTree                      */
/***************************************************/
struct PriorityQ* callBuildHuffTree(struct PriorityQ* head)
{
  while(head->next!= NULL)
  {
    if(listLength(head)<= 1) break;
    head = buildHuffTree(head);
  }
  return head;
}

/***************************************************/
/* Helper builds the huff tree. Algorithm:         */
/* 1. make two TreeNodes one is the head of the    */
/*    list and the n2 is the head's next           */
/* 2. create a treeNode from the sum of the two    */
/*    nodes weights,                               */
/* 3. set the n1,n2 as the children of that newNode*/
/* 4. pop the n1,n2 off the list                   */
/* 5. return the new head by calling insertSorted  */
/*  with the head and newNode                      */
/***************************************************/
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

/***************************************************/
/* Helper that determines by looking               */
/***************************************************/
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

void treeTraverse(struct TreeNode* root, char code[256], int loc)
{
  if(root->left)
  {
    code[loc] = '0';
    treeTraverse(root->left,code,loc+1);
  }
  if(root->right)
  {
    code[loc] = '1';
    treeTraverse(root->right,code,loc+1);
  }
  if(isLeaf(root))
  { 
    int i;
    char huffCode[256][256] = {'\0'};
  
    for(i = loc; i<256; i++)
    {
      code[i] = '\0';
    }
    
    for(i = 0; i<256; i++)
    {
      huffCode[root->sym][i] = code[i];
    }
    //printf("%c: ", root->sym);
    printTable(root,huffCode);
   // printCodes(root,code,huffCode,loc);
  }
}

void printTable(struct TreeNode* root, char huffCode[256][256])
{
  int i;
  for( i = 0; i< 256; i++)
  {
    unsigned char c = i;//save index as its char value
    
    //if the index is out of range print as unprintable
    //moby dick at 187 not defined
    //if(i== 187) printf("ascii 187: %lu\n",ascii[i]);
    if(ascii[i]!=0 && huffCode[i][0]!='\0')
    {
      if( i <= 32 || i>126) 
      {
        printf("=%d\t%lu\t%s\n",i,ascii[i],huffCode[i]);
      }
      //else print the the index, character, frequency
      else  
      { 
        printf("%c\t%lu\t%s\n", c, ascii[i],huffCode[i]);
        //if(huffCode[i][0]!='\0') printf("\t%s", huffCode[i]);
      } 
     // printCodes(root,code,huffCode,loc);
    } 
  }   
}
// void printCodes(struct TreeNode* root, char code[], char huffCode[256][256], int loc)
// { 
//   int i;
//   for(i = 0; i< 256; i++)
//   {
//     if(huffCode[i][0]!='\0')
//     {
//       char c = i;
//       printf("sym: %c: %s",i, huffCode[i]);
//       printf("\n");
//     }
//   }
//   printf("\n");
// }

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
  printf("Symbol\tFreq\tCode\n");
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

