#include <stdio.h>
#include <string.h>
#include "Calculator.h"

#define EXPRESSIONLEN 100	//��Ģ����� ����

int main()
{
	char InfixExpression[EXPRESSIONLEN];	//����ǥ���
	char PostfixExpression[EXPRESSIONLEN];  //����ǥ���

	double Result = 0.0;

	memset(InfixExpression, 0, sizeof(InfixExpression));
	memset(PostfixExpression, 0, sizeof(PostfixExpression));

	printf("����� ��Ģ������� �Է��Ͻÿ� : ");
	scanf("%s", InfixExpression);

	GetPostfix(InfixExpression, PostfixExpression);

	printf("����ǥ��� : %s\n����ǥ��� : %s\n", InfixExpression, PostfixExpression);

	//scanf("%s", PostfixExpression);
	Result = Calculate(PostfixExpression);

	printf("���� ��� : %f\n", Result);

	return 0;
}