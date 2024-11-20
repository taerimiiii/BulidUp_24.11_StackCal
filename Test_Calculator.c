#include <stdio.h>
#include <string.h>
#include "Calculator.h"

#define EXPRESSIONLEN 100	//사칙연산식 길이

int main()
{
	char InfixExpression[EXPRESSIONLEN];	//중위표기식
	char PostfixExpression[EXPRESSIONLEN];  //후위표기식

	double Result = 0.0;

	// 메모리세팅
	memset(InfixExpression, 0, sizeof(InfixExpression));	//전부 0으로 세팅
	memset(PostfixExpression, 0, sizeof(PostfixExpression));

	// 입력
	printf("계산할 사칙연산식을 입력하시오 : ");
	scanf("%s", InfixExpression);

	// 중위표기식 후위표기식으로 바꾸기
	GetPostfix(InfixExpression, PostfixExpression);

	// 확인 및 쉬운 이해를 위한 출력
	printf("중위표기식 : %s\n후위표기식 : %s\n", InfixExpression, PostfixExpression);

	// 후위표기식 계산. 스택 활용.
	Result = Calculate(PostfixExpression);

	// 출력
	printf("계산기 결과 : %f\n", Result);

	return 0;
}
