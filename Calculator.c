#include "Calculator.h"

char NUMBER[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' };

// 인자로 받은 Cipher와 일치하는 숫자가 NUMBER에서 있는지 찾는 함수
// 있으면 1을 반환, 없으면 0을 반환.
int IsNumber(char Cipher)
{
	int i = 0;
	int ArrayLength = sizeof(NUMBER);

	for (i = 0; i < ArrayLength; i++)
	{
		if (Cipher == NUMBER[i])
			return 1;
	}

	return 0;
}

unsigned int GetNextToken(char* Expression, char* Token, int* TYPE)
{
	unsigned int i = 0;

	// Expression의 원소가 연산자일 때 까지 반복(position 인덱스~)
	for (i = 0; 0 != Expression[i]; i++)
	{
		Token[i] = Expression[i]; // 최종적으로 Token에 숫자들 또는 연산자 저장됨.
								  //! 숫자인지 연산자인지 Type에 저장.

		if (IsNumber(Expression[i]) == 1) {  // 숫자인 경우
			*TYPE = OPERAND; //!

			if (IsNumber(Expression[i + 1]) != 1)  // 숫자가 끝나는 지점
				break;
		}
		else {  // 연산자나 괄호인 경우
			*TYPE = Expression[i]; //!
			break;
		}
	}

	Token[++i] = '\0';  // 문자열 종료
	return i;	//GetPostfix함수의 Position변수에 리턴됨. Position변수는 Token의 인덱스.	//Calculate함수의 Read변수에 더하기 연산하여 리턴됨.
}

// 연산의 우선순위 탐색 함수
// 우선순위가 낮을수록 먼저 연산한다.
int GetPriority(char Operator, int InStack)
{
	// 숫자이면 우선순위-1??
	int Priority = -1;

	switch (Operator)
	{
	// 왼쪽 소괄호이고 스택이라면 우선순위3, 왼쪽 소괄호이고 노드라면 우선순위0
	case LEEF_PARENTHESIS :
		if (InStack)
			Priority = 3;
		else
			Priority = 0;
		break;

	// 곱하기, 나누기는 우선순위1
	case MULTIPLY :
	case DIVIDE :
		Priority = 1;
		break;

	// 더하기, 빼기는 우선순위2
	case PLUS :
	case MINUS :
		Priority = 2;
		break;
	}

	return Priority;
}

// 우선 연산하는게 맞는지를 반환하는 함수?
int IsPrior(char OperatorInStack, char OperatorInToken) {
	return (GetPriority(OperatorInStack, 1) > GetPriority(OperatorInToken, 0));
}

// 중위표기식을 후위표기식으로 고치는 함수
void GetPostfix(char* InfixExpression, char* PostfixExpression) {
	LinkedListStack* Stack;
	char Token[32];
	int Type = -1;
	unsigned int Position = 0;
	unsigned int Length = strlen(InfixExpression);

	LLS_CreateStack(&Stack); //스택만들기

	while (Position < Length) {
		Position += GetNextToken(&InfixExpression[Position], Token, &Type);

		// 숫자라면
		if (Type == OPERAND) {
			strcat(PostfixExpression, Token);  // 후위표기식에 숫자 저장O. 스택 푸시X
			strcat(PostfixExpression, " ");
		}
		// 왼쪽 소괄호라면
		else if (Type == LEEF_PARENTHESIS) {
			LLS_Push(Stack, LLS_CreateNode(Token));  // 후위표기식에 저장X. 여는 괄호를 스택에 푸쉬O
		}
		// 오른쪽 소괄호라면
		else if (Type == RIGHT_PARENTHESIS) {
			// 닫는 괄호일 경우, 여는 괄호를 만날 때까지 팝
			while (!LLS_IsEmpty(Stack)) {
				Node* Popped = LLS_Pop(Stack);

				if (Popped->Data[0] == LEEF_PARENTHESIS) {
					LLS_DestroyNode(Popped);
					break;
				}
				else {
					strcat(PostfixExpression, Popped->Data);
					strcat(PostfixExpression, " ");
					LLS_DestroyNode(Popped);
				}
			}
		}
		// 연산자라면
		else {
			while (!LLS_IsEmpty(Stack) &&
				!IsPrior(LLS_Top(Stack)->Data[0], Token[0])) {  //스택이 비어있지 않고, 우선순위가 옳을 동안

				Node* Popped = LLS_Pop(Stack);  // Popped는 스택의 최상위 노드(=현재 무조건 연산자)를 가리키는 포인터이다.

				if (Popped->Data[0] != LEEF_PARENTHESIS) {  //스택의 최상위 연산자가 현재 연산자보다 우선순위가 높거나 같다면 스택에서 팝하여 후위 표기식에 추가.
					strcat(PostfixExpression, Popped->Data);
					strcat(PostfixExpression, " ");
				}

				LLS_DestroyNode(Popped);  // 스택을 팝 하였으므로 노드 소멸 연산. (사용하지 않는 메모리 삭제하기)
			}

			LLS_Push(Stack, LLS_CreateNode(Token));  //현재 연산자는 스택에 푸쉬.
		}
	}

	// 스택에 남아있는 연산자 추가
	// 역할 : 입력 문자열을 모두 처리한 후 스택에 남아 있는 연산자를 후위 표기식에 추가.
	while (!LLS_IsEmpty(Stack)) {

		Node* Popped = LLS_Pop(Stack);  // Popped는 스택의 최상위 노드(=현재 무조건 연산자)를 가리키는 포인터이다.

		if (Popped->Data[0] != LEEF_PARENTHESIS) {   //여는 괄호는 제거하지만 출력하지 않음.
			strcat(PostfixExpression, Popped->Data); //스택에 남아 있는 연산자는 순서대로 후위 표기식으로 출력.
			strcat(PostfixExpression, " ");
		}

		LLS_DestroyNode(Popped);  // 스택을 팝 하였으므로 노드 소멸 연산. (사용하지 않는 메모리 소멸하기)
	}

	LLS_DestroyStack(Stack);  // 모든 스택을 사용하였으므로, 스택 소멸 연산. (사용하지 않는 메모리 소멸하기)
}

// 후위표기식 계산 함수
double Calculate(char* PostfixExpression)
{
	LinkedListStack* Stack;
	Node* ResultNode;

	double Result;
	char Token[32];
	int Type = -1;
	unsigned int Read = 0;
	unsigned int Length = strlen(PostfixExpression);

	LLS_CreateStack(&Stack);

	while (Read < Length)
	{
		Read += GetNextToken(&PostfixExpression[Read], Token, &Type);

		// 띄어쓰기 건너뛰기
		if (Type == SPACE)
			continue;

		// 숫자면
		if (Type == OPERAND)
		{
			Node* NewNode = LLS_CreateNode(Token); //노드 만들고
			LLS_Push(Stack, NewNode);			   //스택에 삽입
		}
		// 연산자면
		else
		{
			char ResultString[32];
			double Operator1, Operator2, TempResult;
			Node* OperatorNode;

			// 스택에서 숫자 두개 빼기
			OperatorNode = LLS_Pop(Stack);
			Operator2 = atof(OperatorNode->Data);
			LLS_DestroyNode(OperatorNode);

			OperatorNode = LLS_Pop(Stack);
			Operator1 = atof(OperatorNode->Data);
			LLS_DestroyNode(OperatorNode);

			// 연산자 종류에 따라 연산하기
			switch (Type)
			{
			case PLUS: TempResult = Operator1 + Operator2; break;
			case MINUS:	TempResult = Operator1 - Operator2; break;
			case MULTIPLY: TempResult = Operator1 * Operator2; break;
			case DIVIDE: TempResult = Operator1 / Operator2; break;
			}

			_gcvt(TempResult, 10, ResultString);			// 문자열로 외어 있는 계산 결과를 소수로 변환하는 내장함수 사용
			LLS_Push(Stack, LLS_CreateNode(ResultString)); 	// 계산결과 노드 만들고 스택에 삽입
		}
	}

	ResultNode = LLS_Pop(Stack);	  // 스택 맨 위(최상위노드) 가리키기
	Result = atof(ResultNode->Data);  // 소수로 되어 있는 계산 결과를 문자열로 변환

	LLS_DestroyNode(ResultNode); // 노드 소멸
	LLS_DestroyStack(Stack);	 // 스택 소멸

	return Result;  //계산 결과 반환
}
