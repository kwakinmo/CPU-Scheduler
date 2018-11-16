// on1.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define MAX_que 50
#define CPU_time 50

typedef struct process process_pointer;
typedef struct process {
	process_pointer *next;	// next process
	int pid;	//process id
	int burst_time;	//cpu burst time
	int priority;	//priority
	int arrive; // arrive time
	int wait;	// waiting time
	int turnaround;	// turnaround time
	int work;	// executed time
	int remain;	// remain time
} process;	

process_pointer *head = NULL, *tail = NULL, *ready_head = NULL, *ready_tail = NULL,
				*fcfs_h = NULL, *fcfs_t = NULL, *nosjf_h = NULL, *nosjf_t = NULL,
				*psjf_h = NULL, *psjf_t = NULL, *nopri_h = NULL, *nopri_t = NULL,
				*ppri_h = NULL, *ppri_t = NULL, *rr_h = NULL, *rr_t = NULL;

void Menu();
void Scheduler_Selection(int select);
void Make_Process(); // process create process id, priority, arrive time...
void Add_Queue(process_pointer *temp, process_pointer **h, process_pointer **t);
void Process_Output(process_pointer **temp);
void State_Result_Out(int time, process_pointer **ready_head, process_pointer **node_head, int flag);
void Process_Result_Output(process_pointer **temp);
void Sort(process_pointer **h, process_pointer **t, int algorithm);
void Copynode(process_pointer *h, process_pointer *temp);
void Copyqueue(process_pointer **node, process_pointer **node_head, process_pointer **node_tail);
void Result_add(int falg, process_pointer **ready_head);

void Nonpreempt(int flag);
void Preempt(int flag);
void R_Rpreempt(int flag);
void FCFS();	// First-Come, First-served Scheduling
void Nonpreempt_SJF();	 // Nonpreempt Sortest-Job-First Scheduling
void Preempt_SJF();		// Preempt Sortest-Job-First Scheduling
void Nonpreempt_Priority();	 // Nonpreempt Priority Scheduling
void Preempt_Priority();	 // Preempt Priority Scheduling
void R_R();		// Round-Robin Scheduling
void Evaluation(); // each cpu scheduling algorithm evaluation

int process_num = 0;	// process 개수
int quantum = 0;	// quantum  값

main()
{
	Menu();
}	


void Menu()
{
	int select = 0;

	printf("\n\n	**************************************************************\n");
	printf("		CPU Scheduling Algorithm Evaluation Simulation\n");
	printf("	**************************************************************\n");
	printf("\n		0. Process Creation\n");
	printf("		1. First-Come, First-Served Scheduling\n");
	printf("		2. Nonpreempt Shortest-Job-First Scheduling\n");
	printf("		3. Preempt Shortest-Job-First Scheduling\n");
	printf("		4. Nonpreempt Priority Scheduling\n");
	printf("		5. Preempt Priority Scheduling\n");
	printf("		6. Round-Robin Scheduling\n");
	printf("		7. Evaluation \n");
	printf("		8. EXIT\n");
	printf("\n	**************************************************************\n");
	printf("\n	Select Number.	");	
	scanf("%d", &select);
	Scheduler_Selection(select);
}


void Scheduler_Selection(int select) 
{

	if(select != 0 && select != 8 && head == NULL)
	{
		printf("\n	Not creat process. Must secect number 1.\n");
		Menu();
	}
	// 선택한 scheduling Algorithms 함수를 호출한다.
	switch(select)
	{
		case 0: Make_Process();
			break;
		case 1: FCFS();
			break;
		case 2: Nonpreempt_SJF();
			break;
		case 3: Preempt_SJF();
			break;		
		case 4: Nonpreempt_Priority();
			break;
		case 5: Preempt_Priority();
			break;
		case 6: R_R();
			break;
		case 7 : Evaluation();
			break;
		case 8 : exit(0);
		
		default: printf("		Bad Command\n\n");
	}
}


