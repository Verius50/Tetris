#include <SFML/Graphics.hpp>
#include <vector>
#include "AI.h"

enum direction { LEFT = 1, UP, RIGHT, DOWN };
using namespace sf;


class object
{
protected:
    std::vector<int> xcords, ycords;        //����������
    
    bool condition = true;      //�����e��� ��� ���
public:
    direction angle = UP;
    Sprite figure[4];
    Texture tx;
    bool isDeleted[4] = { 0, 0, 0, 0 };
    bool iflocated(int x, int y)        //��������� �� �����-�� ���� ������ �� �����������
    {
        for (int n = 0; n < xcords.size(); n++)
            if (x == xcords[n] && y == ycords[n])
                return 1;
        return 0;
    }
    void run(direction rt)      //�������� ������ � �������
    {
        switch (rt)
        {
        case DOWN:
            for (int n = 0; n < ycords.size(); n++)
                ycords[n]++;
            break;
        case RIGHT:
            for (int n = 0; n < xcords.size(); n++)
                xcords[n]++;
            break;
        case LEFT:
            for (int n = 0; n < xcords.size(); n++)
                xcords[n]--;
            break;
        }
        for (int i = 0; i < 4; i++)
            if(!isDeleted[i])
                figure[i].setPosition({ 20.0f + cubeSize * xcords[i], 20.0f + cubeSize * ycords[i] });
    }
    bool ifmoving() const
    {
        return condition;
    }

    int getsize() const
    {
        return xcords.size();
    }

    int getxcord(int n)
    {
        return xcords[n];
    }
    int getycord(int n)
    {
        return ycords[n];
    }
    void stop()
    {
        condition = 0;
    }
    void shiftdown(int y)       //�������� ������, ��� ������ �������� � �����, �� ��� ���� �� 1 ����
    {
        for (int n = 0; n < xcords.size(); n++)
            if (ycords[n] == y)
            {
                ycords[n] = NULL;
                isDeleted[n] = 1;
            }
            else if (ycords[n] != NULL && ycords[n] < y)
                ycords[n]++;
        this->updateTexture();
    }
    virtual void turn()     //��� ������������
    {}
    void updateTexture()
    {
        figure[0].setTexture(tx);   figure[1].setTexture(tx);
        figure[2].setTexture(tx);   figure[3].setTexture(tx);
        for (int i = 0; i < 4; i++)
            if (!isDeleted[i])
                figure[i].setPosition({ 20.0f + cubeSize * xcords[i], 20.0f + cubeSize * ycords[i] });
    }
};
class square : public object        //�������
{
public:
    square(int x, int y, Texture txt)        //�����������, ��������� ��� ����������
    {
        xcords.push_back(x); xcords.push_back(x + 1); xcords.push_back(x); xcords.push_back(x + 1);
        ycords.push_back(y); ycords.push_back(y); ycords.push_back(y + 1); ycords.push_back(y + 1);
        tx = txt;
    }
    void turn() {}          //������������ ��� �� ����
};
class line : public object      //�����
{
public:
    line(int x, int y, Texture txt)        //�����������, ��������� ��� ����������
    {
        xcords.push_back(x); xcords.push_back(x + 1); xcords.push_back(x + 2); xcords.push_back(x + 3);
        ycords.push_back(y); ycords.push_back(y); ycords.push_back(y); ycords.push_back(y);
        tx = txt;
    }
    void turn()     //������� (�� ������� �������)
    {
        if (angle == UP || angle == DOWN)
        {
            xcords[0] += 2; xcords[1] += 1; xcords[3] -= 1;
            ycords[0] -= 2; ycords[1] -= 1; ycords[3] += 1;
            angle = RIGHT;
        }
        else
        {
            xcords[0] -= 2; xcords[1] -= 1; xcords[3] += 1;
            ycords[0] += 2; ycords[1] += 1; ycords[3] -= 1;
            angle = UP;
        }

    }
};

class Jshape : public object        //������ ����� J
{
public:
    Jshape(int x, int y, Texture txt)        //�����������, ��������� ��� ����������
    {
        xcords.push_back(x); xcords.push_back(x + 1); xcords.push_back(x + 2); xcords.push_back(x + 2);
        ycords.push_back(y); ycords.push_back(y); ycords.push_back(y); ycords.push_back(y + 1);
        tx = txt;
    }
    void turn()
    {
        switch (angle)
        {
        case UP:
            xcords[0] += 1; xcords[2] -= 1; xcords[3] -= 2;
            ycords[0] -= 1; ycords[2] += 1; ycords[3] += 0;
            angle = RIGHT;
            break;
        case RIGHT:
            xcords[0] += 1; xcords[2] -= 1; xcords[3] += 0;
            ycords[0] += 1; ycords[2] -= 1; ycords[3] -= 2;
            angle = DOWN;
            break;
        case DOWN:
            xcords[0] -= 1; xcords[2] += 1; xcords[3] += 2;
            ycords[0] += 1; ycords[2] -= 1; ycords[3] += 0;
            angle = LEFT;
            break;
        case LEFT:
            xcords[0] -= 1; xcords[2] += 1; xcords[3] -= 0;
            ycords[0] -= 1; ycords[2] += 1; ycords[3] += 2;
            angle = UP;
            break;
        }
    }
};

