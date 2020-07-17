
#ifndef TICKET_UI_H_
#define TICKET_UI_H_
#include "../Common/list.h"
#include "../Service/Ticket.h"
#include "../Service/Seat.h"
#include "../Service/Schedule.h"

static const int TICKET_PAGE_SIZE=8;


void Ticket_UI_Query(void);

int Ticket_UI_ShowTicket (int ticket_id);



#endif 
