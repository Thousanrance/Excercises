//learn from ��ʯ��ʦ
//haven't finished

#include <stdio.h>
#include <graphics.h>
#include <conio.h>

#define IMG_SIZE 67 //element�ߴ�
#define MAX_ELEMENTS_LIN 6 //һ��element��Ŀ
#define MAX_ELEMENTS_ROW 4 //һ��element��Ŀ
#define MAP_SIZE 8 //ÿ��element��Ŀ
#define WIN_SIZE ((MAP_SIZE+2)*IMG_SIZE) //���ڳߴ�

//����ͼƬ�ز�
IMAGE img_total[3];
IMAGE img_elements[MAX_ELEMENTS_LIN* MAX_ELEMENTS_ROW][1];

//��ͼ
int map[MAP_SIZE+2][MAP_SIZE+2];
int cur_ele_num;

//�洢���������
POINT first = { -1,-1 },second = { -1,-1 };
enum STATE
{
	FIRST,SECOND,
};
int hitflag = FIRST;

//��Ϸ��ʼ��
void GameInit()
{
	//����һ��ͼ�δ���
	initgraph(WIN_SIZE, WIN_SIZE/*, SHOWCONSOLE */ );
	
	//����ͼƬ
	loadimage(&img_total[0],"./res/elements.jpeg");
	loadimage(&img_total[1], "./res/bk.jpg", WIN_SIZE, WIN_SIZE);
	loadimage(&img_total[2], "./res/you win.jpeg", WIN_SIZE, WIN_SIZE);

	
	//�ָ�elements
	SetWorkingImage(&img_total[0]);
	int n = 0;
	for (int i = 0; i < MAX_ELEMENTS_LIN; i++)
	{
		for (int j = 0; j < MAX_ELEMENTS_ROW; j++)
		{
			getimage(&img_elements[n++][0], j * IMG_SIZE, i * IMG_SIZE, IMG_SIZE, IMG_SIZE);
		}
	}
	SetWorkingImage();

	//��ʼ����ͼ
	for (int i = 1; i < MAP_SIZE+1; i++)
	{
		for (int j = 1; j < MAP_SIZE+1; j++)
		{
			map[i][j] = i;
		}
	}
	//���ҵ�ͼ����
	srand(GetTickCount());
	for (int i = 1; i < MAP_SIZE + 1; i++)
	{
		for (int j = 1; j < MAP_SIZE + 1; j++)
		{
			int temp = map[i][j];
			int row = rand() % 8 + 1;
			int col = rand() % 8 + 1;
			map[i][j] = map[row][col];
			map[row][col] = temp;
		}
	}

	cur_ele_num = MAP_SIZE * MAP_SIZE;
}

//��Ϸ����
void GameDraw()
{
	putimage(0, 0, &img_total[1]);
	for (int i = 0; i < MAP_SIZE+2; i++)
	{
		for (int j = 0; j < MAP_SIZE+2; j++)
		{
			if (map[i][j] > 0)
			{
				putimage(j*IMG_SIZE,i*IMG_SIZE,&img_elements[map[i][j]][0]);
			}
		}
	}
}

//����������
void GameMouse()
{
	//ExMessage msg;
	//if (peekmessage(&msg, EM_MOUSE,false))
	//{
	ExMessage Msg = getmessage(EM_MOUSE);
	if (Msg.message == WM_LBUTTONDOWN)
	{
		if (hitflag == FIRST)
		{
			first.x = Msg.y / IMG_SIZE;
			first.y = Msg.x / IMG_SIZE;
			hitflag = SECOND;
		}
		else if (hitflag == SECOND)
		{
			second.x = Msg.y / IMG_SIZE;
			second.y = Msg.x / IMG_SIZE;
			hitflag = FIRST;
		}

	}
	//printf("first(%d,%d),second(%d,%d)\n", first.x, first.y, second.x, second.y);
	//}
}

//�ж�ĳһ���Ƿ���Ԫ��
int isBlocked(int x, int y)
{
	return map[x][y];
}