class Lshape : public object        //������ ����� L
{
public:
    Lshape(int x, int y, Texture txt)        //�����������, ��������� ��� ����������
    {
        xcords.push_back(x); xcords.push_back(x); xcords.push_back(x); xcords.push_back(x + 1);
        ycords.push_back(y); ycords.push_back(y + 1); ycords.push_back(y + 2); ycords.push_back(y + 2);
        tx = txt;
    }
    void turn()     //������� (�� ������� �������)
    {
        switch (angle)
        {
        case LEFT:
            xcords[0] += 2; xcords[1] += 1; xcords[3] += 1;
            ycords[0] -= 2; ycords[1] -= 1; ycords[3] += 1;
            angle = UP;
            break;
        case UP:
            xcords[0] += 2; xcords[1] += 1; xcords[3] -= 1;
            ycords[0] += 2; ycords[1] += 1; ycords[3] += 1;
            angle = RIGHT;
            break;
        case RIGHT:
            xcords[0] -= 2; xcords[1] -= 1; xcords[3] -= 1;
            ycords[0] += 2; ycords[1] += 1; ycords[3] -= 1;
            angle = DOWN;
            break;
        case DOWN:
            xcords[0] -= 2; xcords[1] -= 1; xcords[3] += 1;
            ycords[0] -= 2; ycords[1] -= 1; ycords[3] -= 1;
            angle = LEFT;
            break;
        }

    }
};

class Zshape : public object        //������ ����� Z
{
public:
    Zshape(int x, int y, Texture txt)        //�����������, ��������� ��� ����������
    {
        xcords.push_back(x); xcords.push_back(x + 1); xcords.push_back(x + 1); xcords.push_back(x + 2);
        ycords.push_back(y); ycords.push_back(y); ycords.push_back(y + 1); ycords.push_back(y + 1);
        tx = txt;
    }
    void turn()     //������� (�� ������� �������)
    {
        switch (angle)
        {
        case LEFT:
            xcords[0] += 0; xcords[1] += 1; xcords[3] += 1;
            ycords[0] -= 2; ycords[1] -= 1; ycords[3] += 1;
            angle = UP;
            break;
        case UP:
            xcords[0] += 2; xcords[1] += 1; xcords[3] -= 1;
            ycords[0] -= 0; ycords[1] += 1; ycords[3] += 1;
            angle = RIGHT;
            break;
        case RIGHT:
            xcords[0] -= 0; xcords[1] -= 1; xcords[3] -= 1;
            ycords[0] += 2; ycords[1] += 1; ycords[3] -= 1;
            angle = DOWN;
            break;
        case DOWN:
            xcords[0] -= 2; xcords[1] -= 1; xcords[3] += 1;
            ycords[0] -= 0; ycords[1] -= 1; ycords[3] -= 1;
            angle = LEFT;
            break;
        }
    }
};

class Tshape : public object        //������ ����� T
{
public:
    Tshape(int x, int y, Texture txt)        //�����������, ��������� ��� ����������
    {
        xcords.push_back(x); xcords.push_back(x - 1); xcords.push_back(x); xcords.push_back(x + 1);
        ycords.push_back(y); ycords.push_back(y + 1); ycords.push_back(y + 1); ycords.push_back(y + 1);
        tx = txt;
    }
    void turn()     //������� (�� ������� �������)
    {
        switch (angle)
        {
        case LEFT:
            xcords[0] += 1; xcords[1] -= 1; xcords[3] += 1;
            ycords[0] -= 1; ycords[1] -= 1; ycords[3] += 1;
            angle = UP;
            break;
        case UP:
            xcords[0] += 1; xcords[1] += 1; xcords[3] -= 1;
            ycords[0] += 1; ycords[1] -= 1; ycords[3] += 1;
            angle = RIGHT;
            break;
        case RIGHT:
            xcords[0] -= 1; xcords[1] += 1; xcords[3] -= 1;
            ycords[0] += 1; ycords[1] += 1; ycords[3] -= 1;
            angle = DOWN;
            break;
        case DOWN:
            xcords[0] -= 1; xcords[1] -= 1; xcords[3] += 1;
            ycords[0] -= 1; ycords[1] += 1; ycords[3] -= 1;
            angle = LEFT;
            break;
        }
    }
};

class Sshape : public object        //������ ����� S
{
public:
    Sshape(int x, int y, Texture txt)        //�����������, ��������� ��� ����������
    {
        xcords.push_back(x); xcords.push_back(x + 1); xcords.push_back(x + 1); xcords.push_back(x + 2);
        ycords.push_back(y + 1); ycords.push_back(y + 1); ycords.push_back(y); ycords.push_back(y);
        tx = txt;
    }
    void turn()     //������� (�� ������� �������)
    {
        switch (angle)
        {
        case LEFT:
            xcords[0] -= 2; xcords[1] -= 1; xcords[3] += 1;
            ycords[0] -= 0; ycords[1] += 1; ycords[3] += 1;
            angle = UP;
            break;
        case UP:
            xcords[0] += 0; xcords[1] -= 1; xcords[3] -= 1;
            ycords[0] -= 2; ycords[1] -= 1; ycords[3] += 1;
            angle = RIGHT;
            break;
        case RIGHT:
            xcords[0] += 2; xcords[1] += 1; xcords[3] -= 1;
            ycords[0] -= 0; ycords[1] -= 1; ycords[3] -= 1;
            angle = DOWN;
            break;
        case DOWN:
            xcords[0] -= 0; xcords[1] += 1; xcords[3] += 1;
            ycords[0] += 2; ycords[1] += 1; ycords[3] -= 1;
            angle = LEFT;
            break;
        }
    }
};