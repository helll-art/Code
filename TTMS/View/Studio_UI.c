#include "Studio_UI.h"

#include "./Main_Menu.h"
#include "../Common/list.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"
#include "../Service/EntityKey.h"
#include "Seat_UI.h"
#include "conio.h"

extern account_t gl_CurUser;

static const int STUDIO_PAGE_SIZE = 5;

#include <stdio.h>

void Studio_UI_MgtEntry(account_type_t type) {
	int i, id;
	char choice;
	
	studio_list_t head;
	studio_node_t *pos;
	Pagination_t paging;
	List_Init(head, studio_node_t);
	paging.offset = 0;
	paging.pageSize = STUDIO_PAGE_SIZE;
	//��������
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	if (type==9) {
		do {
			system("cls");
			printf("\n\n\t\t\t                        �ݳ����嵥\n");
			printf("\t\t\t******************************************************************\n\n");
			printf("\t\t\t%-5s  %-14s  %-12s  %-12s  %-12s\n", "ID", "    ����", "����",
				"����", "��λ��");
			
			//��ʾ����
			for (i = 0, pos = (studio_node_t *)(paging.curPos);
				pos != head && i < paging.pageSize; i++) {
				printf("\t\t\t%-5d  %-14s    %-12d  %-12d  %-12d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
				pos = pos->next;
			}
			printf("\n\t\t\t  ����:%2d \t\t\t\t\t%d/%d \n",
				paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
			printf("\t\t\t******************************************************************\n\n");
			printf("\t\t\t[P]��һҳ\t\t\t\t[N]��һҳ\n\t\t\t[A]�����ݳ�����Ϣ\t\t\t[D]ɾ���ݳ�����Ϣ\n\t\t\t[M]�޸��ݳ�����Ϣ\t\t\t[Z]�����ݳ�����λ\n\t\t\t[R]����");
			
			printf("\n\n\t\t\t����������ѡ��:");
			choice = getche();
			switch (choice)
			{ 
			case 'A':
			case 'a':
				if (Studio_UI_Add()) //�����ӳɹ����������һҳ��ʾ
				{
					paging.totalRecords = Studio_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, studio_node_t);
				}
				break; 
			case 'D':
			case 'd':
				printf("\n");
				printf("\t\t\t������Ҫɾ�����ݳ���ID:");
				scanf("%d", &id);
				getchar();
				if (Studio_UI_Delete(id)) {	//������������
					paging.totalRecords = Studio_Srv_FetchAll(head);
					List_Paging(head, paging, studio_node_t);
				}
				break; 
			case 'M':
			case 'm':
				printf("\n");
				printf("\t\t\t������Ҫ�޸ĵ��ݳ���ID:");
				scanf("%d", &id);
				if (Studio_UI_Modify(id)) {	//������������
					paging.totalRecords = Studio_Srv_FetchAll(head);
					List_Paging(head, paging, studio_node_t);
				}
				break; 
			case 'Z':
			case 'z':
				printf("\n");
				printf("\t\t\t������Ҫ������λ���ݳ���ID:");
				scanf("%d", &id);
				getchar();
				Seat_UI_MgtEntry(id);
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
				break; 
			case 'P':
			case 'p':
				if (1 < Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
				}
				break; 
			case 'N':
			case 'n':
				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
				}
				break;
			}
		} while (choice != 'R'&&choice != 'r');
	}
	else
	{
		do {
			system("cls");
			printf("\n\n\t\t\t                        �ݳ����б�\n");
			printf("\t\t\t******************************************************************\n\n");
			printf("\t\t\t%-5s  %-14s  %-12s  %-12s  %-12s\n\n", "ID", "    ����", "����",
				"����", "��λ��");
			//��ʾ����
			for (i = 0, pos = (studio_node_t *)(paging.curPos);
				pos != head && i < paging.pageSize; i++) {
				printf("\t\t\t%-5d  %-14s    %-12d  %-12d  %-12d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
				pos = pos->next;
			}
			printf("\n\t\t\t����:%2d\t\t\t\t\t\t    %d/%d \n",
				paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));

			printf("\n\n\t\t\t\t\t  1.��ҳ  2.��ҳ  3.����");

			printf("\n\n\t\t\t����������ѡ��:");
			choice = getche();
			switch (choice)
			{
			case '1':

				if (1 < Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
				}
				break;
			case '2':

				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
				}
				break;
			}
		} while (choice != '3');
	}
	//�ͷ������ռ�
	List_Destroy(head, studio_node_t);
}

