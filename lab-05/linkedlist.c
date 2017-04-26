#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

/***********************************************************************/
/* Linh Tran                                                           */   
/* Date: November 3, 2014                                              */
/* CS-241 Section # 1                                                  */
/* Program: Linked List                                                */
/*                                                                     */
/* This implements the different operations of a linked list, which    */
/* includes creatNode, insert data in a sorted list, pop node, push    */
/* node, print, freeList, and reverse list.                            */
/* The goal to implement all these operations without leaking or losing*/
/* any memory. This file should be run with listtest.c                 */                                 
/*                                                                     */
/***********************************************************************/

/* Alloc a new node with given data. */
struct ListNode* createNode(int data)
{
  struct ListNode* node = malloc(sizeof(struct ListNode));
  node -> data = data; //node points to data
  node -> next = NULL; //last node points to null
  return node;
}

/* Insert data at appropriate place in a sorted list, return new list head. */
struct ListNode* insertSorted(struct ListNode* head, int data)
{ 
	struct ListNode* current;
	struct ListNode* newNode = createNode(data); 
	
  //if the head is pointing to NULL then set head to point to newNode and return head;
  if (head == NULL)
  {
     head = newNode;
     return head;
	}
  //if the head's data is greater than data, place newNode b4 head's data in the linked
  //list. Head will now point to newNode then head's initial data.
	if (head->data > data)
  {
		newNode -> next = head;
    head = newNode;
		return head;
	}
  //set current to point to where head points
  //while the current next is not NULL and the data if current's 
  //next data is < newNode's data
  //then set current to be it's next until current's next data > newNode's data
  //set newNode's next to point where current's next is pointing.
  //set current's next to be where newNode is pointing.
	else
  { current = head;
		while( current->next != NULL && current->next->data < data)
		{
		  current = current->next;
		}
		newNode->next = current->next;
		current->next = newNode;
		return head;
	}
}

/* Remove data from list pointed to by headRef, changing head if necessary.
 * Make no assumptions as to whether the list is sorted.
 * Memory for removed node should be freed.
 * Return 1 if data was present, 0 if not found. */
int removeItem(struct ListNode** headRef, int data)
{
  struct ListNode* current= *headRef;
  struct ListNode* temp = current;
  //if data is found then set the headref
  //to point
  if(current-> data == data)
  {
    *headRef = current -> next;
    free(current);
    return 1;
  }
  while(current!=NULL)
  {
    if(current->data == data)
    {
      temp ->next = current -> next;
      free(current);
      return 1;
    }
    temp = current;
    current = current -> next;
  }
  return 0;  
}

/* Insert data at head of list, return new list head. */
struct ListNode* push(struct ListNode* head, int data)
{
  struct ListNode* newNode = createNode(data);//alloc space for a newNode
	newNode->next = head; //set the new item to point to the head
  head = newNode;
  return head;
}

/* Remove and return data from head of non-empty list, changing head.
 * Memory for removed node should be freed. */
int pop(struct ListNode** headRef)
{
	struct ListNode* newHead = (*headRef)->next;
	int oldData = -1; 
	oldData = (*headRef)->data;
	free(*headRef); 
	*headRef = newHead;
  return oldData;
}

/* Return length of the list. */
int listLength(struct ListNode* head)
{
  struct ListNode* current = head;
  int count = 0;
  
  while ( current != NULL)
  { 
    count++;    
    current = current -> next;    
  }
  return count;
}

/* Print list data on single line, separated with spaces. */
void printList(struct ListNode* head)
{
  struct ListNode* current = head;
  while ( current != NULL)
  {
    printf("%d ", current-> data);
    current = current -> next;
  }
  printf("\n");
}

/* Free memory used by the list. */
void freeList(struct ListNode* head)
{
  struct ListNode* current = head;
  struct ListNode* node = head;
  while ( current != NULL)
  { 
    current = node -> next;
    free(node);
    node = current;
  }
}

/* Reverse order of elements in the list 
 * this function reverse the list in place by
 * changing pointers */
void reverseList(struct ListNode** headRef)
{ 
  struct ListNode * current, *prev, *next;
  current = *headRef;
  prev = NULL;
  //while current is not NULL
  //next holds the current next
  //current next points to prev
  //prev is now current
  //current now goes to next 
  //continue until current is NULL
  //then set head to prev.
  while(current!= NULL)
  {
    next = current->next;
    current->next = prev;
    prev =current;
    current = next;
  }
  *headRef = prev;
}