void Make_Process()
{
	int i;
	long seed;	// 랜덤시 매번 다른 난수 발생

	process_pointer *temp;
	head = NULL, tail = NULL;

	seed = time(NULL);
    srand(seed);
	
	printf("\n	Number of process :	");
	scanf("%d", &process_num);
	
	while(process_num > MAX_que)
	{
		printf("	Smaller number of process...\n");
		scanf("%d", &process_num);
	}

	for(i = 0; i < process_num; i++) // process를 arrive time 순서에 따라 구조체로
	{
		temp = (process_pointer *)malloc(sizeof(process_pointer));
		temp -> pid = i;	//process id
		temp -> burst_time = rand() % CPU_time + 1;	// cpu burst_time 생성
		temp -> priority = rand() % process_num;	// process priority 생성
		temp -> arrive = rand() % CPU_time + 1;	// process arrive time 생성
		temp -> wait = 0;	// wait time 초기화
		temp -> turnaround = 0;	// turnaround time 초기화
		temp -> work = 0;	// work time 초기화
		temp -> remain = temp -> burst_time;	// remain time = burst_time
		temp -> next = NULL;	
		Add_Queue(temp, &head, &tail); //queue에 삽입
	}

	// 생성한 process들의 정보 출력
	temp = head;
	Sleep(100);
	system("cls");
	printf("\n	Produced process ");
	Process_Output(&temp); 
	
	// process들을 arrive time 순서대로 정렬 
	printf("	Ordering by arrive time ");
	Sort(&head, &tail, 1);
	temp = head;
	Process_Output(&temp);	// 출력

	Menu(); // process의 정보를 보고 어떤 scheduling algorithm을 쓸 것인지 결정
}


void Add_Queue(process_pointer *temp, process_pointer **h, process_pointer **t)
{	//queue에 process를 insert
	if(*h != NULL ) (*t)->next = temp; 	//첫 노드라면 head에  연결
	else *h = temp; 	//아니면 tail에 연결
	*t = temp;
}


void Result_add(int falg, process_pointer **node)
{
	process_pointer *temp;
	switch(falg)
	{
		case 1:
				temp = (process_pointer *)malloc(sizeof(process_pointer));
				Copynode(*node, temp);
				Add_Queue(temp, &fcfs_h, &fcfs_t);
				break;
		case 2:
				temp = (process_pointer *)malloc(sizeof(process_pointer));
				Copynode(*node, temp);
				Add_Queue(temp, &nosjf_h, &nosjf_t);
				break;
		case 3:
				temp = (process_pointer *)malloc(sizeof(process_pointer));
				Copynode(*node, temp);
				Add_Queue(temp, &psjf_h, &psjf_t);
				break;
		case 4:	
				temp = (process_pointer *)malloc(sizeof(process_pointer));
				Copynode(*node, temp);
				Add_Queue(temp, &nopri_h, &nopri_t);
				break;
		case 5:
				temp = (process_pointer *)malloc(sizeof(process_pointer));
				Copynode(*node, temp);
				Add_Queue(temp, &ppri_h, &ppri_t);
				break;
		case 6:
				temp = (process_pointer *)malloc(sizeof(process_pointer));
				Copynode(*node, temp);
				Add_Queue(temp, &rr_h, &rr_t);
				break;
		default: printf("Error");
				exit(0);
	}
}


void Process_Output(process_pointer **temp) 
{	// 생성한 process 정보 출력
	printf("\n	_____________________________________________________________ \n");
	printf("	Pid  Arrival  Burst  Priority  Wait  Turnaround  Work  Remain  \n");
	printf("	_____________________________________________________________ \n");

	for(;*temp != NULL; *temp = (*temp) -> next)
		printf("	%2d %6d %8d %7d %8d %8d %7d %6d  \n",
			(*temp) -> pid + 1, (*temp) -> arrive, (*temp) -> burst_time, (*temp) -> priority + 1, 
			(*temp) -> wait, (*temp) -> turnaround, (*temp) -> work, (*temp) -> remain);
	printf("	_____________________________________________________________ \n");
	printf("\n");
}


