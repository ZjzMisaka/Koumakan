// 控制台仿制东方红魔馆.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include "math.h"
#include "string"
#include <string.h>
#include <stddef.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "windows.h"
#include "windowsx.h"

#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "resource.h"

using namespace std;

#define XMAX 100   //81
#define YMAX 40   //50

int nineoutflag = 0;
int tellgameover = 0;
int turn = 0;
int gameflag = 1;
int hitflag = 0;
int savenumber = 0;
int _sleeptime = 80;                     //刷新频率
char _map[YMAX][XMAX];
int cho;
class randbullet* randbullet_p, * randbullet_p1, * randbullet_head = NULL;
class autobullet* autobullet_p, * autobullet_p1, * autobullet_head = NULL;
class plane* plane_p, * plane_p1, * plane_head = NULL;
class map* map_p, * map_p1, * map_head = NULL, * map1;
class player* player_p;
class save* save_p, * save_p1, * save_head = NULL;
void start();
void createmap();
void printtitle();
void createsave(map* _head);     //中转调用

class flag
{
public:

	int map_flag;
	int randbullet_flag;
	int autobullet_flag;
	int plane_flag;
	int save_flag;
	flag()
	{
		map_flag = 0;
		randbullet_flag = 0;
		autobullet_flag = 0;
		plane_flag = 0;
		save_flag = 0;
	}
}_flag;

class anine
{
public:
	int pointx;
	int pointy;
	int health;
	int fireflag;
	char direction;
	anine()
	{
		fireflag = 1;
		direction = 'l';
	}
	void changepoint(int _pointx, int _pointy) { pointx = _pointx; pointy = _pointy; };
}nine;

class point
{
public:
	int pointx;
	int pointy;
	point()
	{
	}
	void changepoint(int _pointx, int _pointy) { pointx = _pointx; pointy = _pointy; };
};

class direction
{
public:
	double directionx;
	double directiony;
	direction()
	{
	}
	void changedirection(double _directionx, double _directiony) { directionx = _directionx; directiony = _directiony; };
};

class datum//可能是某个点,可能是人物坐标
{
public:
	int datumx;
	int datumy;
	datum()
	{
	}
	void changedatum(int _datumx, int _datumy) { datumx = _datumx; datumy = _datumy; };
};

direction fchangedirection(point _point, direction _direction, datum _datum);

class randbullet
{
public:
	randbullet* randbullet_next;
	point bulletpoint;
	direction bulletdirection;
	randbullet* _lastrand;
	int sign;
	randbullet(int _sign)
	{
		sign = _sign;
	}
	void create(int _pointx, int _pointy, int _directionx, int _directiony, int _sign)
	{
		if (_flag.randbullet_flag == 0)
		{
			randbullet_p->bulletpoint.changepoint(_pointx, _pointy);
			randbullet_p->bulletdirection.changedirection(_directionx, _directiony);
			randbullet_p->sign = _sign;
			randbullet_head = randbullet_p1 = randbullet_p;
			++_flag.randbullet_flag;
		}
		else
		{
			randbullet_p = new randbullet(sign);
			randbullet_p->bulletpoint.changepoint(_pointx, _pointy);
			randbullet_p->bulletdirection.changedirection(_directionx, _directiony);
			randbullet_p->sign = _sign;
			randbullet_p1->randbullet_next = randbullet_p;
			randbullet_p1 = randbullet_p;
		}
		randbullet_p1->randbullet_next = NULL;
	}
	void moveout(randbullet* _randp)
	{
		randbullet* _rand = randbullet_head;
		if (_rand == _randp && _randp->randbullet_next != NULL)
		{
			randbullet_head = randbullet_head->randbullet_next;
			delete _randp;
		}
		else if (_rand == _randp && _randp->randbullet_next == NULL)
		{
			_flag.randbullet_flag = 0;
		}
		else
		{
			while (_rand != _randp)
			{
				_lastrand = _rand;
				_rand = _rand->randbullet_next;
			}
			if (_rand->randbullet_next == NULL)
			{
				delete _rand;
				_lastrand->randbullet_next = NULL;
			}
			else
			{
				_lastrand->randbullet_next = _rand->randbullet_next;
				delete _randp;
			}
		}
	}
};

class autobullet
{
public:
	autobullet* autobullet_next;
	point bulletpoint;
	direction bulletdirection;
	datum bulletdatum;
	autobullet()
	{

	}
	void create(int _pointx, int _pointy, int _directionx, int _directiony, int _datumx, int _datumy)
	{
		if (_flag.autobullet_flag == 0)
		{
			autobullet_p->bulletpoint.changepoint(_pointx, _pointy);
			autobullet_p->bulletdirection.changedirection(_directionx, _directiony);
			autobullet_p->bulletdatum.changedatum(_datumx, _datumy);
			autobullet_head = autobullet_p1 = autobullet_p;
			++_flag.autobullet_flag;
		}
		else
		{
			autobullet_p = new autobullet;
			autobullet_p->bulletpoint.changepoint(_pointx, _pointy);
			autobullet_p->bulletdirection.changedirection(_directionx, _directiony);
			autobullet_p->bulletdatum.changedatum(_datumx, _datumy);
			autobullet_p1->autobullet_next = autobullet_p;
			autobullet_p1 = autobullet_p;
		}
		autobullet_p1->autobullet_next = NULL;
	}
	void moveout(autobullet* _autop)
	{
		autobullet* _auto = autobullet_head;
		autobullet* _lastauto;
		if (_auto == _autop && _autop->autobullet_next != NULL)
		{
			autobullet_head = autobullet_head->autobullet_next;
			delete _autop;
		}
		else if (_auto == _autop && _autop->autobullet_next == NULL)
		{
			_flag.autobullet_flag = 0;
		}
		else
		{
			while (hitflag != 1 && _auto != _autop)
			{
				_lastauto = _auto;
				_auto = _auto->autobullet_next;
			}
			if (hitflag != 1 && _auto->autobullet_next == NULL)
			{
				delete _auto;
				_lastauto->autobullet_next = NULL;
			}
			else if (hitflag != 1)
			{
				_lastauto->autobullet_next = _auto->autobullet_next;
				delete _autop;
			}
		}
	}
};

class plane
{
public:
	int nineflag;
	int health;
	int numberflag;
	int sleepflag;
	int _sleepflag;
	int fireflag;
	plane* plane_next;
	point planepoint;
	direction planedirection;
	datum planedatum;
	void firerand(int _directionx, int _directiony)
	{
		--plane_p->_sleepflag;
		if (plane_p->_sleepflag == 0 && plane_p->numberflag != 0)
		{
			plane_p->_sleepflag = plane_p->sleepflag;
			--plane_p->numberflag;
			randbullet_p->create(plane_p->planepoint.pointx, plane_p->planepoint.pointy, _directionx, _directiony, 0);
		}
	}
	void fireauto(int _directionx, int _directiony, int _datumx, int _datumy)
	{
		--plane_p->_sleepflag;
		if (plane_p->_sleepflag == 0 && plane_p->numberflag != 0)
		{
			plane_p->_sleepflag = plane_p->sleepflag;
			--plane_p->numberflag;
			autobullet_p->create(plane_p->planepoint.pointx, plane_p->planepoint.pointy, _directionx, _directiony, _datumx, _datumy);
		}
	}
	void create(int _pointx, int _pointy, int _directionx, int _directiony, int _datumx, int _datumy, int _health, int sleep, int number)
	{
		if (_flag.plane_flag == 0)
		{
			plane_p->numberflag = number;
			plane_p->_sleepflag = plane_p->sleepflag = sleep;
			plane_p->health = _health;
			plane_p->planepoint.changepoint(_pointx, _pointy);
			plane_p->planedirection.changedirection(_directionx, _directiony);
			plane_p->planedatum.changedatum(_datumx, _datumy);
			plane_head = plane_p1 = plane_p;
			++_flag.plane_flag;
		}
		else
		{
			plane_p = new plane;
			plane_p->numberflag = number;
			plane_p->_sleepflag = plane_p->sleepflag = sleep;
			plane_p->health = _health;
			plane_p->planepoint.changepoint(_pointx, _pointy);
			plane_p->planedirection.changedirection(_directionx, _directiony);
			plane_p->planedatum.changedatum(_datumx, _datumy);
			plane_p1->plane_next = plane_p;
			plane_p1 = plane_p;
		}
		plane_p1->plane_next = NULL;
	}
	void moveout(plane* _planep)
	{
		plane* _plane = plane_head;
		plane* _lastplane;
		if (_plane == _planep && _planep->plane_next != NULL)
		{
			plane_head = plane_head->plane_next;
			delete(_planep);
		}
		else if (_plane == _planep && _planep->plane_next == NULL)
		{
			_plane->planepoint.pointx = XMAX - 1;

			_flag.plane_flag = 0;
		}
		else
		{
			while (hitflag != 1 && _plane != _planep)
			{
				_lastplane = _plane;
				_plane = _plane->plane_next;
			}
			if (hitflag != 1 && _plane->plane_next == NULL)
			{
				delete _plane;
				_lastplane->plane_next = NULL;
			}
			else if (hitflag != 1)
			{
				_lastplane->plane_next = _plane->plane_next;
				delete(_planep);
			}
		}
	}
};

