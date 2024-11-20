#include "LinkedListStack.h"

// 스택 생성 함수
void LLS_CreateStack(LinkedListStack **Stack)
{
	// 스택을 자유 저장소에 생성
	(*Stack) = (LinkedListStack*)malloc(sizeof(LinkedListStack));
	(*Stack)->List = NULL;  //헤드
	(*Stack)->Top = NULL;   //테일
}

// 스택 소멸 함수 (자유저장소(동적할당) 소멸 함수)
void LLS_DestroyStack(LinkedListStack* Stack)
{
	while (!LLS_IsEmpty(Stack))
	{
		Node* Popped = LLS_Pop(Stack);
		LLS_DestroyNode(Popped);
	}

	// 스택을 자유 저장소에서 해제
	free(Stack);
}

// 노드 생성 함수
Node* LLS_CreateNode(char* NewData)
{
	Node* NewNode = (Node*)malloc(sizeof(Node));
	NewNode->Data = (char*)malloc(strlen(NewData) + 1);

	strcpy(NewNode->Data, NewData);	 //데이터를 저장한다.

	NewNode->NextNode = NULL;  //다음 노드에 대한 포인터는 NULL로 초기화한다.

	return NewNode;  //노드의 주소를 반환한다.
}

// 노드 소멸 함수 (자유저장소(동적할당) 소멸 함수)
void LLS_DestroyNode(Node* _Node)
{
	free(_Node->Data);
	free(_Node);
}

// 노드 삽입 함수
void LLS_Push(LinkedListStack* Stack, Node* NewNode)
{
	if (Stack->List == NULL)
	{
		Stack->List = NewNode;
	}
	else
	{
		// 스택의 Top에 신규 노드를 연결한다.
		Stack->Top->NextNode = NewNode;
	}

	// 스택의 Top 필드에 새 노드의 주소를 등록한다.
	Stack->Top = NewNode;
}

// 노드 제거 함수
Node* LLS_Pop(LinkedListStack* Stack)
{
	// LLS_Pop() 함수가 반환할 최상위 노드 저장
	Node* TopNode = Stack->Top;


	if (Stack->List == Stack->Top)
	{
		Stack->List = NULL;
		Stack->Top = NULL;
	}
	else
	{
		// Top 아래에 있던 노드를 새로운 Current Top에 저장
		Node* CurrentTop = Stack->List;
		while (CurrentTop != NULL && CurrentTop->NextNode != Stack->Top)
		{
			CurrentTop = CurrentTop->NextNode;
		}

		// CurrentTop를 Top에 저장
		Stack->Top = CurrentTop;
		Stack->Top->NextNode = NULL;
	}

	return TopNode;
}

// 스택 최상위 노드(테일) 반환 함수
Node* LLS_Top(LinkedListStack* Stack)
{
	return Stack->Top;
}

// 스택 크기 반환 함수
int LLS_GetSize(LinkedListStack* Stack)
{
	int Count = 0;
	Node* Current = Stack->List;

	while (Current != NULL)
	{
		Current = Current->NextNode;
		Count++;
	}

	return Count;
}

// 스택이 비어 있으면 1을 반환
// 스택이 비어 있지 않으면 0을 반환하는 함수
int LLS_IsEmpty(LinkedListStack* Stack)
{
	return (Stack->List == NULL);
}