void State_Result_Out(int time, process_pointer **ready_head, process_pointer **node_head, int flag)
{
	process_pointer *ready_node, *node;
	
	ready_node = *ready_head;
	
	if(flag == 1)
		printf("\n	**********  First-Come, First-Served Scheduling  ************\n");
	else if(flag == 2)
		printf("\n	********  Nonpreempt Shortest-Job-First Scheduling  *********\n");
	else if(flag == 3)
		printf("\n	**********  Preempt Shortest-Job-First Scheduling  **********\n");
	else if(flag == 4)
		printf("\n	*************  Nonpreempt Priority Scheduling  **************\n");
	else if(flag == 5)
		printf("\n	***************  Preempt Priority Scheduling  ***************\n");
	else if(flag == 6)
		printf("\n	*****************  Round-Robin Scheduling  ******************\n");

	printf("\n	[time = %d]\n ", time); // 현재 time
	
	printf("	Current excuting process ");	
	if(ready_node == NULL)
	{
		printf("\n	_____________________________________________________________\n");
		printf("	Pid  Arrival  Burst  Priority  Wait  Turnaround  Work  Remain  \n");
		printf("	_____________________________________________________________\n");
	}
	else 
	{
		printf("\n	_____________________________________________________________\n");
		printf("	Pid  Arrival  Burst  Priority  Wait  Turnaround  Work  Remain  \n");
		printf("	_____________________________________________________________\n");
		printf("	%2d %6d %8d %7d %8d %8d %7d %6d  \n",
				ready_node->pid + 1, ready_node->arrive, ready_node->burst_time, ready_node->priority + 1, 
				ready_node-> wait, ready_node-> turnaround, ready_node->work, ready_node->remain);
		printf("	_____________________________________________________________\n");

		ready_node = ready_node->next;
	}
	printf("\n	Ready Queue ");	// ready queue
	if(ready_node == NULL)
	{	
		printf("\n	_____________________________________________________________\n");
		printf("	Pid  Arrival  Burst  Priority  Wait  Turnaround  Work  Remain  \n");
		printf("	_____________________________________________________________\n");
	}
	else Process_Output(&ready_node); 
		
	node = *node_head;
	printf("\n	Remain process ");
	if(node == NULL)
	{
		printf("\n	_____________________________________________________________\n");
		printf("	Pid  Arrival  Burst  Priority  Wait  Turnaround  Work  Remain  \n");
		printf("	_____________________________________________________________\n");		
	}
	else	Process_Output(&node); 
}


void Process_Result_Output(process_pointer **temp)
{
	printf("\n	____________________________________________________________ \n");
	printf("	Pid  Arrival  Burst  Priority  Wait  Turnaround  Completion  \n");
	printf("	____________________________________________________________ \n");

	for(;*temp != NULL; *temp = (*temp) -> next)
		printf("	%2d %6d %8d %7d %8d %8d %13d  \n",
			(*temp) -> pid + 1, (*temp) -> arrive, (*temp) -> burst_time, (*temp) -> priority + 1, 
			(*temp) -> wait, (*temp) -> turnaround, (*temp) -> arrive + (*temp) -> turnaround);
	printf("	____________________________________________________________ \n");
	printf("\n");
}


void Copynode(process_pointer *h, process_pointer *temp)
{	// node를 temp에 복사
	temp -> pid = h -> pid;	//process id
	temp -> burst_time = h -> burst_time;	// cpu burst_time
	temp -> priority = h -> priority; // process priority 
	temp -> arrive = h -> arrive;	// process arrive time
	temp -> wait = h -> wait;
	temp -> turnaround = h -> turnaround;
	temp -> work = h -> work;
	temp -> remain = h -> remain;
	temp -> next = NULL;	
}


void Copyqueue(process_pointer **node, process_pointer **node_head, process_pointer **node_tail)
{
	process_pointer *temp;

	for(; *node; *node = (*node)->next) // list copy
	{
		temp = (process_pointer *)malloc(sizeof(process_pointer));
		Copynode(*node, temp);
		Add_Queue(temp, node_head, node_tail);
	}
}


void Sort(process_pointer **h, process_pointer **t, int algorithm)
{	// process들을 정렬
	int flag = 1;
	process_pointer *start, *mid, *end, *h1, *h2, *t1, *temp;
	// bubble sort로 정렬하기 위해 맨앞과 맨뒤의 노드를 생성
	h1 = (process_pointer *)malloc(sizeof(process_pointer));
	t1 = (process_pointer *)malloc(sizeof(process_pointer));
	(*t)->next = (process_pointer *)malloc(sizeof(process_pointer));

	h1 -> next = *h;	// 맨앞과 맨뒤의 노드를  process들과 연결
	t1 = (*t)->next;
	t1 -> next = NULL;
	
	while(1 == flag) // bubble sort
	{
		flag = 0;
		start = h1;
		mid = start -> next;
		end = mid -> next;

		while(end != t1)
		{
			if(algorithm ==1) // arrive time에 따라 sort
			{
				if(mid -> arrive > end -> arrive)
				{
					start -> next = end;
					mid -> next = end->next;
					end->next = mid;
					flag = 1;
				}
			}
			else if(algorithm == 2)	// burst_time에 따라 sort
			{
				if(mid -> burst_time > end -> burst_time)
				{
					start -> next = end;
					mid -> next = end->next;
					end->next = mid;
					flag = 1;
				}

			}
			else if(algorithm == 3)	// remain에 따라 sort
			{
				if(mid -> remain > end -> remain)
				{
					start -> next = end;
					mid -> next = end->next;
					end->next = mid;
					flag = 1;
				}
			}
			else if(algorithm == 4 || algorithm == 5)
			{	// priority에 따라 sort
				if(mid -> priority > end -> priority)
				{
					start -> next = end;
					mid -> next = end->next;
					end->next = mid;
					flag = 1;
				}
			}

			start = start->next;
			mid = start->next;
			end = mid->next;
		}
	}

	*h = h1->next;	//	head를 설정
	
	flag = 1;
	h2 = h1;

	while(1 == flag) {	//	tail을 설정
		if(h2->next == NULL)
		{
			flag = 0;
			*t = temp; 
			(*t)->next = NULL;
		}
		else
		{
			temp = h2;
			h2 = h2->next;
		}
	}
	// 필요 없으므로 free()
	free(h1);
	free(t1);
}


