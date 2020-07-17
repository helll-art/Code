#include <stdio.h>
#include <stdlib.h>

#include "Queries_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"
#include "conio.h"

void Queries_Menu(void){
	char choice;
	do {
		system("cls");
		printf("\n\n\t\t\t                        ��Ϣ��ѯ\n");
		printf("\t\t\t******************************************************\n");
		printf("\t\t\t\t  1.�ݳ�����ѯ\n\n");
		printf("\t\t\t\t  2.��Ŀ��ѯ\n\n");
		printf("\t\t\t\t  3.�ݳ��ƻ���ѯ\n\n");
		printf("\t\t\t\t  4.�˳�\n\n");
		printf("\n\t\t\t******************************************************\n");
		printf("\n\t\t\t���������ѡ��(1-4):");
		fflush(stdin);
		choice = getche();
		switch (choice) {
		case '1':
			Studio_UI_MgtEntry(2);
			break;
		case '2':
			Play_UI_MgtEntry(1);
			break;
		case '3':
			Schedule_UI_ListAll();
			break;
		}
	} while ('4' != choice);
	}