class player
{
public:
	int fireflag = 0;
	int boomyi = 4;
	char input;
	int liveflag = 0;
	int cardsflag = 3;
	int cards = 0;
	int level = 1;
	int life;
	point playerpoint;
	direction playerdirection;
	player()
	{
		playerdirection.directionx = 0;
		playerdirection.directiony = 0;
		playerpoint.pointx = XMAX / 2;
		playerpoint.pointy = YMAX - 3;
	}
	void fire(int _pointx, int _pointy, int _directionx, int _directiony)
	{
		randbullet_p->create(_pointx, _pointy, 0, 1, 1);
	}
	void operate()
	{
		int moveflag = 0;
		if (GetAsyncKeyState(VK_ESCAPE))//ESC
		{
			int result = MessageBox(NULL, TEXT("放弃游戏？"), TEXT("暂停"), MB_YESNO);
			switch (result)
			{
			case IDYES:
			{
				tellgameover = 1;
				break;
			}
			case IDNO: {break; }
			}
		}
		if (GetAsyncKeyState(VK_CONTROL))//control
		{
			if (moveflag == 0)
			{
				if (GetAsyncKeyState(VK_UP))
				{     //用在子弹飞机上的思路
					--playerdirection.directiony;
				}
				if (GetAsyncKeyState(VK_DOWN))
				{
					++playerdirection.directiony;
				}
				if (GetAsyncKeyState(VK_LEFT))
				{
					--playerdirection.directionx;
				}
				if (GetAsyncKeyState(VK_RIGHT))
				{
					++playerdirection.directionx;
				}
				++moveflag;
			}
			else
			{
				--moveflag;
			}
		}
		else if (GetAsyncKeyState(VK_UP))
		{     //用在子弹飞机上的思路
			--playerdirection.directiony;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			++playerdirection.directiony;
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			--playerdirection.directionx;
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			++playerdirection.directionx;
		}
		if (GetAsyncKeyState(0x5A))//Z
		{
			if (player_p->level <= 10)
				randbullet_p->create(player_p->playerpoint.pointx, player_p->playerpoint.pointy - 1, 0, -1, 1);
			else if (player_p->level > 10 && player_p->level <= 20)
			{
				if (fireflag % 2)
				{
					if (player_p->playerpoint.pointx - 1 >= 1 && player_p->playerpoint.pointy - 3 <= YMAX - 1)
						randbullet_p->create(player_p->playerpoint.pointx - 1, player_p->playerpoint.pointy - 3, 0, -1, 1);
					if (player_p->playerpoint.pointx + 1 <= XMAX - 3 && player_p->playerpoint.pointy - 3 <= YMAX - 1)
						randbullet_p->create(player_p->playerpoint.pointx + 1, player_p->playerpoint.pointy - 3, 0, -1, 1);
				}
				else if (!(fireflag % 2))
				{
					if (player_p->playerpoint.pointy - 2 <= YMAX - 1)
						randbullet_p->create(player_p->playerpoint.pointx, player_p->playerpoint.pointy - 2, 0, -1, 1);
				}
				++player_p->fireflag;
			}
			else if (player_p->level > 20 && player_p->level <= 35)
			{
				if (fireflag % 2)
				{
					if (player_p->playerpoint.pointx - 1 >= 1 && player_p->playerpoint.pointy - 3 <= YMAX - 1)
						randbullet_p->create(player_p->playerpoint.pointx - 1, player_p->playerpoint.pointy - 3, 0, -1, 1);
					if (player_p->playerpoint.pointx + 1 <= XMAX - 3 && player_p->playerpoint.pointy - 3 <= YMAX - 1)
						randbullet_p->create(player_p->playerpoint.pointx + 1, player_p->playerpoint.pointy - 3, 0, -1, 1);
				}
				else if (!(fireflag % 2))
				{
					if (player_p->playerpoint.pointx - 1 >= 1)
						randbullet_p->create(player_p->playerpoint.pointx - 1, player_p->playerpoint.pointy, -1, -1, 1);
					if (player_p->playerpoint.pointx + 1 <= XMAX - 3)
						randbullet_p->create(player_p->playerpoint.pointx + 1, player_p->playerpoint.pointy, 1, -1, 1);
					randbullet_p->create(player_p->playerpoint.pointx, player_p->playerpoint.pointy - 1, 0, -1, 1);
				}
				++player_p->fireflag;
			}
			else if (player_p->level > 35 && player_p->level <= 55)
			{
				if (player_p->playerpoint.pointx - 1 >= 1 && player_p->playerpoint.pointy + 1 <= YMAX - 1)
					randbullet_p->create(player_p->playerpoint.pointx - 1, player_p->playerpoint.pointy + 1, 0, -1, 1);
				if (player_p->playerpoint.pointx + 1 <= XMAX - 3 && player_p->playerpoint.pointy + 1 <= YMAX - 1)
					randbullet_p->create(player_p->playerpoint.pointx + 1, player_p->playerpoint.pointy + 1, 0, -1, 1);
				if (!(fireflag % 2))
				{
					if (player_p->playerpoint.pointx - 2 >= 1)
						randbullet_p->create(player_p->playerpoint.pointx - 2, player_p->playerpoint.pointy, -1, -1, 1);
					if (player_p->playerpoint.pointx + 2 <= XMAX - 3)
						randbullet_p->create(player_p->playerpoint.pointx + 2, player_p->playerpoint.pointy, 1, -1, 1);
				}
				++player_p->fireflag;
			}
			else if (player_p->level > 55)
			{
				if (player_p->playerpoint.pointx - 1 >= 1 && player_p->playerpoint.pointy + 1 <= YMAX - 1)
					randbullet_p->create(player_p->playerpoint.pointx - 1, player_p->playerpoint.pointy + 1, 0, -1, 1);
				if (player_p->playerpoint.pointx + 1 <= XMAX - 3 && player_p->playerpoint.pointy + 1 <= YMAX - 1)
					randbullet_p->create(player_p->playerpoint.pointx + 1, player_p->playerpoint.pointy + 1, 0, -1, 1);
				if (fireflag % 2)
				{
					if (player_p->playerpoint.pointx - 2 >= 1)
						randbullet_p->create(player_p->playerpoint.pointx - 2, player_p->playerpoint.pointy, -1, 0, 1);
					if (player_p->playerpoint.pointx + 2 <= XMAX - 3)
						randbullet_p->create(player_p->playerpoint.pointx + 2, player_p->playerpoint.pointy, 1, 0, 1);
				}
				if (!(fireflag % 2))
				{
					if (player_p->playerpoint.pointx - 2 >= 1)
						randbullet_p->create(player_p->playerpoint.pointx - 2, player_p->playerpoint.pointy, 0, -1, 1);
					if (player_p->playerpoint.pointx + 2 <= XMAX - 3)
						randbullet_p->create(player_p->playerpoint.pointx + 2, player_p->playerpoint.pointy, 0, -1, 1);
					if (player_p->playerpoint.pointx - 2 >= 1)
						randbullet_p->create(player_p->playerpoint.pointx - 2, player_p->playerpoint.pointy, -1, -1, 1);
					if (player_p->playerpoint.pointx + 2 <= XMAX - 3)
						randbullet_p->create(player_p->playerpoint.pointx + 2, player_p->playerpoint.pointy, 1, -1, 1);
				}
				++player_p->fireflag;
			}
		}
		if (_kbhit() && liveflag == 0) //x
		{
			input = _getch();
			if ('x' == input || 'X' == input)
			{
				if (cardsflag >= 0)
				{
					--cardsflag;
					cards = 20;
				}
			}
		}
		if (cards >= 1)
		{
			liveflag = 1;
			--cards;
			if (player_p->playerpoint.pointy - 1 >= 1)
			{
				randbullet_p->create(player_p->playerpoint.pointx, player_p->playerpoint.pointy - 1, 0, -1, 1);
				randbullet_p->create(player_p->playerpoint.pointx, player_p->playerpoint.pointy - 1, 1, -1, 1);
				randbullet_p->create(player_p->playerpoint.pointx, player_p->playerpoint.pointy - 1, -1, -1, 1);
				randbullet_p->create(player_p->playerpoint.pointx, player_p->playerpoint.pointy - 1, -1, 0, 1);
				randbullet_p->create(player_p->playerpoint.pointx, player_p->playerpoint.pointy - 1, 1, 0, 1);
			}
			if (player_p->playerpoint.pointx + 2 <= XMAX - 3)
			{
				randbullet_p->create(player_p->playerpoint.pointx + 2, player_p->playerpoint.pointy, 1, -1, 1);
				randbullet_p->create(player_p->playerpoint.pointx + 2, player_p->playerpoint.pointy, 1, 0, 1);
				randbullet_p->create(player_p->playerpoint.pointx + 2, player_p->playerpoint.pointy, 0, -1, 1);
			}
			if (player_p->playerpoint.pointx - 2 >= 1)
			{
				randbullet_p->create(player_p->playerpoint.pointx - 2, player_p->playerpoint.pointy, -1, -1, 1);
				randbullet_p->create(player_p->playerpoint.pointx - 2, player_p->playerpoint.pointy, -1, 0, 1);
				randbullet_p->create(player_p->playerpoint.pointx - 2, player_p->playerpoint.pointy, 0, -1, 1);
			}
			while (player_p->playerpoint.pointy - boomyi >= 2)
			{
				randbullet_p->create(player_p->playerpoint.pointx, player_p->playerpoint.pointy - boomyi, -1, 0, 1);
				randbullet_p->create(player_p->playerpoint.pointx, player_p->playerpoint.pointy - boomyi, 1, 0, 1);
				boomyi += 3;
			}
			boomyi = 4;
		}
		if (cards == 0)
		{
			liveflag = 0;
		}
	}
};