int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;
	do {
		/*system("cls");*/
			printf("\n\n\t\t\t                        �����ݳ���\n");
			printf("\t\t\t******************************************************************\n");

		printf("\t\t\t�ݳ�������:");
		//getchar();
		gets(rec.name);
		printf("\t\t\t����:");
		scanf("\t\t\t%d", &rec.rowsCount);
		printf("\t\t\t����:");
		scanf("%d", &rec.colsCount);
		getchar();
		rec.seatsCount = rec.colsCount*rec.rowsCount;
		printf("\t\t\t******************************************************************\n");

		//��ȡ����
		rec.id = EntKey_Srv_CompNewKey("Studio");  //�����Ļ�ȡ�����⣬�Ѿ����

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\n\n\t\t\t\t                        ���ӳɹ�\n");
		}
		else
			printf("\n\n\t\t\t\t                        ����ʧ��\n");
		printf("\n\n\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t1.��������\t2.����");
		printf("\n\n\t\t\t����������ѡ��:");
		choice = getche();
	} while ('1' == choice);
	return newRecCount;
}

int Studio_UI_Modify(int id) {
	studio_t rec;
	int rtn = 0;
	int newrow, newcolumn;
	seat_list_t list;
	int seatcount;

	/*Load record*/
	if (!Studio_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t�ݳ���������,�����������");
		getch();
		return 0;
	}

			printf("\n\n\t\t\t                        �޸��ݳ���\n");
			printf("\t\t\t******************************************************************\n");

	printf("\t\t\t���޸ĵ��ݳ���ID:%d\n", rec.id);
	printf("\t\t\t�µ��ݳ�������:", rec.name);
	getchar(); 
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if (seatcount) {
		do {			//�����λ�ļ���������λ��Ϣ������µ����б������ǰ�󣬷�����������
			printf("\t\t\t����(����� %d):", rec.rowsCount);
			scanf("\t\t\t%d", &(newrow));
			printf("\t\t\t����(����� %d):", rec.colsCount);
			scanf("%d", &(newcolumn));
		} while (newrow<rec.rowsCount || newcolumn<rec.colsCount);
		getchar();
		rec.rowsCount = newrow;
		rec.colsCount = newcolumn;
		rec.seatsCount = newrow*newcolumn;
	}
	else {
		printf("\t\t\t����:");
		scanf("%d", &rec.rowsCount);
		printf("\t\t\t����:");
		scanf("%d", &rec.colsCount);
		rec.seatsCount = rec.rowsCount*rec.colsCount;
	}

	printf("\t\t\t------------------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf("\n\t\t\t�޸ĳɹ�,��������˳�");
	}
	else
		printf("\n\t\t\t�޸�ʧ��,��������˳�");
	getch();
	return rtn;
}

int Studio_UI_Delete(int id) {

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//��ɾ����ӳ��ʱ��ͬʱ���ݷ�ӳ��idɾ����λ�ļ��е���λ
		if (Seat_Srv_DeleteAllByRoomID(id))
			printf("\n\t\t\tɾ���ɹ�,��������˳�\n");
		rtn = 1;
	}
	else {
		printf("\n\t\t\t�ݳ���������,��������˳�\n");
	}
	printf("\t\t\t");
	getch();
	return rtn;
}