void FCFS()
{	// FCFS의 flag는 1
	fcfs_h = NULL, fcfs_t = NULL;
	Nonpreempt(1);
	Menu();
}


void Nonpreempt_SJF()
{	// Nonpreempt_SJF의 flag는 2
	nosjf_h = NULL, nosjf_t = NULL;
	Nonpreempt(2);
	Menu();
}


void Preempt_SJF()
{	// Preempt_SJF의 flag는 3
	psjf_h = NULL, psjf_t = NULL;
	Preempt(3);
	Menu();
}


void Nonpreempt_Priority()
{	// Nonpreempt_Priority의 flag는 4
	nopri_h = NULL, nopri_t = NULL;
	Nonpreempt(4);
	Menu();
}


void Preempt_Priority()
{	// Preempt_Priority의 flag는 5
	ppri_h = NULL, ppri_t = NULL;
	Preempt(5);
	Menu();
}


void R_R()
{	// Round Robin의 flag는 6
	rr_h = NULL, rr_t = NULL;
	printf("	Insert quantum : ");
	scanf("%d", &quantum);
	R_Rpreempt(6);
	Menu();
}

void Evaluation()
{
	process_pointer *temp;
	float aver_wait = 0, aver_turn = 0;

	printf("	Insert quantum : ");
	scanf("%d", &quantum);
	
	fcfs_h = NULL, fcfs_t = NULL;
	Nonpreempt(1);

	nosjf_h = NULL, nosjf_t = NULL;
	Nonpreempt(2);

	psjf_h = NULL, psjf_t = NULL;
	Preempt(3);

	nopri_h = NULL, nopri_t = NULL;
	Nonpreempt(4);

	ppri_h = NULL, ppri_t = NULL;
	Preempt(5);

	rr_h = NULL, rr_t = NULL;
	R_Rpreempt(6);
	
	Sleep(100);
	system("cls");

	printf("\n\n	******************  Scheduling Evaluation  ***************** ");

	temp = fcfs_h;
	printf("\n\n	First-Come, First-Served Scheduling ");
	Process_Result_Output(&temp);
	temp = fcfs_h;
	aver_wait = 0, aver_turn = 0;
	for(; fcfs_h; fcfs_h = fcfs_h->next)
	{
		aver_wait += fcfs_h->wait;
		aver_turn += fcfs_h->turnaround;
	}
	aver_wait /= process_num;
	aver_turn /= process_num;
	printf("	Average Wate time = %.3f, Qverage Turnaround time = %.3f\n", 
		aver_wait, aver_turn);

	temp = nosjf_h;
	printf("\n\n	Nonpreempt Shortest-Job-First Scheduling ");
	Process_Result_Output(&temp);
	temp = nosjf_h;
	aver_wait = 0, aver_turn = 0;
	for(; nosjf_h; nosjf_h = nosjf_h->next)
	{
		aver_wait += nosjf_h->wait;
		aver_turn += nosjf_h->turnaround;
	}
	aver_wait /= process_num;
	aver_turn /= process_num;
	printf("	Average Wate time = %.3f, Qverage Turnaround time = %.3f\n", 
	aver_wait, aver_turn);
   	
    temp = psjf_h;
	printf("\n\n	Preempt Shortest-Job-First Scheduling ");
	Process_Result_Output(&temp);
    temp = psjf_h;
    aver_wait = 0, aver_turn = 0;
	for(; psjf_h; psjf_h = psjf_h->next)
	{
		aver_wait += psjf_h->wait;
		aver_turn += psjf_h->turnaround;
	}
	aver_wait /= process_num;
	aver_turn /= process_num;
	printf("	Average Wate time = %.3f, Qverage Turnaround time = %.3f\n", 
		aver_wait, aver_turn);
	
	temp = nopri_h;
	printf("\n\n	Nonpreempt Priority Scheduling ");
	Process_Result_Output(&temp);
	temp = nopri_h;
	aver_wait = 0, aver_turn = 0;
	for(; nopri_h; nopri_h = nopri_h->next)
	{
		aver_wait += nopri_h->wait;
		aver_turn += nopri_h->turnaround;
	}
	aver_wait /= process_num;
	aver_turn /= process_num;
	printf("	Average Wait time = %.3f, Qverage Turnaround time = %.3f\n", 
		aver_wait, aver_turn);

	temp = ppri_h;
	printf("\n\n	Preempt Priority Scheduling ");
	Process_Result_Output(&temp);
	temp = ppri_h;
	aver_wait = 0, aver_turn = 0;
	for(; ppri_h; ppri_h = ppri_h->next)
	{
		aver_wait += ppri_h->wait;
		aver_turn += ppri_h->turnaround;
	}
	aver_wait /= process_num;
	aver_turn /= process_num;
	printf("	Average Wait time = %.3f, Qverage Turnaround time = %.3f\n", 
		aver_wait, aver_turn);

	temp = rr_h;
	printf("\n\n	Round-Robin Scheduling ");
	Process_Result_Output(&temp);
	temp = rr_h;
	aver_wait = 0, aver_turn = 0;
	for(; rr_h; rr_h = rr_h->next)
	{
		aver_wait += rr_h->wait;
		aver_turn += rr_h->turnaround;
	}
	aver_wait /= process_num;
	aver_turn /= process_num;
	printf("	Average Wait time = %.3f, Qverage Turnaround time = %.3f\n", 
		aver_wait, aver_turn);
		
	Menu();
}


