#pragma warning(disable:4996)

#define STUDENT_NUMBER 3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

// 1. 구조체로 정리
// 2. 엑셀 파일 전달
// 3. 추가 삭제 기능

typedef struct _student {
	int studentID;	//20161110061	11개
	char name[20];
	double score;
}Student, *pStudent;

char* GetNextString(char* data, char separator, char* target)
{
	while (*data && (*data != separator))	// NULL 또는 구분자까지
	{
		*target++ = *data++;	// 문자를 대상에 한개씩 넣기 char
		
	}

	// 문자열 끝을 NULL로 바꿔줌
	if (*(target - 1) == '\n') *(target - 1) = 0;
	else *target = 0;

	if (*data == separator) data++; // 문자가 구분자일때 다음 칸으로
	
	return data;	// 끝부분
}	//strtok 원리랑 같음


void selectBubbleSort(pStudent p, int target)
{
	bool flag = true;

	for (int i = 0; i < STUDENT_NUMBER; i++)	// 전체 개수
	{
		flag = false;
		for (int j = 0; j < STUDENT_NUMBER - i - 1; j++)	// 비교 횟수
		{
			switch (target)
			{
			case 1:
				if (p[j].studentID > p[j + 1].studentID)	// 오름차순
				{
					Student tmp;
					tmp = p[j];
					p[j] = p[j + 1];
					p[j + 1] = tmp;
					flag = true;
				}
				break;

			case 2:
				if (strcmp(p[j].name, p[j + 1].name) > 0)	// ㄱㄴㄷ 뒤로 갈수록 큼(1나왔음->뒤에 것이 큼)-> 오름차순
				{
					Student tmp;
					tmp = p[j];
					p[j] = p[j + 1];
					p[j + 1] = tmp;
					flag = true;
				}
				break;

			case 3:
				if (p[j].score < p[j + 1].score)	// 내림차순
				{
					Student tmp;
					tmp = p[j];
					p[j] = p[j + 1];
					p[j + 1] = tmp;
					flag = true;
				}
				break;

			default:
				break;
			}
		}
		if (flag == false) break;
	}
}


// 요구사항 : 점수(double), 학번(int), 이름(char*)에 따라 정렬
void sortData(pStudent p)
{	
	//char* target = (char*)malloc(sizeof(char*) *10);
	//scanf_s("%s", target, sizeof(target));
	//printf("정렬 대상 : %s\n", target);
	//free(target);

	int target = 0;
	bool flag = true;

	printf("정렬할 기준의 숫자를 선택하세요.\n==================\n");
	printf("학번 -> 1\n이름 -> 2\n점수 -> 3\n");
	printf("==================\n");
	printf("입력 : ");
	scanf_s("%d", &target);
	printf("정렬 기준 -> ");
	
	switch (target)
	{
	case 1:
		printf("학번\n");
		printf("------------------\n");
		selectBubbleSort(p, target);
		break;
	case 2:
		printf("이름\n");
		printf("------------------\n");
		selectBubbleSort(p, target);
		break;
	case 3:
		printf("점수\n");
		printf("------------------\n");
		selectBubbleSort(p, target);
		break;
	default:
		printf("잘못입력하셨습니다.\n");
		break;
	}
	system("cls");
}


void swapStruct(pStudent p1, pStudent p2)
{
	Student tmp;
	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

void printStruct(pStudent p)
{
	for (int i = 0; i < STUDENT_NUMBER; i++)
	{
		printf("%2d=%3s=%.1f\n", p[i].studentID, p[i].name, p[i].score);
	}
}

void saveText(FILE* filename, pStudent p)
{
	for (int i = 0; i < STUDENT_NUMBER; i++)
	{
		fprintf(filename, "%2d-%3s-%.1f\n", p[i].studentID, p[i].name, p[i].score);
	}
}

int main()
{	

	Student polyStudent[STUDENT_NUMBER];
	//pStudent poly = polyStudent;
	char* strData[3];
	int count = 0;

	//printf("%p %p\n", polyStudent, poly);

	FILE* p_file = NULL;
	FILE* save_file = NULL;

	if (0 == fopen_s(&p_file, "StudentData.csv", "rt")) {
	
		char one_line_string[128], str[32], * p_pos;
		const char* p_title_format_table[3] = {" %s", " %s", "  %s\n"};	// 0,1,2 포멧설정
		const char* p_data_format_table[3]  = {"  %s", "  %3s", " %3s\n" };
		int len = strlen(str) + 1;

		for (int i = 0; i < 3; i++)
		{
			strData[i] = (char*)malloc(sizeof(char) * len);
		}


		if (NULL != fgets(one_line_string, 128, p_file)) {
			p_pos = one_line_string;
			//printf(p_pos);
			for (int i = 0; *p_pos; i++)	// NULL 나올 때까지
			{
				p_pos = GetNextString(p_pos, ',', str);
				printf(p_title_format_table[i], str);
			}
			
		}

		printf("=================\n");
		while (NULL != fgets(one_line_string, 128, p_file)) { //전체 3줄임
			p_pos = one_line_string;	// 한줄씩 나타냄(문자열)	, 초기위치

			for (int i = 0; *p_pos; i++)	// 마지막 문자가 NULL까지 돌아감
			{
				p_pos = GetNextString(p_pos, ',', str);	// 위치 주소 반환
				strcpy(strData[i], str);
				//printf(p_data_format_table[i], str);
			}
			// 한줄이 끝나면 저장해놓은 것 대입
			polyStudent[count].studentID = atoi(strData[0]);
			strcpy(polyStudent[count].name, strData[1]);
			polyStudent[count].score = atof(strData[2]);
		
			count++;
			//printf("==========================\n");
			//printf("%s-%s-%s\n", strData[0], strData[1], strData[2] );
			

		}

		for (int i = 0; i < 3; i++)
		{
			free(strData[i]);
		}

		fclose(p_file);
	}
	

	//swapStruct(&polyStudent[0], &polyStudent[1]);
	printStruct(&polyStudent);
	//printf("비교%d\n", strcmp("ㄱ", "ㄴ"));
	printf("=================\n");
	//system("cls");
	sortData(&polyStudent);
	printStruct(&polyStudent);
	printf("=================\n");
	printf("데이터를 저장하시겠습니까? (Y/N) : ");
	char comfirmButton;
	getchar();	// 버퍼 제거
	scanf_s("%c", &comfirmButton, sizeof(comfirmButton));
	if ((comfirmButton == 'Y') || (comfirmButton == 'y')) {
		save_file = fopen("log.txt", "w");
		saveText(save_file, &polyStudent);
		printf("[저장]\n");
		fclose(save_file);
	}
	else if ((comfirmButton == 'N') || (comfirmButton == 'n')) {
		printf("[취소]\n");
	}

	return 0;
}