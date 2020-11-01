#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "Canvas.h"
#include "Point.h"
#include <ctime>

class Monster
{
public:
	string name, icon; // 몬스터 이름과 화면 출력용 아이콘
	int nItem;         // 먹은 아이템 수
	Point q, p;        // 이전 위치(q)와 현재 위치(p)
	int nSleep;        // 쉬어야 하는 횟수
	double dist;       // 쉬고 난 다음 움직인 거리
	double total;      // 총 움직인 거리

					   // 이동이 화면을 벗어나는 경우 이동 처리를 막아준다
					   // hint : Point에 () 연산자가 존재한다
	void clip(int maxx, int maxy)
	{
		p.operator()(maxx, maxy);
	}

	double distance(Point &p1, Point &p2)
	{
		double d;
		d = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
		return d;
	}
	// 자신의 위치에 아이템이 있는경우 습득을 할 수 있음.
	// 지도의 좌표에 있는 값이 1인경우 아이템 있음, 0 인경우 없음이다.
	// 이곳에서 아이템 획득 여부와 거리 계산, 휴식 여부를 수행한다.
	// 플레이어의 경우 휴식을 하지 않는다.
	void eat(vector<vector<int> > &map, bool isMonster = true)//world를 전달 받음
	{
		total += distance(p, q);
		dist += distance(p, q);

		if (map[p.y][p.x] == 1)//아이템 있음
		{
			map[p.y][p.x] = 0;
			nItem++;
		}

		if (isMonster == true && dist > 20)//몬스터이고 dist가 20이면
		{
			nSleep = 10;
		}
	}

public:
	Monster(string n = "Monster", string i = "M", int x = 0, int y = 0)
		: name(n), icon(i), nItem(0),
		p(x, y), q(x, y), nSleep(0), dist(0.0), total(0.0)
	{}
	virtual ~Monster() {
		cout << " " << icon << "\t"
			<< "Item:" << nItem << "\t\t"
			<< "Distance:" << total << endl;
	}

	void draw(Canvas &canvas) { canvas.draw(p, icon); }

	// 몬스터의 경우는 거리를 20 이상 이동하고 나면 10번의 이동턴 동안 쉬어야 한다.
	// 몬스터는 매 이동 턴 마다 대각선, 가로, 세로, 제자리 이렇게 총 9가지 이동중 한가지 이동을 하는 것을 기본 동작으로 한다.
	// 기본 몬스터의 이동 거리는 매턴당 최대 1칸이다.
	virtual void move(vector<vector<int> > &map, int maxx, int maxy)
	{
		while (nSleep > 0)
		{
			p.x = q.x;
			p.y = q.y;
			nSleep -= 1;
			dist = 0;
			return;
		}
		int n = 1 + (rand() % 9);
		q.x = p.x;//이전의 이동한 위치 받아오기
		q.y = p.y;
		switch (n)//1부터 9까지 랜덤 실행
		{
		case 1:
			p.x += 1;
			break;
		case 2:
			p.y += 1;
			break;
		case 3:
			p.x += 1;
			p.y += 1;
			break;
		case 4:
			p.x -= 1;
			break;
		case 5:
			p.y -= 1;
			break;
		case 6:
			p.x -= 1;
			p.y -= 1;
			break;
		case 7:
			p.x += 1;
			p.y -= 1;
			break;
		case 8:
			p.x -= 1;
			p.y += 1;
			break;
		case 9://제자리
			p.x = p.x;
			p.y = p.y;
			break;
		}
		if (p.x >= maxx || p.y >= maxy || p.x < 0 || p.y < 0)
		{
			clip(maxx, maxy);
			eat(map);
		}
		else
		{
			eat(map);
		}
	}
	void print() { cout << "\t" << name << " " << icon << "\t" << nItem << "\t" << nSleep << endl; }
};

#endif