void Nonpreempt(int flag)
{
	process_pointer *node_head = NULL, *node_tail = NULL, *node, *ready_node, *temp;

	int time =0, cpu = -1, exit_num = 0;

	ready_head = NULL, ready_tail = NULL;
	node = head; 

	Copyqueue(&node, &node_head, &node_tail);
	
	while(exit_num < process_num)
	{
		Sleep(100);
		system("cls");
	
		node = node_head;

		while(node != NULL && node -> arrive == time )
		{	// process list에서 시간이 된 process들을 ready queue에 넣는다.
			temp = (process_pointer *)malloc(sizeof(process_pointer));
			Copynode(node, temp);
			Add_Queue(temp, &ready_head, &ready_tail);
			
			node = node -> next; // ready queue에 들어간 process들은 process list에서 제거
	
			node_head = node_head-> next;			
		}

		if(cpu == -2)
		{	// ready queue에서 첫번째 process는 현재 수행하고 있는 process
			// cpu == 2라면 수행하고 있던 process가 작업을 마친 것
			cpu = -1; // cpu == -1이면 cpu가 놀게 된 상태

			Result_add(flag, &ready_head);

			ready_head = ready_head->next;

			if(ready_head != NULL)
			{
				if(flag == 2) // Nonpreempt SJF-burst_time로 정렬
					Sort(&ready_head, &ready_tail, flag);
				if(flag == 4)	// Nonpreempt Priority-priority로 정렬
					Sort(&ready_head, &ready_tail, flag);
			}			
		}

		ready_node = ready_head;

		while(ready_node != NULL)
		{
			if(cpu == ready_node->pid)
			{
				if(ready_node->remain == 1)
				{	//	끝마치는 process 
					exit_num++; // 종료된 process의 개수
					cpu = -2; // process가 종료
				}
				ready_node->work++;
				ready_node->remain--;
				ready_node->turnaround = ready_node->wait + ready_node->work;
			}
			else if(cpu == -1) // cpu가 놀고 있다면
			{
				cpu = ready_node->pid; 
				if(ready_node->remain == 1)
				{
					exit_num++;
					cpu = -2;
				}
				ready_node->work++;
				ready_node->remain--;
				ready_node->turnaround = ready_node->wait + ready_node->work;
			}
			else
			{	// ready queue에서 wating하고 있는  process들
				ready_node->wait++;
				ready_node->turnaround = ready_node->wait + ready_node->work;
			}
			ready_node = ready_node->next;
		}
		State_Result_Out(time, &ready_head, &node_head, flag);

		time++;
	}
	Result_add(flag, &ready_head);
}


