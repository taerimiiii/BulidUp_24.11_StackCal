#include <stdio.h>
#include <string.h>
#include "Calculator.h"

#define EXPRESSIONLEN 100	//사칙연산식 길이

int main()
{
	char InfixExpression[EXPRESSIONLEN];	//중위표기식
	char PostfixExpression[EXPRESSIONLEN];  //후위표기식

	double Result = 0.0;

	memset(InfixExpression, 0, sizeof(InfixExpression));
	memset(PostfixExpression, 0, sizeof(PostfixExpression));

	printf("계산할 사칙연산식을 입력하시오 : ");
	scanf("%s", InfixExpression);

	GetPostfix(InfixExpression, PostfixExpression);

	printf("중위표기식 : %s\n후위표기식 : %s\n", InfixExpression, PostfixExpression);

	//scanf("%s", PostfixExpression);
	Result = Calculate(PostfixExpression);

	printf("계산기 결과 : %f\n", Result);

	return 0;
}