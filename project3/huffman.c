/***********************************************************************/
/* Linh Tran                                                            */   
/* Date: November 3, 2014                                               */
/* CS-241 Section # 1                                                   */
/* Program: Huffman coding                                              */
/*                                                                      */
/* This program reads input a file using file.IO. Purpose is to         */
/* create a table base on the estimated probability of frequency        */
/* for each symbol. This program only encodes the files, didn't have    */
/* time to complete decoding.                                           */
/* Huffman coding is a type of compression algorithm.                   */
/*                                                                      */ 
/* The are four main parts to this algorithm:                           */
/* 1. Read the file and get the freq of the symbols store into array    */
/* called ascii[].                                                      */
/* 2. Create a PiorityQ that holds the symbols and their weights.       */
/* 3. Create a tree by having two structs that is a listNode that points*/ 
/* to a treeNode that contains both the symbol and weight. using        */
/* insorted                                                             */
/* sort and pop, listLength, comparing fuction to manage the tree.      */
/* the tree counts weight as the most prioritize item in the tree,      */
/* if the weight are the same the heightest ascii value of the left     */
/* subtree's child is prioritized.                                      */
/* 4. Encoding the file has several steps:                              */
/*    a. tranverse through the tree to get the codes saved into a       */
/*      2D array called huffCode[][]. is saved when reach a leaf in the */
/*      tree.                                                           */
/*    b. write the elements of the table to the file looping          */
/*      through ascii and huffCode[][]. Using fwrite() function.        */
/*    c. encode the codes to file by indivdual bytes at a               */
/*       time. this uses putc() function and strotol().                 */
/***********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

#define CODE_SIZE 256
#define BYTE_LEN 8
unsigned long symCount = 0;
unsigned long ascii[CODE_SIZE]={0};//intialize all entires to 0
char code[CODE_SIZE] ={'\0'};
char huffCode[CODE_SIZE][CODE_SIZE]={'\0'};
int uniqueSyms = 0;

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
void treeTraverse(struct TreeNode* root, char code[CODE_SIZE], 
char huffCode[CODE_SIZE][CODE_SIZE],int loc);
void printChart(char huffCode[CODE_SIZE][CODE_SIZE]);
void writeToFile(FILE* in, FILE* out, char huffCode[CODE_SIZE][CODE_SIZE]);
void encode (FILE *in, FILE* out, char huffCode[CODE_SIZE][CODE_SIZE]);
void freeTree(struct TreeNode* root);
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
  int i = 0;
  while((i=getc(in))!=EOF)
  {
    //checks if the asciiCharValue is a ascii value
    if( i>= 0 && i < CODE_SIZE)
    {  
      //increment the index value stored at that index
      //by 1. 
      ascii[i]++;
    }
    symCount++;//increm the symCount 
  }
}

struct PriorityQ* createListNode(struct TreeNode* Tnode)
{
  //alloc memory for the node using the struct listNode
  struct PriorityQ* listNode = malloc(sizeof(struct PriorityQ));
  listNode->Tnode = Tnode;
  return listNode;
}
/***************************************************/
/* Create a list node for PriorityQ using Tnode    */
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
/* Create a tree node for Binary tree using a      */
/* symbol and its weight                           */
/***************************************************/
struct PriorityQ* setLeafNode(struct PriorityQ* head)
{
  //start with empty priority quequej
  struct PriorityQ* node = head; 
  // int countTreeNode = 0;
  int i;
  for(i= 0; i<CODE_SIZE; i++)
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

/***************************************************/
/* Give the head of the ProrityQ loop throught the */
/* ascii array that holds the symbols and freq     */ 
/* to create the treenode and list node holding the*/
/* treeNode and insert that node into the PriorityQ*/
/***************************************************/
struct PriorityQ* insertSorted(struct PriorityQ* head, 
struct PriorityQ* newNode)
{
  //if the head is NULL or the if head is not NULL and compareTreeNodes 
  //returns 1. Insert the newNode.
  if( head == NULL || (head!= NULL && 
  compareTreeNodes(newNode->Tnode,head->Tnode)))
  {
    newNode->next= head;
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
  if(isLeaf(head))
  {
    return head->sym; 
  }
  leftMostSym(head->left);
  
}

/***************************************************/
/* Compare the head and newNode returns a 1        */
/* if the weight if the newNode< head. or          */
/* if the weights are the same check if the        */
/* either of the nodes are trees then get compare  */
/* the greater symbol. Whenever the newNode        */
/* is greater than the head return 1.              */
/***************************************************/
int compareTreeNodes(struct TreeNode* newNode, struct TreeNode* head)
{
  //compare two tree nodes return 1 if the newNode > then head node
  if(newNode->weight < head->weight) return 1; //replace in front Q
  if(newNode->weight > head->weight) return 0; //not Q
 
  //weights are same
  else
  {
    if(leftMostSym(newNode) > leftMostSym(head)) return 1;
  }
  //if both are leaves just compare their symbols
  return 0;
}

/***************************************************/
/* Helper that calls printTree to print the tree   */
/***************************************************/
void printList(struct PriorityQ* head)
{
  while (head!= NULL)
  {
    printTree(head->Tnode);
    head = head -> next;
  }
}

/***************************************************/
/* Helper that prints the tree                     */
/***************************************************/
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
  //free(node1->Tnode); 
  free(node1);
  head = node2->next;
  //free(node2->Tnode);
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
  //struct PriorityQ* newLNode = createListNode(newTNode);
  newTNode->left = n1;
  newTNode->right = n2; 
  head = pop(head);
  return insertSorted(head,  createListNode(newTNode));
}

/***************************************************/
/* Helper that determines the list length          */
/* used in callbuildHuffTree to determine when     */
/* the tree is completed                           */
/***************************************************/
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

/***************************************************/
/* Helper that determines that finds the           */
/* sum of two treeNodes return the sum             */
/***************************************************/
unsigned long sumTNodes (struct PriorityQ* head)
{
   unsigned long sum = 0;
   sum += head->Tnode->weight;
   sum += head->next->Tnode->weight;
   return sum;
}

/***************************************************/
/* Function tranverse throught the tree to get     */
/* the codes and store the individual codes for a  */
/* sym inside a char array called code at position */
/* loc. when reach the leaf then create            */
/* double array that stores the code[] of that     */
/* particular symbol in the spot of the symbol     */
/* is found. Call printTable suing the treeNode    */
/* root and huffCode double array                  */
/***************************************************/
void treeTraverse(struct TreeNode* root, char code[CODE_SIZE], 
char huffCode[CODE_SIZE][CODE_SIZE], int loc)
{
  if(root->left)
  {
    code[loc] = '0';
    treeTraverse(root->left,code,huffCode,loc+1);
  }
  if(root->right)
  {
    code[loc] = '1';
    treeTraverse(root->right,code,huffCode,loc+1);
  }
  if(isLeaf(root))
  { 
    int i;
    for(i = loc; i<CODE_SIZE; i++)
    {
      code[i] = '\0';
    }
    
    for(i = 0; i<CODE_SIZE; i++)
    {
      huffCode[root->sym][i] = code[i];
    }
  }
}
/***************************************************/
/* Prints the codes in a chart that separated into */
/* three columns for the symbol, freq, and code    */
/* and the total chars.                            */
/* codes are from the huffCode[][]. freq from      */
/* ascii[].                                        */
/* 1. loop through ascii[] if ascii at index i     */
/* is greater than 0                               */
/* 2. check the range of the syms if btwn [32,126] */
/* print as a char otherwise print with "=#at that */
/* index". Print the codes by using %s at          */
/* huffCode[i]                                     */
/* 3. when down print the symCount                 */
/***************************************************/
void printChart(char huffCode[CODE_SIZE][CODE_SIZE])
{
  printf("Symbol\tFreq\tCode\n");
  int i;
  for( i = 0; i< CODE_SIZE; i++)
  {    
    if(ascii[i]>0)
    {
      uniqueSyms++;
      if( i <= 32 || i>126) 
      {
        printf("=%d\t%lu\t%s\n",i,ascii[i],huffCode[i]);
      }
      else  
      { 
        printf("%c\t%lu\t%s\n", i, ascii[i],huffCode[i]);
      } 
    } 
  } 
  printf("Total chars = %lu\n", symCount);//print the total symbols   
}
/***************************************************/
/* Write the Number of symbols, symbols, freq, and */
/* and the total number of encoded symbols to the  */
/* file using fwrite() function. We first          */
/* rewind the file and then write each of those    */
/* values to the file                              */
/***************************************************/
void writeToFile(FILE* in, FILE* out, char huffCode[CODE_SIZE][CODE_SIZE])
{
  unsigned short tableSyms = uniqueSyms;
  unsigned long totalEncode = symCount;
  unsigned char sym;
  unsigned long symFreq;
  rewind(in);
  fwrite(&tableSyms,sizeof(tableSyms),1,out);
  int i;
  for(i = 0; i <CODE_SIZE; i++)
  {
    if(ascii[i]>0)
    {
      sym = i;
      symFreq = ascii[i];
      fwrite(&sym,sizeof(sym),1,out);
      fwrite(&symFreq,sizeof(symFreq),1,out);  
    }
  }
  fwrite(&totalEncode,sizeof(totalEncode),1,out);
}

/***************************************************/
/* Write indiv bytes to the file at a time         */
/* using an char array starting bits inserted at   */
/* the end of the array.                           */
/***************************************************/
void encode (FILE *in, FILE* out, char huffCode[CODE_SIZE][CODE_SIZE])
{

  char binArray[BYTE_LEN]={'0'};
  int binIndx = 7;
  int codeIndx=0; //code where in the code
  int c; //characters read in
  int i;
  int binRep = 0;
  rewind(in);
  
  //read in the characters from the file
  while((c=getc(in))!=EOF)
  {
    //while still inside the code
    while(huffCode[c][codeIndx]!='\0')
    {
      //if the index of the binary array is greater than
      //or equal to zero then insert the bits into the 
      //binArray, increment the code index
      //decrement the binIndx
      //insert the bits starting from the right to left
      if(binIndx >= 0)
      {
        //if(binIndx==2)printf("%s\n", binArray);
        binArray[binIndx] = huffCode[c][codeIndx];
        codeIndx++; 
        binIndx--;
      }
      
      //if the binIndx is done filling up the binArray
      //then print out byte to the file using putc
      //resets the binIndx to start at the end of the
      //binArray.
      if(binIndx< 0)
      {
        //printf("%s", binArray);
        binRep = strtol(binArray, NULL,2);
        putc(binRep,out);
        binIndx = 7;
        //for loop through binArray to clear
        //array
        for(i = 0; i < BYTE_LEN; i++)
        {
          binArray[i] = '0';
        }
      }
    }
    //if code is null then the code is 
    //finish for that symbol reset the codeIndx to 0.
    if(huffCode[c][codeIndx]=='\0')
    {
      codeIndx = 0;
    }
  }
  //for the case where reach the end of the file
  //and the bits read in doesn't fill up the byte
  //print the array with padding 0s.
  if(binIndx !=7)
  {
    binRep = strtol(binArray, NULL, 2);
    putc(binRep,out);
  }
}
void freeTree ( struct TreeNode* root)
{
  if(root != NULL)
  {
    freeTree (root-> left );
    freeTree (root-> right );
    free (root);
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
  head = callBuildHuffTree(head);
  treeTraverse(head->Tnode,code,huffCode,0);
  printChart(huffCode);
  writeToFile(in,out,huffCode);
  encode(in,out,huffCode);
  //freeTree(head->Tnode);
  
}

/***************************************************/
/* Decode a Huffman encoded file.                  */
/* in -- File to decode.                           */
/* out -- File where decoded data will be written. */
/***************************************************/
void decodeFile(FILE* in, FILE* out)
{
	
}