class map
{
private:

public:
	map()
	{

	}
	int hitx;
	int hity;
	map* map_next;
	char maps[YMAX][XMAX];
	void gameover(bool win)
	{
		randbullet* randp = randbullet_head, * _randp = randbullet_head;
		autobullet* autop = autobullet_head, * _autop = autobullet_head;
		plane* planep = plane_head, * _planep = plane_head;
		map* mapp = map_head, * _mapp = map_head;
		HWND b = GetForegroundWindow();
		int result;
		if (win == false)
			result = MessageBox(b, TEXT("保存录像？"), TEXT("你输了"), MB_YESNO | MB_ICONEXCLAMATION);
		else
			result = MessageBox(b, TEXT("保存录像？"), TEXT("你赢了"), MB_YESNO | MB_ICONEXCLAMATION);
		switch (result)
		{
		case IDYES: {map1 = map_head; map_head = NULL; createsave(map1); break; }
		case IDNO: break;
		}
		while (randbullet_head != NULL && randbullet_head->randbullet_next != NULL)
		{
			_randp = randbullet_head;
			randbullet_head = randbullet_head->randbullet_next;
			delete _randp;
			randbullet_p = randbullet_head;
		}
		while (autobullet_head != NULL && autobullet_head->autobullet_next != NULL)
		{
			_autop = autobullet_head;
			autobullet_head = autobullet_head->autobullet_next;
			delete _autop;
			autobullet_p = autobullet_head;
		}
		while (plane_head != NULL && plane_head->plane_next != NULL)
		{
			_planep = plane_head;
			plane_head = plane_head->plane_next;
			delete _planep;
			plane_p = plane_head;
		}
		while (map_head != NULL && map_head->map_next != NULL)
		{
			_mapp = map_head;
			map_head = map_head->map_next;
			delete _mapp;
			map_p = map_head;
			map_p->maps[0][0] = '\0';
		}
		_flag.map_flag = 0;
		_flag.plane_flag = 0;
		_flag.autobullet_flag = 0;
		_flag.randbullet_flag = 0;
		delete player_p;
		gameflag = 0;
		nineoutflag = 0;
		tellgameover = 0;
		turn = 0;
		hitflag = 0;
	}
	void move(point* point, direction* direction)
	{
		if (point->pointx + direction->directionx >= 1 && point->pointx + direction->directionx <= XMAX - 3 && point->pointy + direction->directiony >= 1 && point->pointy + direction->directiony <= YMAX - 2)
		{
			hitx = point->pointx += direction->directionx;
			hity = point->pointy += direction->directiony;
			player_p->playerdirection.directionx = 0;
			player_p->playerdirection.directiony = 0;
		}

		else if (point->pointx + direction->directionx < 1 || point->pointx + direction->directionx > XMAX - 3)
		{
			hitx = point->pointx;
			hity = point->pointy += direction->directiony;
			player_p->playerdirection.directionx = 0;
			player_p->playerdirection.directiony = 0;
		}
		else
		{
			hitx = point->pointx += direction->directionx;
			hitx = point->pointy;
			player_p->playerdirection.directionx = 0;
			player_p->playerdirection.directiony = 0;
		}
	}
	void moveothers()
	{
		randbullet* randp = randbullet_head, * _rand, * _randp = randbullet_head;
		autobullet* autop = autobullet_head, * _auto, * _autop = autobullet_head;
		plane* planep = plane_head, * _plane, * _planep = plane_head;
		if (tellgameover == 1)
		{
			tellgameover = 0;
			while (randbullet_head != NULL && randbullet_head->randbullet_next != NULL)
			{
				_randp = randbullet_head;
				randbullet_head = randbullet_head->randbullet_next;
				delete _randp;
				randbullet_p = randbullet_head;
			}
			while (autobullet_head != NULL && autobullet_head->autobullet_next != NULL)
			{
				_autop = autobullet_head;
				autobullet_head = autobullet_head->autobullet_next;
				delete _autop;
				autobullet_p = autobullet_head;
			}
			while (plane_head != NULL && plane_head->plane_next != NULL)
			{
				_planep = plane_head;
				plane_head = plane_head->plane_next;
				delete _planep;
				plane_p = plane_head;
			}
			--player_p->life;
			player_p->level = 1;
			hitflag = 1;
			hitx = player_p->playerpoint.pointx;
			hity = player_p->playerpoint.pointy;
			gameover(false);
		}
		while (hitflag != 1 && randp != NULL)
		{
			randp->bulletpoint.pointx += randp->bulletdirection.directionx;
			randp->bulletpoint.pointy += randp->bulletdirection.directiony;
			if (randp->bulletpoint.pointx == player_p->playerpoint.pointx &&
				randp->bulletpoint.pointy == player_p->playerpoint.pointy &&
				player_p->liveflag == 0 &&
				randp->sign == 0)
			{
				while (randbullet_head != NULL && randbullet_head->randbullet_next != NULL)
				{
					_randp = randbullet_head;
					randbullet_head = randbullet_head->randbullet_next;
					delete _randp;
					randbullet_p = randbullet_head;
				}
				while (autobullet_head != NULL && autobullet_head->autobullet_next != NULL)
				{
					_autop = autobullet_head;
					autobullet_head = autobullet_head->autobullet_next;
					delete _autop;
					autobullet_p = autobullet_head;
				}
				while (plane_head != NULL && plane_head->plane_next != NULL)
				{
					_planep = plane_head;
					plane_head = plane_head->plane_next;
					delete _planep;
					plane_p = plane_head;
				}
				--player_p->life;
				player_p->cardsflag = 3;
				if (player_p->level >= 15)
				{
					player_p->level -= 15;
				}
				hitflag = 1;
				hitx = player_p->playerpoint.pointx;
				hity = player_p->playerpoint.pointy;
				if (player_p->life == 0)
				{
					gameover(false);
					return;
				}
				break;
			}
			_rand = randp;
			if (randp->bulletpoint.pointy <= 0 || randp->bulletpoint.pointy >= YMAX - 1 || randp->bulletpoint.pointx <= 0 || randp->bulletpoint.pointx >= XMAX - 2)
			{
				_rand = _rand->randbullet_next;
				randbullet_head->moveout(randp);
			}
			else
				_rand = _rand->randbullet_next;
			randp = _rand;
		}
		while (hitflag != 1 && autop != NULL)
		{
			autop->bulletpoint.pointx += autop->bulletdirection.directionx;
			autop->bulletpoint.pointy += autop->bulletdirection.directiony;
			if (autop->bulletpoint.pointx == player_p->playerpoint.pointx &&
				autop->bulletpoint.pointy == player_p->playerpoint.pointy &&
				player_p->liveflag == 0)
			{
				while (randbullet_head != NULL && randbullet_head->randbullet_next != NULL)
				{
					_randp = randbullet_head;
					randbullet_head = randbullet_head->randbullet_next;
					delete _randp;
					randbullet_p = randbullet_head;
				}
				while (autobullet_head != NULL && autobullet_head->autobullet_next != NULL)
				{
					_autop = autobullet_head;
					autobullet_head = autobullet_head->autobullet_next;
					delete _autop;
					autobullet_p = autobullet_head;
				}
				while (plane_head != NULL && plane_head->plane_next != NULL)
				{
					_planep = plane_head;
					plane_head = plane_head->plane_next;
					delete _planep;
					plane_p = plane_head;
				}
				--player_p->life;
				player_p->cardsflag = 3;
				if (player_p->level >= 15)
				{
					player_p->level -= 15;
				}
				hitflag = 1;
				hitx = player_p->playerpoint.pointx;
				hity = player_p->playerpoint.pointy;
				if (player_p->life == 0)
				{
					gameover(false);
					return;
				}
				break;
			}
			_auto = autop;
			if (autop->bulletpoint.pointy <= 0 || autop->bulletpoint.pointy >= YMAX - 1 || autop->bulletpoint.pointx <= 0 || autop->bulletpoint.pointx >= XMAX - 2)
			{
				_auto = _auto->autobullet_next;
				autop->moveout(autop);
			}
			else
				_auto = _auto->autobullet_next;
			autop = _auto;
		}
		while (hitflag != 1 && planep != NULL)
		{
			planep->planepoint.pointx += planep->planedirection.directionx;
			planep->planepoint.pointy += planep->planedirection.directiony;
			if (planep->planepoint.pointx == player_p->playerpoint.pointx &&
				planep->planepoint.pointy == player_p->playerpoint.pointy &&
				player_p->liveflag == 0)
			{
				while (randbullet_head != NULL && randbullet_head->randbullet_next != NULL)
				{
					_randp = randbullet_head;
					randbullet_head = randbullet_head->randbullet_next;
					delete _randp;
					randbullet_p = randbullet_head;
				}
				while (autobullet_head != NULL && autobullet_head->autobullet_next != NULL)
				{
					_autop = autobullet_head;
					autobullet_head = autobullet_head->autobullet_next;
					delete _autop;
					autobullet_p = autobullet_head;
				}
				while (plane_head != NULL && plane_head->plane_next != NULL)
				{
					_planep = plane_head;
					plane_head = plane_head->plane_next;
					delete _planep;
					plane_p = plane_head;
				}
				hitflag = 1;
				--player_p->life;
				player_p->cardsflag = 3;
				if (player_p->level >= 15)
				{
					player_p->level -= 15;
				}
				hitx = player_p->playerpoint.pointx;
				hity = player_p->playerpoint.pointy;
				if (player_p->life == 0)
				{
					gameover(false);
					return;
				}
				break;
			}
			_plane = planep;
			if (hitflag != 1 && map_p->maps[planep->planepoint.pointy][planep->planepoint.pointx] == '^')
			{
				--planep->health;
				_plane = planep->plane_next;
				if (planep->health == 0)
				{
					planep->moveout(planep);
					++player_p->level;
				}
			}
			else if (planep->planepoint.pointy <= 0 || planep->planepoint.pointy >= YMAX - 1 || planep->planepoint.pointx <= 0 || planep->planepoint.pointx >= XMAX - 2)
			{
				_plane = _plane->plane_next;
				planep->moveout(planep);
			}
			else
				_plane = _plane->plane_next;
			planep = _plane;
		}
		if (map_p->maps[nine.pointy][nine.pointx] == '^')
			--nine.health;
	}
	void printscreen()
	{
		int i = 0;
		int j = 0;
		int x;
		int y;
		randbullet* randp = randbullet_head;
		autobullet* autop = autobullet_head;
		plane* planep = plane_head;
		while (randp != NULL)
		{
			x = randp->bulletpoint.pointx;
			y = randp->bulletpoint.pointy;
			if (x >= 1 && x <= XMAX - 2 && y >= 1 && y <= YMAX - 3 && randp->sign == 0)
			{
				map_p->maps[y][x] = '*';
			}
			else if (x >= 1 && x <= XMAX - 2 && y >= 1 && y <= YMAX - 3 && randp->sign == 1 && map_p->maps[y][x] != '*')
			{
				map_p->maps[y][x] = '^';
			}
			randp = randp->randbullet_next;
		}
		while (autop != NULL)
		{
			x = autop->bulletpoint.pointx;
			y = autop->bulletpoint.pointy;
			if (x >= 1 && x <= XMAX - 2 && y >= 1 && y <= YMAX - 3)
			{
				if (autop->bulletdirection.directionx == 1 && autop->bulletdirection.directiony == 1)
					maps[y][x] = '\\';
				else if (autop->bulletdirection.directionx == 1 && autop->bulletdirection.directiony == 0)
					maps[y][x] = '-';
				else if (autop->bulletdirection.directionx == 1 && autop->bulletdirection.directiony == -1)
					maps[y][x] = '/';
				else if (autop->bulletdirection.directionx == 0 && autop->bulletdirection.directiony == -1)
					maps[y][x] = '|';
				else if (autop->bulletdirection.directionx == -1 && autop->bulletdirection.directiony == -1)
					maps[y][x] = '\\';
				else if (autop->bulletdirection.directionx == -1 && autop->bulletdirection.directiony == 0)
					maps[y][x] = '-';
				else if (autop->bulletdirection.directionx == -1 && autop->bulletdirection.directiony == 1)
					maps[y][x] = '/';
				else if (autop->bulletdirection.directionx == 0 && autop->bulletdirection.directiony == 1)
					maps[y][x] = '|';
				else
					maps[y][x] = '*';
			}
			autop = autop->autobullet_next;
		}
		while (planep != NULL)
		{
			x = planep->planepoint.pointx;
			y = planep->planepoint.pointy;
			if (x >= 1 && x <= XMAX - 2 && y >= 1 && y <= YMAX - 3)
			{
				map_p->maps[y][x] = 'O';
			}
			planep = planep->plane_next;
		}
		if (nineoutflag == 1)
		{
			x = nine.pointx;
			y = nine.pointy;
			if (x >= 1 && x <= XMAX - 2 && y >= 1 && y <= YMAX - 3)
			{
				map_p->maps[y][x] = '@';
			}
		}
		maps[player_p->playerpoint.pointy][player_p->playerpoint.pointx] = '8';
		//		待施工
		char printmap[XMAX * YMAX];
		printmap[0] = '\0';
		strcat(printmap, maps[0]);
		system("cls");
		puts(printmap);
		if (hitflag == 1)
		{
			char _maps[YMAX][XMAX];
			memcpy(_maps, map_p->maps, sizeof _map);
			if (_maps[hity][hitx + 1] == ' ')_maps[hity][hitx + 1] = '-';
			if (_maps[hity][hitx - 1] == ' ')_maps[hity][hitx - 1] = '-';
			if (_maps[hity + 1][hitx] == ' ')_maps[hity + 1][hitx] = '|';
			if (_maps[hity - 1][hitx] == ' ')_maps[hity - 1][hitx] = '|';
			if (_maps[hity + 1][hitx + 1] == ' ')_maps[hity + 1][hitx + 1] = '\\';
			if (_maps[hity - 1][hitx + 1] == ' ')_maps[hity - 1][hitx + 1] = '/';
			if (_maps[hity + 1][hitx - 1] == ' ')_maps[hity + 1][hitx - 1] = '/';
			if (_maps[hity - 1][hitx - 1] == ' ')_maps[hity - 1][hitx - 1] = '\\';
			char printmap1[XMAX * YMAX];
			memcpy(printmap1, _maps, sizeof _map);
			Sleep(40);
			system("cls");
			puts(printmap1);
			if (_maps[hity][hitx + 2] == ' ')_maps[hity][hitx + 2] = '-';
			if (_maps[hity][hitx - 2] == ' ')_maps[hity][hitx - 2] = '-';
			if (_maps[hity + 2][hitx] == ' ')_maps[hity + 2][hitx] = '|';
			if (_maps[hity - 2][hitx] == ' ')_maps[hity - 2][hitx] = '|';
			if (_maps[hity + 2][hitx + 2] == ' ')_maps[hity + 2][hitx + 2] = '\\';
			if (_maps[hity - 2][hitx + 2] == ' ')_maps[hity - 2][hitx + 2] = '/';
			if (_maps[hity + 2][hitx - 2] == ' ')_maps[hity + 2][hitx - 2] = '/';
			if (_maps[hity - 2][hitx - 2] == ' ')_maps[hity - 2][hitx - 2] = '\\';
			memcpy(printmap1, _maps, sizeof _map);
			Sleep((40 + _sleeptime) / 2);
			system("cls");
			puts(printmap1);
			Sleep(2000);
			--hitflag;
		}
	}
	void create()
	{
		if (_flag.map_flag == 0)
		{
			memcpy(map_p->maps, _map, sizeof _map);
			map_head = map_p1 = map_p;
			++_flag.map_flag;
		}
		else
		{
			map_p = new map;
			memcpy(map_p->maps, _map, sizeof _map);
			map_p->hitx = map_p1->hitx;
			map_p->hity = map_p1->hity;
			map_p1->map_next = map_p;
			map_p1 = map_p;
		}
		map_p1->map_next = NULL;
	}
};

