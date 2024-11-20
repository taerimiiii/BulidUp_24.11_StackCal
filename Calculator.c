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

	LLS_CreateStack(&Stack); //스텍만들기

	while (Position < Length) {
		Position += GetNextToken(&InfixExpression[Position], Token, &Type);

		if (Type == OPERAND) {
			strcat(PostfixExpression, Token);
			strcat(PostfixExpression, " ");  // 공백 추가
		}
		else if (Type == LEEF_PARENTHESIS) {
			// 여는 괄호를 스택에 푸쉬
			LLS_Push(Stack, LLS_CreateNode(Token));
		}
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
					strcat(PostfixExpression, " ");  // 공백 추가
					LLS_DestroyNode(Popped);
				}
			}
		}
		else {  // 연산자 처리
			while (!LLS_IsEmpty(Stack) &&
				!IsPrior(LLS_Top(Stack)->Data[0], Token[0])) {
				Node* Popped = LLS_Pop(Stack);

				if (Popped->Data[0] != LEEF_PARENTHESIS) {
					strcat(PostfixExpression, Popped->Data);
					strcat(PostfixExpression, " ");  // 공백 추가
				}

				LLS_DestroyNode(Popped);
			}

			LLS_Push(Stack, LLS_CreateNode(Token));
		}
	}

	// 스택에 남아있는 연산자 추가
	while (!LLS_IsEmpty(Stack)) {
		Node* Popped = LLS_Pop(Stack);

		if (Popped->Data[0] != LEEF_PARENTHESIS) {
			strcat(PostfixExpression, Popped->Data);
			strcat(PostfixExpression, " ");  // 공백 추가
		}

		LLS_DestroyNode(Popped);
	}

	LLS_DestroyStack(Stack);
}


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

		if (Type == SPACE)
			continue;

		if (Type == OPERAND)
		{
			Node* NewNode = LLS_CreateNode(Token);
			LLS_Push(Stack, NewNode);
		}
		else
		{
			char ResultString[32];
			double Operator1, Operator2, TempResult;
			Node* OperatorNode;

			OperatorNode = LLS_Pop(Stack);
			Operator2 = atof(OperatorNode->Data);
			LLS_DestroyNode(OperatorNode);

			OperatorNode = LLS_Pop(Stack);
			Operator1 = atof(OperatorNode->Data);
			LLS_DestroyNode(OperatorNode);

			switch (Type)
			{
			case PLUS: TempResult = Operator1 + Operator2; break;
			case MINUS:	TempResult = Operator1 - Operator2; break;
			case MULTIPLY: TempResult = Operator1 * Operator2; break;
			case DIVIDE: TempResult = Operator1 / Operator2; break;
			}

			_gcvt(TempResult, 10, ResultString);
			LLS_Push(Stack, LLS_CreateNode(ResultString));
		}
	}

	ResultNode = LLS_Pop(Stack);
	Result = atof(ResultNode->Data);
	LLS_DestroyNode(ResultNode);

	LLS_DestroyStack(Stack);

	return Result;
}
