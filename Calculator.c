#include "Calculator.h"

char NUMBER[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' };

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

unsigned int GetNextToken(char* Expression, char* Token, int* TYPE) {
	unsigned int i = 0;

	for (i = 0; 0 != Expression[i]; i++) {
		Token[i] = Expression[i];

		if (IsNumber(Expression[i]) == 1) {  // 숫자인 경우
			*TYPE = OPERAND;

			if (IsNumber(Expression[i + 1]) != 1)  // 숫자가 끝나는 지점
				break;
		}
		else {  // 연산자나 괄호인 경우
			*TYPE = Expression[i];
			break;
		}
	}

	Token[++i] = '\0';  // 문자열 종료
	return i;
}


int GetPriority(char Operator, int InStack)
{
	int Priority = -1;

	switch (Operator)
	{
	case LEEF_PARENTHESIS :
		if (InStack)
			Priority = 3;
		else
			Priority = 0;
		break;

	case MULTIPLY :
	case DIVIDE :
		Priority = 1;
		break;

	case PLUS :
	case MINUS :
		Priority = 2;
		break;
	}

	return Priority;
}

int IsPrior(char OperatorInStack, char OperatorInToken) {
	return (GetPriority(OperatorInStack, 1) > GetPriority(OperatorInToken, 0));
}

void GetPostfix(char* InfixExpression, char* PostfixExpression) {
	LinkedListStack* Stack;
	char Token[32];
	int Type = -1;
	unsigned int Position = 0;
	unsigned int Length = strlen(InfixExpression);

	LLS_CreateStack(&Stack);

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