class save
{
public:
	time_t savetime;
	char name[10];
	char chtime[64];
	map* head;
	save* save_next;
	void review(save* _savehead)
	{
		if (_savehead->head != NULL)
		{
			map* _head = _savehead->head;
			while (_head->map_next != NULL)
			{
				char printmap[YMAX * XMAX];
				printmap[0] = '\0';
				strcat(printmap, _head->maps[0]);
				system("cls");
				puts(printmap);
				Sleep(_sleeptime);
				_head = _head->map_next;
			}
			char printmap[YMAX * XMAX];
			printmap[0] = '\0';
			strcat(printmap, _head->maps[0]);
			system("cls");
			puts(printmap);
		}
		else
			printf("NULL\n");
	}
	void create(map* _head)
	{
		if (_flag.save_flag == 0)
		{
			save_p->head = _head;
			printf("name\n");
			std::cin >> save_p->name;
			save_p->savetime = time(0);
			strftime(save_p->chtime, sizeof(save_p->chtime), "%Y/%m/%d %T", localtime(&save_p->savetime));
			save_head = save_p1 = save_p;
			++_flag.save_flag;
		}
		else
		{
			save_p = new save;
			save_p->head = _head;
			printf("name\n");
			std::cin >> save_p->name;
			save_p->savetime = time(0);
			strftime(save_p->chtime, sizeof(save_p->chtime), "%Y/%m/%d %T", localtime(&save_p->savetime));
			save_p1->save_next = save_p;
			save_p1 = save_p;
		}
		save_p1->save_next = NULL;
	}
	void moveout(save* _savep)
	{
		save* _save = save_head;
		if (_save == _savep && _savep->save_next != NULL)
		{
			save_head = save_head->save_next;
			delete(_savep);
		}
		else if (_save == _savep && _savep->save_next == NULL)
		{
			_flag.save_flag = 0;
		}
		else
		{
			while (_save->save_next != _savep)
			{
				_save = _save->save_next;
			}
			_save->save_next = _savep->save_next;
			delete(_savep);
		}
	}
};

void createsave(map* _head)
{
	save_p->create(_head);
}

direction fchangedirection(point _point, direction _direction, datum _datum)
{
	direction rdirection;
	rdirection.directionx = 1;
	rdirection.directiony = 0;
	if (_datum.datumx == _point.pointx && _datum.datumy == _point.pointy)
	{
		rdirection.directionx = 0;
		rdirection.directiony = 1;
	}
	else
	{
		rdirection.directionx = _datum.datumx - _point.pointx;
		rdirection.directiony = _datum.datumy - _point.pointy;
		if (_direction.directionx == 0 && _direction.directiony == 0)
		{
			rdirection.directionx = fabs(rdirection.directionx) / rdirection.directionx;
			rdirection.directiony = fabs(rdirection.directiony) / rdirection.directiony;
		}
		else if (rdirection.directionx == _direction.directionx && rdirection.directiony == _direction.directiony && _direction.directionx != 0 && _direction.directiony != 0)
		{
			rdirection.directionx = 0;
			rdirection.directiony = 0;
		}
		else if (rdirection.directionx / rdirection.directiony >= 2)
		{
			rdirection.directionx = abs(rdirection.directionx) / rdirection.directionx;
			rdirection.directiony = 0;
		}
		else if (rdirection.directionx / rdirection.directiony < 2 && rdirection.directionx / rdirection.directiony >= 0.5)
		{
			rdirection.directionx = abs(rdirection.directionx) / rdirection.directionx;
			rdirection.directiony = abs(rdirection.directiony) / rdirection.directiony;
		}
		else if (rdirection.directionx / rdirection.directiony < 0.5 && rdirection.directionx / rdirection.directiony > 0)
		{
			rdirection.directionx = 0;
			rdirection.directiony = abs(rdirection.directiony) / rdirection.directiony;
		}
		else if (rdirection.directionx / rdirection.directiony >= -0.5 && rdirection.directionx / rdirection.directiony < 0)
		{
			rdirection.directionx = 0;
			rdirection.directiony = abs(rdirection.directiony) / rdirection.directiony;
		}
		else if (rdirection.directionx / rdirection.directiony >= -2 && rdirection.directionx / rdirection.directiony < 0.5)
		{
			rdirection.directionx = abs(rdirection.directionx) / rdirection.directionx;
			rdirection.directiony = abs(rdirection.directiony) / rdirection.directiony;
		}
		else if (rdirection.directionx / rdirection.directiony < -2)
		{
			rdirection.directionx = abs(rdirection.directionx) / rdirection.directionx;
			rdirection.directiony = 0;
		}
		else
		{
			rdirection.directionx = fabs(rdirection.directionx) / rdirection.directionx;
			rdirection.directiony = fabs(rdirection.directiony) / rdirection.directiony;
		}
	}
	return rdirection;
}

void createmap()
{
	int i = 0;
	int j = 0;
	while (j <= YMAX - 1)
	{
		while (i <= XMAX - 1)
		{
			_map[j][i] = ' ';
			++i;
		}
		i = 0;
		++j;
	}
	i = 0;
	while (i <= XMAX - 1)
	{
		_map[0][i] = '*';
		_map[YMAX - 1][i] = '*';
		++i;
	}
	j = 0;
	while (j <= YMAX - 1)
	{
		_map[j][0] = '*';
		_map[j][XMAX - 2] = '*';
		_map[j][XMAX - 1] = '\n';
		++j;
	}
	_map[YMAX - 1][XMAX - 1] = '\0';
}

void printtitle()
{
	puts("*******************************************************************************");
	puts("*********************************  -welcome!-  ********************************");
	puts("********************************* 八云紫的愤怒 ********************************");
	puts("*******************************************************************************");
	puts("                          *****      rules:     *****");
	puts("                           **** 按 ↑↓←→移动 ****");
	puts("                            ***  按control加速  ***");
	puts("    ********************     **     按z射击     **");
	puts("    *    按esc可暂停   *      *   按x使用符卡   *");
	puts("    *  按esc可放弃本关 *       *****************");
	puts("    *   bomb默认为4个  *");
	puts("    *    bomb时无敌    *");
	puts("    *   地图大小可调   *");
	puts("    *   *************  *");
	puts("    *    东方作死⑨    *");
	puts("    *        之        *");
	puts("    *   紫妈的fes@#bu  *");
	puts("    ********************\n");
}

