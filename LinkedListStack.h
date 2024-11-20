#ifndef LINKEDLIST_STACK_H
#define LINKEDLIST_STACK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 노드 구조체
typedef struct tagNode {
	char* Data;
	struct tagNode* NextNode;
} Node;

// 스택 구조체
typedef struct tagLinkedListStack {
	Node* List; //헤드, 스택의 맨 아래
	Node* Top;  //테일, 스택의 맨 위
} LinkedListStack;


// 링크드 리스트 기반 스택과 스택의 노드 표현하는데 사용할 함수 선언
void LLS_CreateStack(LinkedListStack** Stack);
void LLS_DestroyStack(LinkedListStack* Stack);

Node* LLS_CreateNode(char* Data);
void LLS_DestroyNode(Node* _Node);

void LLS_Push(LinkedListStack* Stack, Node* NewNode);
Node* LLS_Pop(LinkedListStack* Stack);

Node* LLS_Top(LinkedListStack* Stack);
int LLS_GetSize(LinkedListStack* Stack);
int LLS_IsEmpty(LinkedListStack* Stack);

#endif