//ˮƽ�����Ƿ��ܹ�����
bool horizon(POINT first, POINT second)
{
	//���ε��ͬһ��Ԫ��
	if (first.x == second.x && first.y == second.y)
	{
		return false;
	}
	//����Ƿ���ͬһ��
	if (first.x != second.x)
	{
		return false;
	}
	//������
	int m_min = min(first.y, second.y);
	int m_max = max(first.y, second.y);
	for (int i = m_min + 1; i < m_max; i++)
	{
		if (isBlocked(first.x, i))
		{
			return false;
		}
	}
	return true;
}

//��ֱ�����Ƿ��ܹ�����
bool vertical(POINT first, POINT second)
{
	//���ε��ͬһ��Ԫ��
	if (first.x == second.x && first.y == second.y)
	{
		return false;
	}
	//����Ƿ���ͬһ��
	if (first.y != second.y)
	{
		return false;
	}
	//���ϵ���
	int m_min = min(first.x, second.x);
	int m_max = max(first.x, second.x);
	for (int i = m_min + 1; i < m_max; i++)
	{
		if (isBlocked(i, first.y))
		{
			return false;
		}
	}
	return true;
}

//����һ���յ������
bool turn_once(POINT first, POINT second)
{
	//���ε��ͬһ��Ԫ��
	if (first.x == second.x && first.y == second.y)
	{
		return false;
	}

	//���������յ�
	POINT temp1 = { first.x,second.y }, temp2 = { second.x,first.y };
	if (!isBlocked(temp1.x, temp1.y))
	{
		if (horizon(first, temp1) && vertical(temp1, second))
		{
			return true;
		}

	}
	else if (!isBlocked(temp2.x, temp2.y))
	{
		if (vertical(first, temp2) && horizon(temp2, second))
		{
			return true;
		}
	}
	return false;
}

//���������յ������
bool turn_twice(POINT first, POINT second)
{
	//���ε��ͬһ��Ԫ��
	if (first.x == second.x && first.y == second.y)
	{
		return false;
	}

	//����һ�Թյ�
	POINT temp1 = { first.x,0 }, temp2 = { second.x,0 };
	for (int i = 0; i < MAP_SIZE + 2; i++)
	{
		if (i == first.y || i == second.y)
		{
			continue;
		}
		temp1.y = i;
		temp2.y = i;
		if (!isBlocked(temp1.x, temp1.y) && !isBlocked(temp2.x, temp2.y))
		{
			if (horizon(first, temp1) && vertical(temp1, temp2) && horizon(temp2, second))
			{
				return true;
			}
		}
	}

	POINT temp3 = { 0,first.y }, temp4 = { 0,second.y };
	for (int i = 0; i < MAP_SIZE + 2; i++)
	{
		if (i == first.x || i == second.x)
		{
			continue;
		}
		temp3.x = i;
		temp4.x = i;
		if (!isBlocked(temp3.x, temp3.y) && !isBlocked(temp4.x, temp4.y))
		{
			if (vertical(first, temp3) && horizon(temp3, temp4) && vertical(temp4, second))
			{
				return true;
			}
		}
	}
	return false;
}

void show()
{
	for (int i = 0; i < MAP_SIZE + 2; i++)
	{
		for (int j = 0; j < MAP_SIZE + 2; j++)
		{
			printf("%d ", map[i][j]);
		}
		printf("\n");
	}
}

int main()
{
	GameInit();

	//show();

	BeginBatchDraw();

	while (1)
	{
		GameDraw();
		FlushBatchDraw();

		if (cur_ele_num == 0)
		{
			break;
		}

		GameMouse();

		if(map[first.x][first.y] != 0 && map[second.x][second.y] != 0 && map[first.x][first.y] == map[second.x][second.y])
		{
			if (horizon(first, second)||vertical(first, second)||turn_once(first, second)||turn_twice(first,second))
			{
				map[first.x][first.y] = 0;
				map[second.x][second.y] = 0;
				first = { -1,-1 }, second = { -1,-1 };
				cur_ele_num-=2;
				//printf("cur_ele_num = %d\n", cur_ele_num);
				//show();
			}
		}
	}

	BeginBatchDraw();
	putimage(0, 0, &img_total[2]);
	FlushBatchDraw();

	_getch();
	closegraph();

	return 0;
}