void start()
{
	puts("八云紫「说了咱是少女! 人家到底哪里老了! 」");
	Sleep(1000);
	puts("八云紫「天天日常年龄的其实是紫黑吧! 」");
	Sleep(1000);
	puts("八云紫「495岁的吸血鬼都是loli, 为什么人家就是老婆婆! 」");
	Sleep(1500);
	puts("八云紫「人家到底哪里老了! 17岁就是17岁, 才不是最后两个数字! 」");
	Sleep(1500);
	puts("八云紫「紫妈也就算了, 紫婆婆紫奶奶算个什么啊! 」");
	Sleep(1300);
	puts("八云紫「死基佬们, 有本事不拿人家的同人撸管子啊!  」");
	Sleep(1300);
	puts("八云紫「谁再拿年龄日常的! 送去和斯大林搞基!  」");
	Sleep(1200);
	puts("⑨    「我就是叫紫妈怎么了! 」");
	Sleep(1000);
	puts("八云紫「......」");
	Sleep(1000);
	puts("八云紫上路了......");
	Sleep(1500);
	system("cls");
	PlaySound(MAKEINTRESOURCE(IDR_WAVE4), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
	plane* _planep_ = plane_head;
	FILE* fp = NULL;
	double _x, _y, _ax, _ay;
	int _dx, _dy, _he, _sl, _nu, _breakflag = 0;
	char _type;
	fp = fopen("..\\data\\KoumakanData.txt", "r");
	if (fp == NULL)
	{
		fp = fopen("C:\\Program Files(x86)\\Koumakan", "r");
		if (fp == NULL)
		{
			fp = fopen("C:\\Koumakan\\data\\KoumakanData.txt", "r");
			if (fp == NULL)
			{
				fp = fopen("D:\\Koumakan\\data\\KoumakanData.txt", "r");
				if (fp == NULL)
				{
					fp = fopen("E:\\Koumakan\\data\\KoumakanData.txt", "r");
					if (fp == NULL)
					{
						fp = fopen("F:\\Koumakan\\data\\KoumakanData.txt", "r");
						if (fp == NULL)
						{
							fp = fopen("G:\\Koumakan\\data\\KoumakanData.txt", "r");
							if (fp == NULL)
							{
								fp = fopen("H:\\Koumakan\\data\\KoumakanData.txt", "r");
							}
							else
							{
								cout << "路径错误" << endl;
								Sleep(5000);
								exit(0);
							}
						}
					}
				}
			}
		}
	}
	int i = 1;
	if (turn == 0)
	{
		randbullet_p = new randbullet(0);
		autobullet_p = new autobullet;
		plane_p = new plane;
		map_p = new map;
		save_p = new save;
		++turn;
	}
	createmap();
	map_p->create();
	map_p->printscreen();
	while (1)
	{
		while (1)

		{
			fscanf(fp, "%c\n", &_type);
			if (_type == 'r')
			{
				fscanf(fp, "%lf %lf %d %d\n", &_x, &_y, &_dx, &_dy);
				_dx = player_p->playerpoint.pointx - 30;
				_dy = player_p->playerpoint.pointy - 5;
				while ((_dx != 1 && _dx != 0 && _dx != -1) || (_dy != 1 && _dy != 0 && _dy != -1))
				{
					if (_dx == 0 && _dy == 0)
						++_dx;
					_dx = _dx / 2;
					_dy = _dy / 2;
				}
				if (_x == 0 && _y == 0)
				{
					randbullet_p->create(1, 1, _dx, _dy, 0);
				}
				else if (_x == 1 && _y == 0)
				{
					randbullet_p->create(XMAX - 3, 1, _dx, _dy, 0);
				}
				else if (_x == 0 && _y != 0)
				{
					randbullet_p->create(1, (int)(YMAX * _y), _dx, _dy, 0);
				}
				else if (_x != 1 && _y == 0 && _x != 0)
				{
					randbullet_p->create((int)(XMAX * _x), 1, _dx, _dy, 0);
				}
				else
				{
					randbullet_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, 0);
				}
			}
			else if (_type == 'a')
			{
				fscanf(fp, "%lf %lf %d %d %d %d\n", &_x, &_y, &_dx, &_dy, &_ax, &_ay);
				if (_x == 0 && _y == 0)
				{
					if (_ax == 0 && _ay == 0)
					{
						autobullet_p->create(1, 1, _dx, _dy, 1, 1);
					}
					else if (_ax == 1 && _ay == 0)
					{
						autobullet_p->create(1, 1, _dx, _dy, XMAX - 3, 1);
					}
					else if (_ax == 0 && _ay != 0)
					{
						autobullet_p->create(1, 1, _dx, _dy, 1, _ay);
					}
					else if (_ax != 1 && _ay == 0 && _ax != 0)
					{
						autobullet_p->create(1, 1, _dx, _dy, _ax, 1);
					}
					else
					{
						autobullet_p->create(1, 1, _dx, _dy, _ax, _ay);
					}
				}
				else if (_x == 1 && _y == 0)
				{
					if (_ax == 0 && _ay == 0)
					{
						autobullet_p->create(XMAX - 3, 1, _dx, _dy, 1, 1);
					}
					else if (_ax == 1 && _ay == 0)
					{
						autobullet_p->create(XMAX - 3, 1, _dx, _dy, XMAX - 3, 1);
					}
					else if (_ax == 0 && _ay != 0)
					{
						autobullet_p->create(XMAX - 3, 1, _dx, _dy, 1, _ay);
					}
					else if (_ax != 1 && _ay == 0 && _ax != 0)
					{
						autobullet_p->create(XMAX - 3, 1, _dx, _dy, _ax, 1);
					}
					else
					{
						autobullet_p->create(XMAX - 3, 1, _dx, _dy, _ax, _ay);
					}
				}
				else if (_x == 0 && _y != 0)
				{
					if (_ax == 0 && _ay == 0)
					{
						autobullet_p->create(1, (int)(YMAX * _y), _dx, _dy, 1, 1);
					}
					else if (_ax == 1 && _ay == 0)
					{
						autobullet_p->create(1, (int)(YMAX * _y), _dx, _dy, XMAX - 3, 1);
					}
					else if (_ax == 0 && _ay != 0)
					{
						autobullet_p->create(1, (int)(YMAX * _y), _dx, _dy, 1, _ay);
					}
					else if (_ax != 1 && _ay == 0 && _ax != 0)
					{
						autobullet_p->create(1, (int)(YMAX * _y), _dx, _dy, _ax, 1);
					}
					else
					{
						autobullet_p->create(1, (int)(YMAX * _y), _dx, _dy, _ax, _ay);
					}
				}
				else if (_x != 1 && _y == 0 && _x != 0)
				{
					if (_ax == 0 && _ay == 0)
					{
						autobullet_p->create((int)(XMAX * _x), 1, _dx, _dy, 1, 1);
					}
					else if (_ax == 1 && _ay == 0)
					{
						autobullet_p->create((int)(XMAX * _x), 1, _dx, _dy, XMAX - 3, 1);
					}
					else if (_ax == 0 && _ay != 0)
					{
						autobullet_p->create((int)(XMAX * _x), 1, _dx, _dy, 1, _ay);
					}
					else if (_ax != 1 && _ay == 0 && _ax != 0)
					{
						autobullet_p->create((int)(XMAX * _x), 1, _dx, _dy, _ax, 1);
					}
					else
					{
						autobullet_p->create((int)(XMAX * _x), 1, _dx, _dy, _ax, _ay);
					}
				}
				else
				{
					if (_ax == 0 && _ay == 0)
					{
						autobullet_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, 1, 1);
					}
					else if (_ax == 1 && _ay == 0)
					{
						autobullet_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, XMAX - 3, 1);
					}
					else if (_ax == 0 && _ay != 0)
					{
						autobullet_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, 1, _ay);
					}
					else if (_ax != 1 && _ay == 0 && _ax != 0)
					{
						autobullet_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, _ax, 1);
					}
					else
					{
						autobullet_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, _ax, _ay);
					}
				}
			}
			else if (_type == 'p')
			{
				fscanf(fp, "%lf %lf %d %d %lf %lf %d %d %d\n", &_x, &_y, &_dx, &_dy, &_ax, &_ay, &_he, &_sl, &_nu);
				if (_x == 0 && _y == 0)
				{
					if (_ax == 0 && _ay == 0)
					{
						plane_p->create(1, 1, _dx, _dy, 1, 1, _he, _sl, _nu);
					}
					else if (_ax == 1 && _ay == 0)
					{
						plane_p->create(1, 1, _dx, _dy, XMAX - 3, 1, _he, _sl, _nu);
					}
					else if (_ax == 0 && _ay != 0)
					{
						plane_p->create(1, 1, _dx, _dy, 1, _ay, _he, _sl, _nu);
					}
					else if (_ax != 1 && _ay == 0 && _ax != 0)
					{
						plane_p->create(1, 1, _dx, _dy, _ax, 1, _he, _sl, _nu);
					}
					else
					{
						plane_p->create(1, 1, _dx, _dy, _ax, _ay, _he, _sl, _nu);
					}
				}
				else if (_x == 1 && _y == 0)
				{
					if (_ax == 0 && _ay == 0)
					{
						plane_p->create(XMAX - 3, 1, _dx, _dy, 1, 1, _he, _sl, _nu);
					}
					else if (_ax == 1 && _ay == 0)
					{
						plane_p->create(XMAX - 3, 1, _dx, _dy, XMAX - 3, 1, _he, _sl, _nu);
					}
					else if (_ax == 0 && _ay != 0)
					{
						plane_p->create(XMAX - 3, 1, _dx, _dy, 1, _ay, _he, _sl, _nu);
					}
					else if (_ax != 1 && _ay == 0 && _ax != 0)
					{
						plane_p->create(XMAX - 3, 1, _dx, _dy, _ax, 1, _he, _sl, _nu);
					}
					else
					{
						plane_p->create(XMAX - 3, 1, _dx, _dy, _ax, _ay, _he, _sl, _nu);
					}
				}
				else if (_x == 0 && _y != 0)
				{
					if (_ax == 0 && _ay == 0)
					{
						plane_p->create(1, (int)(YMAX * _y), _dx, _dy, 1, 1, _he, _sl, _nu);
					}
					else if (_ax == 1 && _ay == 0)
					{
						plane_p->create(1, (int)(YMAX * _y), _dx, _dy, XMAX - 3, 1, _he, _sl, _nu);
					}
					else if (_ax == 0 && _ay != 0)
					{
						plane_p->create(1, (int)(YMAX * _y), _dx, _dy, 1, _ay, _he, _sl, _nu);
					}
					else if (_ax != 1 && _ay == 0 && _ax != 0)
					{
						plane_p->create(1, (int)(YMAX * _y), _dx, _dy, _ax, 1, _he, _sl, _nu);
					}
					else
					{
						plane_p->create(1, (int)(YMAX * _y), _dx, _dy, _ax, _ay, _he, _sl, _nu);
					}
				}
				else if (_x != 1 && _y == 0 && _x != 0)
				{
					if (_ax == 0 && _ay == 0)
					{
						plane_p->create((int)(XMAX * _x), 1, _dx, _dy, 1, 1, _he, _sl, _nu);
					}
					else if (_ax == 1 && _ay == 0)
					{
						plane_p->create((int)(XMAX * _x), 1, _dx, _dy, XMAX - 3, 1, _he, _sl, _nu);
					}
					else if (_ax == 0 && _ay != 0)
					{
						plane_p->create((int)(XMAX * _x), 1, _dx, _dy, 1, _ay, _he, _sl, _nu);
					}
					else if (_ax != 1 && _ay == 0 && _ax != 0)
					{
						plane_p->create((int)(XMAX * _x), 1, _dx, _dy, _ax, 1, _he, _sl, _nu);
					}
					else
					{
						plane_p->create((int)(XMAX * _x), 1, _dx, _dy, _ax, _ay, _he, _sl, _nu);
					}
				}
				else
				{
					if (_ax == 0 && _ay == 0)
					{
						plane_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, 1, 1, _he, _sl, _nu);
					}
					else if (_ax == 1 && _ay == 0)
					{
						plane_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, XMAX - 3, 1, _he, _sl, _nu);
					}
					else if (_ax == 0 && _ay != 0)
					{
						plane_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, 1, _ay, _he, _sl, _nu);
					}
					else if (_ax != 1 && _ay == 0 && _ax != 0)
					{
						plane_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, _ax, 1, _he, _sl, _nu);
					}
					else
					{
						plane_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, _ax, _ay, _he, _sl, _nu);
					}
				}
			}
			else if (_type == 'n')
			{
				_breakflag = 1;
				break;
			}
			else if (_type == 's')
			{
				Sleep(3000);
				break;
			}
			else if (_type == 'e')
			{
				_planep_ = plane_head;
				while (_planep_ != NULL)
				{
					--_planep_->_sleepflag;
					if (_planep_->_sleepflag == 0 && _planep_->numberflag != 0)
					{
						_planep_->_sleepflag = _planep_->sleepflag;
						--_planep_->numberflag;
						randbullet_p->create(_planep_->planepoint.pointx, _planep_->planepoint.pointy + 1, 0, 1, 0);
					}
					_planep_ = _planep_->plane_next;
				}
				break;
			}
		}
		if (_breakflag == 1)
		{
			//⑨
			PlaySound(MAKEINTRESOURCE(IDR_WAVE5), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
			randbullet* __randp = randbullet_head;
			autobullet* __autop = autobullet_head;
			plane* __planep = plane_head;
			while (randbullet_head != NULL && randbullet_head->randbullet_next != NULL)
			{
				__randp = randbullet_head;
				randbullet_head = randbullet_head->randbullet_next;
				delete __randp;
				randbullet_p = randbullet_head;
			}
			while (autobullet_head != NULL && autobullet_head->autobullet_next != NULL)
			{
				__autop = autobullet_head;
				autobullet_head = autobullet_head->autobullet_next;
				delete __autop;
				autobullet_p = autobullet_head;
			}
			while (plane_head != NULL && plane_head->plane_next != NULL)
			{
				__planep = plane_head;
				plane_head = plane_head->plane_next;
				delete __planep;
				plane_p = plane_head;
			}
			nine.health = 15;
			nine.pointx = XMAX / 2;
			nine.pointy = 5;
			nineoutflag = 1;
			system("cls");
			map_p->create();
			map_p->printscreen();
			Sleep(2000);
			puts("⑨    「有本事突然从我身后出现」");
			Sleep(1000);
			puts("⑨    「把我的脸按在键盘上」");
			Sleep(1000);
			puts("八云紫 <出现>");
			Sleep(1000);
			puts("八云紫「怕是连我家橙都打不过」");
			Sleep(1500);
			fclose(fp);
			fp = fopen("..\\data\\KoumakanBossData.txt", "r");
			if (fp == NULL)
			{
				fp = fopen("C:\\Program Files(x86)\\Koumakan", "r");
				if (fp == NULL)
				{
					fp = fopen("C:\\Koumakan\\data\\KoumakanBossData.txt", "r");
					if (fp == NULL)
					{
						fp = fopen("D:\\Koumakan\\data\\KoumakanBossData.txt", "r");
						if (fp == NULL)
						{
							fp = fopen("E:\\Koumakan\\data\\KoumakanBossData.txt", "r");
							if (fp == NULL)
							{
								fp = fopen("F:\\Koumakan\\data\\KoumakanBossData.txt", "r");
								if (fp == NULL)
								{
									fp = fopen("G:\\Koumakan\\data\\KoumakanBossData.txt", "r");
									if (fp == NULL)
									{
										fp = fopen("H:\\Koumakan\\data\\KoumakanBossData.txt", "r");
									}
									else
									{
										cout << "路径错误" << endl;
										Sleep(5000);
										exit(0);
									}
								}
							}
						}
					}
				}
			}
			int i = 1;
			while (1)
			{
				while (1)
				{
					fscanf(fp, "%c\n", &_type);
					if (_type == 'r')
					{
						fscanf(fp, "%lf %lf %d %d\n", &_x, &_y, &_dx, &_dy);
						_dx = player_p->playerpoint.pointx - 30;
						_dy = player_p->playerpoint.pointy - 5;
						while ((_dx != 1 && _dx != 0 && _dx != -1) || (_dy != 1 && _dy != 0 && _dy != -1))
						{
							if (_dx == 0 && _dy == 0)
								++_dx;
							_dx = _dx / 2;
							_dy = _dy / 2;
						}
						if (_x == 0 && _y == 0)
						{
							randbullet_p->create(1, 1, _dx, _dy, 0);
						}
						else if (_x == 1 && _y == 0)
						{
							randbullet_p->create(XMAX - 3, 1, _dx, _dy, 0);
						}
						else if (_x == 0 && _y != 0)
						{
							randbullet_p->create(1, (int)(YMAX * _y), _dx, _dy, 0);
						}
						else if (_x != 1 && _y == 0 && _x != 0)
						{
							randbullet_p->create((int)(XMAX * _x), 1, _dx, _dy, 0);
						}
						else
						{
							randbullet_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, 0);
						}
					}
					else if (_type == 'a')
					{
						fscanf(fp, "%lf %lf %d %d %d %d\n", &_x, &_y, &_dx, &_dy, &_ax, &_ay);
						if (_x == 0 && _y == 0)
						{
							if (_ax == 0 && _ay == 0)
							{
								autobullet_p->create(1, 1, _dx, _dy, 1, 1);
							}
							else if (_ax == 1 && _ay == 0)
							{
								autobullet_p->create(1, 1, _dx, _dy, XMAX - 3, 1);
							}
							else if (_ax == 0 && _ay != 0)
							{
								autobullet_p->create(1, 1, _dx, _dy, 1, _ay);
							}
							else if (_ax != 1 && _ay == 0 && _ax != 0)
							{
								autobullet_p->create(1, 1, _dx, _dy, _ax, 1);
							}
							else
							{
								autobullet_p->create(1, 1, _dx, _dy, _ax, _ay);
							}
						}
						else if (_x == 1 && _y == 0)
						{
							if (_ax == 0 && _ay == 0)
							{
								autobullet_p->create(XMAX - 3, 1, _dx, _dy, 1, 1);
							}
							else if (_ax == 1 && _ay == 0)
							{
								autobullet_p->create(XMAX - 3, 1, _dx, _dy, XMAX - 3, 1);
							}
							else if (_ax == 0 && _ay != 0)
							{
								autobullet_p->create(XMAX - 3, 1, _dx, _dy, 1, _ay);
							}
							else if (_ax != 1 && _ay == 0 && _ax != 0)
							{
								autobullet_p->create(XMAX - 3, 1, _dx, _dy, _ax, 1);
							}
							else
							{
								autobullet_p->create(XMAX - 3, 1, _dx, _dy, _ax, _ay);
							}
						}
						else if (_x == 0 && _y != 0)
						{
							if (_ax == 0 && _ay == 0)
							{
								autobullet_p->create(1, (int)(YMAX * _y), _dx, _dy, 1, 1);
							}
							else if (_ax == 1 && _ay == 0)
							{
								autobullet_p->create(1, (int)(YMAX * _y), _dx, _dy, XMAX - 3, 1);
							}
							else if (_ax == 0 && _ay != 0)
							{
								autobullet_p->create(1, (int)(YMAX * _y), _dx, _dy, 1, _ay);
							}
							else if (_ax != 1 && _ay == 0 && _ax != 0)
							{
								autobullet_p->create(1, (int)(YMAX * _y), _dx, _dy, _ax, 1);
							}
							else
							{
								autobullet_p->create(1, (int)(YMAX * _y), _dx, _dy, _ax, _ay);
							}
						}
						else if (_x != 1 && _y == 0 && _x != 0)
						{
							if (_ax == 0 && _ay == 0)
							{
								autobullet_p->create((int)(XMAX * _x), 1, _dx, _dy, 1, 1);
							}
							else if (_ax == 1 && _ay == 0)
							{
								autobullet_p->create((int)(XMAX * _x), 1, _dx, _dy, XMAX - 3, 1);
							}
							else if (_ax == 0 && _ay != 0)
							{
								autobullet_p->create((int)(XMAX * _x), 1, _dx, _dy, 1, _ay);
							}
							else if (_ax != 1 && _ay == 0 && _ax != 0)
							{
								autobullet_p->create((int)(XMAX * _x), 1, _dx, _dy, _ax, 1);
							}
							else
							{
								autobullet_p->create((int)(XMAX * _x), 1, _dx, _dy, _ax, _ay);
							}
						}
						else
						{
							if (_ax == 0 && _ay == 0)
							{
								autobullet_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, 1, 1);
							}
							else if (_ax == 1 && _ay == 0)
							{
								autobullet_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, XMAX - 3, 1);
							}
							else if (_ax == 0 && _ay != 0)
							{
								autobullet_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, 1, _ay);
							}
							else if (_ax != 1 && _ay == 0 && _ax != 0)
							{
								autobullet_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, _ax, 1);
							}
							else
							{
								autobullet_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, _ax, _ay);
							}
						}
					}
					else if (_type == 'p')
					{
						fscanf(fp, "%lf %lf %d %d %lf %lf %d %d %d\n", &_x, &_y, &_dx, &_dy, &_ax, &_ay, &_he, &_sl, &_nu);
						if (_x == 0 && _y == 0)
						{
							if (_ax == 0 && _ay == 0)
							{
								plane_p->create(1, 1, _dx, _dy, 1, 1, _he, _sl, _nu);
							}
							else if (_ax == 1 && _ay == 0)
							{
								plane_p->create(1, 1, _dx, _dy, XMAX - 3, 1, _he, _sl, _nu);
							}
							else if (_ax == 0 && _ay != 0)
							{
								plane_p->create(1, 1, _dx, _dy, 1, _ay, _he, _sl, _nu);
							}
							else if (_ax != 1 && _ay == 0 && _ax != 0)
							{
								plane_p->create(1, 1, _dx, _dy, _ax, 1, _he, _sl, _nu);
							}
							else
							{
								plane_p->create(1, 1, _dx, _dy, _ax, _ay, _he, _sl, _nu);
							}
						}
						else if (_x == 1 && _y == 0)
						{
							if (_ax == 0 && _ay == 0)
							{
								plane_p->create(XMAX - 3, 1, _dx, _dy, 1, 1, _he, _sl, _nu);
							}
							else if (_ax == 1 && _ay == 0)
							{
								plane_p->create(XMAX - 3, 1, _dx, _dy, XMAX - 3, 1, _he, _sl, _nu);
							}
							else if (_ax == 0 && _ay != 0)
							{
								plane_p->create(XMAX - 3, 1, _dx, _dy, 1, _ay, _he, _sl, _nu);
							}
							else if (_ax != 1 && _ay == 0 && _ax != 0)
							{
								plane_p->create(XMAX - 3, 1, _dx, _dy, _ax, 1, _he, _sl, _nu);
							}
							else
							{
								plane_p->create(XMAX - 3, 1, _dx, _dy, _ax, _ay, _he, _sl, _nu);
							}
						}
						else if (_x == 0 && _y != 0)
						{
							if (_ax == 0 && _ay == 0)
							{
								plane_p->create(1, (int)(YMAX * _y), _dx, _dy, 1, 1, _he, _sl, _nu);
							}
							else if (_ax == 1 && _ay == 0)
							{
								plane_p->create(1, (int)(YMAX * _y), _dx, _dy, XMAX - 3, 1, _he, _sl, _nu);
							}
							else if (_ax == 0 && _ay != 0)
							{
								plane_p->create(1, (int)(YMAX * _y), _dx, _dy, 1, _ay, _he, _sl, _nu);
							}
							else if (_ax != 1 && _ay == 0 && _ax != 0)
							{
								plane_p->create(1, (int)(YMAX * _y), _dx, _dy, _ax, 1, _he, _sl, _nu);
							}
							else
							{
								plane_p->create(1, (int)(YMAX * _y), _dx, _dy, _ax, _ay, _he, _sl, _nu);
							}
						}
						else if (_x != 1 && _y == 0 && _x != 0)
						{
							if (_ax == 0 && _ay == 0)
							{
								plane_p->create((int)(XMAX * _x), 1, _dx, _dy, 1, 1, _he, _sl, _nu);
							}
							else if (_ax == 1 && _ay == 0)
							{
								plane_p->create((int)(XMAX * _x), 1, _dx, _dy, XMAX - 3, 1, _he, _sl, _nu);
							}
							else if (_ax == 0 && _ay != 0)
							{
								plane_p->create((int)(XMAX * _x), 1, _dx, _dy, 1, _ay, _he, _sl, _nu);
							}
							else if (_ax != 1 && _ay == 0 && _ax != 0)
							{
								plane_p->create((int)(XMAX * _x), 1, _dx, _dy, _ax, 1, _he, _sl, _nu);
							}
							else
							{
								plane_p->create((int)(XMAX * _x), 1, _dx, _dy, _ax, _ay, _he, _sl, _nu);
							}
						}
						else
						{
							if (_ax == 0 && _ay == 0)
							{
								plane_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, 1, 1, _he, _sl, _nu);
							}
							else if (_ax == 1 && _ay == 0)
							{
								plane_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, XMAX - 3, 1, _he, _sl, _nu);
							}
							else if (_ax == 0 && _ay != 0)
							{
								plane_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, 1, _ay, _he, _sl, _nu);
							}
							else if (_ax != 1 && _ay == 0 && _ax != 0)
							{
								plane_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, _ax, 1, _he, _sl, _nu);
							}
							else
							{
								plane_p->create((int)(XMAX * _x), (int)(YMAX * _y), _dx, _dy, _ax, _ay, _he, _sl, _nu);
							}
						}
					}
					else if (_type == 'n')
					{
						fclose(fp);
						fp = fopen("..\\data\\KoumakanBossData.txt", "r");
						if (fp == NULL)
						{
							fp = fopen("C:\\Program Files(x86)\\Koumakan", "r");
							if (fp == NULL)
							{
								fp = fopen("C:\\Koumakan\\data\\KoumakanBossData.txt", "r");
								if (fp == NULL)
								{
									fp = fopen("D:\\Koumakan\\data\\KoumakanBossData.txt", "r");
									if (fp == NULL)
									{
										fp = fopen("E:\\Koumakan\\data\\KoumakanBossData.txt", "r");
										if (fp == NULL)
										{
											fp = fopen("F:\\Koumakan\\data\\KoumakanBossData.txt", "r");
											if (fp == NULL)
											{
												fp = fopen("G:\\Koumakan\\data\\KoumakanBossData.txt", "r");
												if (fp == NULL)
												{
													fp = fopen("H:\\Koumakan\\data\\KoumakanBossData.txt", "r");
												}
												else
												{
													cout << "路径错误" << endl;
													Sleep(5000);
													exit(0);
												}
											}
										}
									}
								}
							}
						}
					}
					else if (_type == 's')
					{
						Sleep(3000);
						break;
					}
					else if (_type == 'e')
					{
						_planep_ = plane_head;
						while (_planep_ != NULL)
						{
							--_planep_->_sleepflag;
							if (_planep_->_sleepflag == 0 && _planep_->numberflag != 0)
							{
								_planep_->_sleepflag = _planep_->sleepflag;
								--_planep_->numberflag;
								randbullet_p->create(_planep_->planepoint.pointx, _planep_->planepoint.pointy + 1, 0, 1, 0);
							}
							_planep_ = _planep_->plane_next;
						}
						break;
					}
				}
				if (nine.health != 0)
				{
					if (nine.direction == 'l' && nine.pointx >= 15)
					{
						nine.changepoint(nine.pointx - 1, nine.pointy);
					}
					else if (nine.direction == 'l' && nine.pointx < 15)
					{
						nine.changepoint(nine.pointx - 1, nine.pointy);
						nine.direction = 'r';
					}
					else if (nine.direction == 'r' && nine.pointx <= 85)
					{
						nine.changepoint(nine.pointx + 1, nine.pointy);
					}
					else if (nine.direction == 'r' && nine.pointx > 85)
					{
						nine.changepoint(nine.pointx + 1, nine.pointy);
						nine.direction = 'l';
					}

					if (nine.fireflag == 1)
					{
						randbullet_p->create(nine.pointx + 3, nine.pointy, 1, 0, 0);
						randbullet_p->create(nine.pointx + 3, nine.pointy + 3, 1, 1, 0);
						randbullet_p->create(nine.pointx, nine.pointy + 3, 0, 1, 0);
						randbullet_p->create(nine.pointx - 3, nine.pointy + 3, -1, 1, 0);
						randbullet_p->create(nine.pointx - 3, nine.pointy, -3, 0, 0);
						randbullet_p->create(nine.pointx - 3, nine.pointy - 3, -1, -1, 0);
						randbullet_p->create(nine.pointx, nine.pointy - 3, 0, -1, 0);
						randbullet_p->create(nine.pointx + 3, nine.pointy - 3, 1, -1, 0);
						++nine.fireflag;
					}
					else
					{
						++nine.fireflag;
						nine.fireflag = 1;
					}
				}
				else
				{
					puts("本小姐是最强的!!");
					Sleep(2000);
					_breakflag = 1;
					map1->gameover(true);
					return;
				}
				_planep_ = plane_head;
				while (_planep_ != NULL)
				{
					--_planep_->_sleepflag;
					if (_planep_->_sleepflag == 0 && _planep_->numberflag != 0)
					{
						_planep_->_sleepflag = _planep_->sleepflag;
						--_planep_->numberflag;
						randbullet_p->create(_planep_->planepoint.pointx, _planep_->planepoint.pointy + 1, 0, 1, 0);
					}
					_planep_ = _planep_->plane_next;
				}
				autobullet* _head = autobullet_head;
				while (_head != NULL)
				{
					_head->bulletdirection.changedirection(fchangedirection(_head->bulletpoint, _head->bulletdirection, _head->bulletdatum).directionx,
						fchangedirection(_head->bulletpoint, _head->bulletdirection, _head->bulletdatum).directiony);
					_head = _head->autobullet_next;
				}
				player_p->operate();
				map_p->move(&(player_p->playerpoint), &(player_p->playerdirection));
				map_p->moveothers();
				if (gameflag == 0)
				{
					++gameflag;
					fclose(fp);
					return;
				}
				map_p->create();
				map_p->printscreen();
				printf("score:%d\n", player_p->level);
				printf("enemy's health: %d\n", nine.health);
				Sleep(_sleeptime);

			}
		}
		_planep_ = plane_head;
		while (_planep_ != NULL)
		{
			--_planep_->_sleepflag;
			if (_planep_->_sleepflag == 0 && _planep_->numberflag != 0)
			{
				_planep_->_sleepflag = _planep_->sleepflag;
				--_planep_->numberflag;
				randbullet_p->create(_planep_->planepoint.pointx, _planep_->planepoint.pointy + 1, 0, 1, 0);
			}
			_planep_ = _planep_->plane_next;
		}

		//		if (i++ % 10== 0)
		//		{
		//			//1.初始方向自定的randbullet,
		//			//2.初始方向为玩家的randbullet
		//			//3.初始方向自定的autobullet
		//			//4.能够发射randbullet和autobullet的飞机
		//
		//			int x1, y1, x2, y2, x3, y3;
		//			randbullet_p->create(XMAX / 3 * 2, YMAX - 1, 0, -1, 0);							//1
		//			x1 = player_p->playerpoint.pointx - 30;											//2
		//			y1 = player_p->playerpoint.pointy - 5;											//2
		//			while ((x1 != 1 && x1 != 0 && x1 != -1) || (y1 != 1 && y1 != 0 && y1 != -1))	//2
		//			{																				//2
		//				if (x1 == 0 && y1 == 0)														//2
		//					++x1;																	//2
		//				x1 = x1 / 2;																//2
		//				y1 = y1 / 2;																//2
		//			}																				//2
		//			randbullet_p->create(30, 1, x1, y1, 0);											//2
		//			autobullet_p->create(XMAX/4, 1, 1, 0, XMAX - 1, YMAX / 2);						//3
		//			plane_p->create(XMAX / 4, 3, 1, 0, XMAX - 1, YMAX / 2, 1/*health*/, 1/*sleep*/, 3/*number*/);    //4
		//			while (_planep_ != NULL)														//4
		//			{																				//4
		//				--_planep_->_sleepflag;														//4
		//				if (_planep_->_sleepflag == 0 && _planep_->numberflag != 0)					//4
		//				{																			//4
		//					_planep_->_sleepflag = _planep_->sleepflag;								//4
		//					--_planep_->numberflag;													//4
		//					randbullet_p->create(_planep_->planepoint.pointx, _planep_->planepoint.pointy + 1, 0, 1, 0);	//4
		//				}																			//4
		//				_planep_ = _planep_->plane_next;											//4
		//			}																				//4
		//		}

		autobullet* _head = autobullet_head;
		while (_head != NULL)
		{
			_head->bulletdirection.changedirection(fchangedirection(_head->bulletpoint, _head->bulletdirection, _head->bulletdatum).directionx,
				fchangedirection(_head->bulletpoint, _head->bulletdirection, _head->bulletdatum).directiony);
			_head = _head->autobullet_next;
		}
		player_p->operate();
		map_p->move(&(player_p->playerpoint), &(player_p->playerdirection));
		map_p->moveothers();
		if (gameflag == 0)
		{
			++gameflag;
			fclose(fp);
			return;
		}
		map_p->create();
		map_p->printscreen();
		printf("score: %d\n", player_p->level);
		Sleep(_sleeptime);
	}
}