void Preempt(int flag)
{	// Preempt scheduling algorithm
	process_pointer *node_head = NULL, *node_tail = NULL, *node, *ready_node, *temp;

	int time =0, cpu = -1, exit_num = 0; // cpu는 현재 수행하고 있는 pid

	ready_head = NULL, ready_tail = NULL;
	node = head; 

	Copyqueue(&node, &node_head, &node_tail);

	while(exit_num < process_num)
	{
		Sleep(100);
		system("cls");
	
		node = node_head;

		while(node != NULL && node -> arrive == time )
		{
			temp = (process_pointer *)malloc(sizeof(process_pointer));
			Copynode(node, temp);
			Add_Queue(temp, &ready_head, &ready_tail);
			
			node = node -> next;
	
			node_head = node_head-> next;			
		}
		if(cpu == -2)
		{
			Result_add(flag, &ready_head);
			cpu = -1;
			ready_head = ready_head->next;
		}
		if(ready_head != NULL)
		{
			if(flag == 3)	// Preempt_SJF-remain으로 정렬
				Sort(&ready_head, &ready_tail, flag);
			if(flag == 5)	// Preempt_Priority-priority로 정렬
				Sort(&ready_head, &ready_tail, flag);
			cpu = ready_head->pid;	// ready queue에서 가장 앞에 있는 process를 수행
		}			
		ready_node = ready_head;

		while(ready_node != NULL)
		{
			if(cpu == ready_node->pid)
			{
				if(ready_node->remain == 1)
				{
					exit_num++;
					cpu = -2;
				}
				ready_node->work++;
				ready_node->remain--;
				ready_node->turnaround = ready_node->wait + ready_node->work;
			}
			else
			{
				ready_node->wait++;
				ready_node->turnaround = ready_node->wait + ready_node->work;
			}
			ready_node = ready_node->next;
		}		
		State_Result_Out(time, &ready_head, &node_head, flag);

		time++;
	}
	Result_add(flag, &ready_head);
}


void R_Rpreempt(int flag)
{
	process_pointer *node_head = NULL, *node_tail = NULL, *node, *ready_node, *temp;

	int time =0, cpu = -1, timequantum = 0, exit_num = 0;

	ready_head = NULL, ready_tail = NULL;
	node = head; 

	Copyqueue(&node, &node_head, &node_tail);
	
	while(exit_num < process_num)
	{
		Sleep(100);
		system("cls");

		node = node_head;

		while(node != NULL && node -> arrive == time )
		{	// process list에서 시간이 된 process들을 ready queue에 넣는다.
			temp = (process_pointer *)malloc(sizeof(process_pointer));
			Copynode(node, temp);
			Add_Queue(temp, &ready_head, &ready_tail);
			
			node = node -> next; // ready queue에 들어간 process들은 process list에서 제거
	
			node_head = node_head-> next;			
		}

		if(cpu < 0 || timequantum == 0)
		{	// ready queue에서 첫번째 process는 현재 수행하고 있는 process
			// cpu == 2라면 수행하고 있던 process가 작업을 마친 것
			// timequantum == 0이면 할당받은 시간을 다 쓴 것
			timequantum = quantum;
			if(cpu >= 0)
			{
				temp = (process_pointer *)malloc(sizeof(process_pointer));
				Copynode(ready_head, temp);
				Add_Queue(temp, &ready_head, &ready_tail);
				ready_head = ready_head->next;

			}
			else if(cpu == -2)
			{
				Result_add(flag, &ready_head);
				cpu = -1;
				ready_head = ready_head->next;

			}
			if(ready_head != NULL)
				cpu = ready_head->pid;
		}
		ready_node = ready_head;

		while(ready_node != NULL)
		{
			if(cpu == ready_node->pid)
			{
				if(ready_node->remain == 1)
				{	//	끝마치는 process 
					exit_num++; // 종료된 process의 개수
					cpu = -2; // process가 종료
				}
				ready_node->work++;
				ready_node->remain--;
				ready_node->turnaround = ready_node->wait + ready_node->work;
				timequantum--;
			}
			else
			{	// ready queue에서 wating하고 있는  process들
				ready_node->wait++;
				ready_node->turnaround = ready_node->wait + ready_node->work;
			}
			ready_node = ready_node->next;
		}

		State_Result_Out(time, &ready_head, &node_head, flag);

		time++;
	}
	Result_add(flag, &ready_head);
}