int main()
{
	while (1)
	{
		savenumber = 0;
		hitflag = 0;
		PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
		system("cls");
		printtitle();
		{
			printf("    输入'0'退出\n    输入'1'观看回放\n    输入'2'删除回放\n    输入'3'打开东方音乐馆--红魔乡\n    输入其他数字开始游戏\n");
			scanf("%d", &cho);
			if (cho == 0)
			{
				system("cls");                     //刷新屏幕
				return 0;
			}
			else if (cho == 1)
			{
				system("cls");
				save* savep;
				if (save_head != NULL)
				{
					savep = save_head;
					while (savep != NULL)
					{
						printf("%d.    name:%s    time:%s\n", ++savenumber, savep->name, savep->chtime);
						savep = savep->save_next;
					}
					savenumber = 0;
				}
				else
				{
					printf("NO DATA\n");
					Sleep(2000);
					continue;
				}
				int n;
				scanf("%d", &n);
				int i = 1;
				savep = save_head;
				while (i < n)
				{
					++i;
					savep = savep->save_next;
				}
				savep->review(savep);
				continue;
			}
			else if (cho == 2)
			{
				system("cls");
				save* savep;
				if (save_head != NULL)
				{
					savep = save_head;
					while (savep != NULL)
					{
						printf("%d.    name:%s    time:%s\n", ++savenumber, savep->name, savep->chtime);
						savep = savep->save_next;
						savenumber = 0;
						int n;
						scanf("%d", &n);
						int i = 1;
						savep = save_head;
						while (i < n && savep->save_next != NULL)
							savep = savep->save_next;
						savep->moveout(savep);
						break;
					}
				}
				else
				{
					printf("NO DATA\n");
					Sleep(3000);
				}
			}
			else if (cho == 3)
			{
				system("cls");
				puts("1．比赤色更紅的梦");
				puts("2．如鬼灯般的紅色之魂");
				puts("3．妖魔夜行");
				puts("4．新月般的妖精");
				puts("5．活泼的纯情小姑娘");
				puts("6．上海红茶馆　～ Chinese Tea");
				puts("7．明治十七年的上海爱丽丝");
				puts("8．巴瓦鲁魔法图书馆");
				puts("9．Locked Girl　～ 少女密室");
				puts("10．女仆和血之怀表");
				puts("11．月時计 ～ LunaDial");
				puts("12．特佩斯年幼的末裔");
				puts("13．献给逝去公主的七重奏");
				puts("14．魔法少女們的百年祭");
				puts("15．她就是U.N Owen吗？");
				puts("16．比红更虚幻的永远");
				puts("17．紅楼　～ Eastern Dream...");
				puts("18．循环播放");
				int n;
				scanf("%d", &n);
				switch (n)
				{
				case 1:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("1 ．比赤色更紅的梦\n是标题画面的音乐。\n本来是打算把游戏的标题「东方红魔乡」作为曲名的，\n但因为曲调过于宁静, 所以就没有这样做(^^\n这次「红」是关键字。所以到处都充满了红色。\n");
					system("Pause");
					break;
				}
				case 2:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE2), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("2 ．如鬼灯般的紅色之魂\n暗夜中如鬼灯那样的红色圆形，飘零游移不定的魂．．．\n其实就是常见鬼火。鬼灯是季节性的植物，花语意为：「欺骗」。\n尽管是季节的产物，COMIC MARKET夏也只是在夏天，\nCOMIC MARKET冬以后再想买也许会买不到……\n");
					system("Pause");
					break;
				}
				case 3:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE3), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("3 ．妖魔夜行\n写上“妖魔夜行”这个看起来很有人类感觉的名字。\n游戏中这里没有激烈的战斗，所以给人以比较清爽的感觉\n妖魔以外的“魔”不算是“妖”么\n");
					system("Pause");
					break;
				}
				case 4:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE4), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("4 ．新月般的妖精\n在这里说的妖精不是尖耳朵那种的，是泛指所有的妖精……\n妖精以外的“精”不算是“妖”么\n");
					system("Pause");
					break;
				}
				case 5:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE5), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("5 ．活泼的纯情小姑娘\n因为想作听起来给人笨笨的感觉的曲子，所以首先曲子名就很笨。\n字读起来和文字看起来都很笨。\n在钢琴旋律中混合着异国情调的感觉。\n个人认为是很萌的～。\n");
					system("Pause");
					break;
				}
				case 6:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE6), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("6．上海红茶馆　～ Chinese Tea\n本来是打算弄成中国风味的曲子、结果弄的并不是很像（汗）\n最近都没怎么喝红茶呢。血之红茶还真是门学问呢。\n不过，蕾米莉亚她们喝的红茶其实就是人的血，真的可以称之为红茶吗？\n");
					system("Pause");
					break;
				}
				case 7:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE7), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("7．明治十七年的上海爱丽丝\n这时的上海是怎样的呢？\n明治十七年是清法战争的那年。\n有着外国租界的上海、没有比它更适合「魔都」这个名字的城市了。\n另外，所谓「魔都」不是「魔界都市 - 新宿 - 」的缩写。\n");
					system("Pause");
					break;
				}
				case 8:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE8), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("8．巴瓦鲁魔法图书馆\n有个专门收藏魔导书的图书馆真是不错啊。\n突然幻想出来的。\n其实整个世界观都是幻想出来的吧。\n巴瓦鲁只是个专有名词，没什么特殊含义。\n应该是来自法语、大概本来就没什么特别的意思吧（笑）\n");
					system("Pause");
					break;
				}
				case 9:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE9), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("9．Locked Girl　～ 少女密室\n\Locked Girl、钥匙女孩。和Key没什么特殊的关系。\n喜欢太阳晒不到的地方、不健康少女\n虽然是密室、密室和杀人没什么特殊的联系。\n我大概只能做到密室自杀吧。\n");
					system("Pause");
					break;
				}
				case 10:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE10), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("10．女仆和血之怀表\n啊，真是简单易懂的标题。易懂的事就是好事啊。\n比起女仆或女服务员，我更喜欢侍女这种称呼。\n根据统计、大部分人提到女仆都会想到年轻的女孩（什么统计啊？）、\n不过能干的女仆一般是30~40以上的。\n");
					system("Pause");
					break;
				}
				case 11:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE11), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("11．月時计 ～ LunaDial\n指的是「黄泉津平坂、暗夜行路」（藤木稟）中出现的时钟。\n我也很想知道月時计到底是怎样的东西(^^;\n（乐曲中）HARD和ROCK的部分就是（音乐标题中的）LUNA的部分。\n那个小说中提到的灵气琴、\n我在1，2年前终于知道是怎样的乐器了(^^;\n不过，即使是明治时期的上流阶级，家中会放这种东西么？\n灵气琴（テル·ミン）\n");
					system("Pause");
					break;
				}
				case 12:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE12), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("12．特佩斯年幼的末裔\n特佩斯就是指布拉德·特佩斯。\n作为德拉克拉公爵的原形而出名。\n毕竟他将2万多名俘虏全部串刺了...\n但蕾米莉亚并不是他的后裔。\n");
					system("Pause");
					break;
				}
				case 13:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE13), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("13．献给逝去公主的七重奏　　\n标题是模仿某古典名曲的。\n不过和那首曲子没任何关联。\n而且曲子也并不是真正的七重奏。只有那种气氛而已（汗）\n而且，谁也没死啊、公主又是谁？\n");
					system("Pause");
					break;
				}
				case 14:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE14), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("14．魔法少女們的百年祭\n百年一次，像是收获节般的活动。\n是要收获些什么呢（笑）\n");
					system("Pause");
					break;
				}
				case 15:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE15), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("15．她就是U.N Owen吗？\nU.N.Owen（ユナ·ナンシィ·オーエン）。\n知道的人快想起来吧。这个名字的原出处很老了（大约70年前？）\n至于为什么要把欧文这个名字写成罗马字母呢、\n那是为了让玩家对芙兰朵露有种「她到底是谁」的印象。\n芙兰朵露的第9张SPELL CARD，出处也是那里。\n");
					system("Pause");
					break;
				}
				case 16:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE16), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("16．比红更虚幻的永远\n听起来像是主题曲的改编、所以曲名也感觉差不多。\n虽然红但是没有虚幻的感觉啦(^^; \n");
					system("Pause");
					break;
				}
				case 17:
				{
					system("cls");
					PlaySound(MAKEINTRESOURCE(IDR_WAVE17), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
					puts("17．紅楼　～ Eastern Dream...\n说起来，我很久以前就有以“日语~英语”这种格式来命名曲子的习惯了…\n先不提这个了、我还有在STAFF ROLL里加上\"XXXDream\"的习惯，这次也如此了(^^; \n");
					system("Pause");
					break;
				}
				case 18:
				{
					while (!_kbhit())
					{
						system("cls");
						puts("1 ．比赤色更紅的梦\n是标题画面的音乐。\n本来是打算把游戏的标题「东方红魔乡」作为曲名的，\n但因为曲调过于宁静, 所以就没有这样做(^^\n这次「红」是关键字。所以到处都充满了红色。\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("2 ．如鬼灯般的紅色之魂\n暗夜中如鬼灯那样的红色圆形，飘零游移不定的魂．．．\n其实就是常见鬼火。鬼灯是季节性的植物，花语意为：「欺骗」。\n尽管是季节的产物，COMIC MARKET夏也只是在夏天，\nCOMIC MARKET冬以后再想买也许会买不到……\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE2), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("3 ．妖魔夜行\n写上“妖魔夜行”这个看起来很有人类感觉的名字。\n游戏中这里没有激烈的战斗，所以给人以比较清爽的感觉\n妖魔以外的“魔”不算是“妖”么\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE3), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("4 ．新月般的妖精\n在这里说的妖精不是尖耳朵那种的，是泛指所有的妖精……\n妖精以外的“精”不算是“妖”么\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE4), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("5 ．活泼的纯情小姑娘\n因为想作听起来给人笨笨的感觉的曲子，所以首先曲子名就很笨。\n字读起来和文字看起来都很笨。\n在钢琴旋律中混合着异国情调的感觉。\n个人认为是很萌的～。\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE5), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("6．上海红茶馆　～ Chinese Tea\n本来是打算弄成中国风味的曲子、结果弄的并不是很像（汗）\n最近都没怎么喝红茶呢。血之红茶还真是门学问呢。\n不过，蕾米莉亚她们喝的红茶其实就是人的血，真的可以称之为红茶吗？\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE6), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("7．明治十七年的上海爱丽丝\n这时的上海是怎样的呢？\n明治十七年是清法战争的那年。\n有着外国租界的上海、没有比它更适合「魔都」这个名字的城市了。\n另外，所谓「魔都」不是「魔界都市 - 新宿 - 」的缩写。\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE7), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("8．巴瓦鲁魔法图书馆\n有个专门收藏魔导书的图书馆真是不错啊。\n突然幻想出来的。\n其实整个世界观都是幻想出来的吧。\n巴瓦鲁只是个专有名词，没什么特殊含义。\n应该是来自法语、大概本来就没什么特别的意思吧（笑）\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE8), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("9．Locked Girl　～ 少女密室\n\Locked Girl、钥匙女孩。和Key没什么特殊的关系。\n喜欢太阳晒不到的地方、不健康少女\n虽然是密室、密室和杀人没什么特殊的联系。\n我大概只能做到密室自杀吧。\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE9), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("10．女仆和血之怀表\n啊，真是简单易懂的标题。易懂的事就是好事啊。\n比起女仆或女服务员，我更喜欢侍女这种称呼。\n根据统计、大部分人提到女仆都会想到年轻的女孩（什么统计啊？）、\n不过能干的女仆一般是30~40以上的。\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE10), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("11．月時计 ～ LunaDial\n指的是「黄泉津平坂、暗夜行路」（藤木稟）中出现的时钟。\n我也很想知道月時计到底是怎样的东西(^^;\n（乐曲中）HARD和ROCK的部分就是（音乐标题中的）LUNA的部分。\n那个小说中提到的灵气琴、\n我在1，2年前终于知道是怎样的乐器了(^^;\n不过，即使是明治时期的上流阶级，家中会放这种东西么？\n灵气琴（テル·ミン）\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE11), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("12．特佩斯年幼的末裔\n特佩斯就是指布拉德·特佩斯。\n作为德拉克拉公爵的原形而出名。\n毕竟他将2万多名俘虏全部串刺了...\n但蕾米莉亚并不是他的后裔。\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE12), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("13．献给逝去公主的七重奏　　\n标题是模仿某古典名曲的。\n不过和那首曲子没任何关联。\n而且曲子也并不是真正的七重奏。只有那种气氛而已（汗）\n而且，谁也没死啊、公主又是谁？\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE13), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("14．魔法少女們的百年祭\n百年一次，像是收获节般的活动。\n是要收获些什么呢（笑）\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE14), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("15．她就是U.N Owen吗？\nU.N.Owen（ユナ·ナンシィ·オーエン）。\n知道的人快想起来吧。这个名字的原出处很老了（大约70年前？）\n至于为什么要把欧文这个名字写成罗马字母呢、\n那是为了让玩家对芙兰朵露有种「她到底是谁」的印象。\n芙兰朵露的第9张SPELL CARD，出处也是那里。\n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE15), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("16．比红更虚幻的永远\n听起来像是主题曲的改编、所以曲名也感觉差不多。\n虽然红但是没有虚幻的感觉啦(^^; \n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE16), GetModuleHandle(NULL), SND_RESOURCE);

						system("cls");
						puts("17．紅楼　～ Eastern Dream...\n说起来，我很久以前就有以“日语~英语”这种格式来命名曲子的习惯了…\n先不提这个了、我还有在STAFF ROLL里加上\"XXXDream\"的习惯，这次也如此了(^^; \n");
						PlaySound(MAKEINTRESOURCE(IDR_WAVE17), GetModuleHandle(NULL), SND_RESOURCE);
					}
					break;
				}
				}
			}
			else
			{
				system("cls");
				HWND a = GetForegroundWindow();
				int result = MessageBox(a, TEXT("一命通关？"), TEXT("FFFFFF"), MB_ICONQUESTION | MB_YESNO);
				switch (result)
				{
				case IDYES: {player_p = new player; player_p->life = 1; break; }
				case IDNO: {player_p = new player; player_p->life = 3; break; }
				}
				start();
			}
		}
	}
	return 0;
}