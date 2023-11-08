#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Shapes.cpp"

using namespace sf;

Texture Red, Yellow, Blue, Green, Cyan, Purple, Pink;
int score = 0, level = 1;      //счёт (необязательно)
RenderWindow window(sf::VideoMode(800, 600), "Tetris");



std::vector<object*> objects;       //массив указателей на все обьекты

int checkpos(int x, int y)      //проверяем, находится ли что-то по координатам и возвращаем номер этого объекта
{
    for (int n = 0; n < objects.size(); n++)    //для всех объектов
        if (objects[n]->iflocated(x, y))
            return n;
    return -1;
}
bool canmove(int num, direction rt)
{
    for (int n = 0; n < objects.size(); n++)        //для всех фигур
    {
        if (n != num)       //если фигуры разные
            for (int i = 0; i < objects[num]->getsize(); i++)       //для каждого блока этого объекта
            {
                switch (rt)
                {
                case RIGHT:
                    if (objects[n]->iflocated(objects[num]->getxcord(i) + 1, objects[num]->getycord(i)))        //у другого объекта есть что-то справа?
                        return 0;
                    break;
                case DOWN:
                    if (objects[n]->iflocated(objects[num]->getxcord(i), objects[num]->getycord(i) + 1))        //у другого объекта есть что-то снизу?
                        return 0;
                    break;
                case LEFT:
                    if (objects[n]->iflocated(objects[num]->getxcord(i) - 1, objects[num]->getycord(i)))        //у другого объекта есть что-то слева?
                        return 0;
                    break;
                }
            }
        for (int i = 0; i < objects[num]->getsize(); i++)
        {
            if (objects[num]->getycord(i) + 2 == height + 1)        //если объект у нижнего края
                return 0;
            if ((objects[num]->getxcord(i) + 1 == width && rt == RIGHT) || (objects[num]->getxcord(i) - 1 < 0 && rt == LEFT))      //если у боковых стенок, при попытке движения в эту сторону
                return 0;
        }
    }
    return 1;
}
void move()     //симуляция движения
{
    srand(time(0));     //рандомизируем все значения
    bool nobodymove = 1;        //находится ли хоть кто-то в движении?
    for (int n = 0; n < objects.size(); n++)
        if (objects[n]->ifmoving() && canmove(n, DOWN))         //если какой-то объект двигается и может двигаться вниз, мы его перемещаем
        {
            objects[n]->run(DOWN);
            nobodymove = 0;
        }
        else
            objects[n]->stop();         // иначе стоп

    if (nobodymove)      //если никто не двигается
    {
        //создаём сверху объект случайным образом
        switch (rand() % 7)
        {
        case 0: objects.push_back(new square(2 + rand() % (width - 4), 0, Purple));
            break;
        case 1: objects.push_back(new line(2 + rand() % (width - 4), 0, Blue));
            break;
        case 2: objects.push_back(new Jshape(2 + rand() % (width - 4), 0, Red));
            break;
        case 3: objects.push_back(new Lshape(2 + rand() % (width - 4), 0, Green));
            break;
        case 4: objects.push_back(new Zshape(2 + rand() % (width - 4), 0, Cyan));
            break;
        case 5: objects.push_back(new Tshape(2 + rand() % (width - 4), 0, Yellow));
            break;
        case 6: objects.push_back(new Sshape(2 + rand() % (width - 4), 0, Pink));
            break;
        }
        
        for (size_t i = 0; i < objects.size(); i++)
        {
            objects[i]->updateTexture();
        }

    }
    int numready = 0;       //считаем кол-во одновременно находящихся в одной строке блоков
    int y = 0;      //"y" этой строки
    for (y = 3; y < height; y++)        //для всех строк
    {
        for (int n = 0; n < objects.size(); n++)        //для всех объектов
            for (int x = 0; x < width; x++)             // для каждого столбца
                if (!objects[n]->ifmoving() && objects[n]->iflocated(x, y))     //если что-то есть в этой точке
                    numready++;
                //else continue;
        if (numready == width)      //если что-то есть на всей строчке
        {
            for (int n = 0; n < objects.size(); n++)
                if (!objects[n]->ifmoving())
                    objects[n]->shiftdown(y);      //всё сдвигаем вниз
            score += 10;
            level += 1;
        }
        numready = 0;
    }
}

bool canturn(int num)
{
    objects[num]->turn(); 
    for (int i = 0; i < objects.size(); i++)
        if(i != num)
        for (int j = 0; j < 4; j++)
            if ((objects[num]->iflocated(objects[i]->getxcord(j), objects[i]->getycord(j) ))
                || objects[num]->getycord(j) + 1 >= height
                || objects[num]->getxcord(j) >= width || objects[num]->getxcord(j) < 0)
            {
                objects[num]->turn(); objects[num]->turn(); objects[num]->turn();
                return 0;
            }
    objects[num]->turn(); objects[num]->turn(); objects[num]->turn();
    return 1;
}

int main()
{
    
    window.setFramerateLimit(60);
    
    Red.loadFromFile("Red.png");
    Yellow.loadFromFile("Yellow.png");
    Blue.loadFromFile("Blue.png");
    Green.loadFromFile("Green.png");
    Cyan.loadFromFile("Cyan.png");
    Purple.loadFromFile("Purple.png");
    Pink.loadFromFile("Pink.png");
    RectangleShape back({ cubeSize * width, cubeSize * height}); back.setFillColor(Color(200, 200, 200));
    back.setPosition({ 20, 20 }); 
    VertexArray backLines(Lines); backLines.resize((width + height - 1) * 2);
    for (int i = 0; i < (width + height - 1) * 2; i += 2){
        if (i < (width - 1) * 2){
            backLines[i].position = Vector2f{ 20.0f + cubeSize * ((i / 2) + 1), 20.0f };
            backLines[i + 1].position = Vector2f{ 20.0f + cubeSize * ((i / 2) + 1), 20.0f + cubeSize * height };
        }
        else {
            backLines[i].position = { 20.0f, 20.0f + cubeSize * (((i - (width - 1) * 2) / 2) + 1) };
            backLines[i + 1].position = { 20.0 + cubeSize * width, 20.0f + cubeSize * (((i - (width - 1) * 2) / 2) + 1) };
        }
    }

    bool pressed = 0; int timer = 0; int counter = 4;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (!pressed)
            {
                for (int n = 0; n < objects.size(); n++)
                    if (objects[n]->ifmoving())
                    {
                        if(canturn(n))
                            objects[n]->turn();
                        objects[n]->updateTexture();

                    }
            }
            pressed = 1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            if (!pressed)
            {
                for (int n = 0; n < objects.size(); n++)
                    if (objects[n]->ifmoving() && canmove(n, RIGHT))
                        objects[n]->run(RIGHT);
            }
            pressed = 1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            if (!pressed)
            {
                for (int n = 0; n < objects.size(); n++)
                    if (objects[n]->ifmoving() && canmove(n, LEFT))
                        objects[n]->run(LEFT);
            }
            pressed = 1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if (!pressed)
            {
                for (int n = 0; n < objects.size(); n++)
                    if (objects[n]->ifmoving() && canmove(n, DOWN))
                        objects[n]->run(DOWN);
            }
            pressed = 1;
        }
        if(!Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::Right)&&
            !Keyboard::isKeyPressed(Keyboard::Down)&& !Keyboard::isKeyPressed(Keyboard::Left)) pressed = 0;
        if (timer >= 20.0/sqrt(level))
        {
            move();
            for (int n = 0; n < objects.size(); n++)
                for (int x = 0; x < width; x++)
                    if (!objects[n]->ifmoving() && objects[n]->iflocated(x, 1))
                    {
                        std::cout << "Game over";
                        timer = -999;
                    }
            timer = 0;
        }
        timer++;
        window.clear();
        window.draw(back);
        window.draw(backLines);
        for (size_t i = 0; i < objects.size(); i++)
        {
            counter = 4;
            if (!objects[i]->isDeleted[0])
                window.draw(objects[i]->figure[0]);
            else counter--;
            if (!objects[i]->isDeleted[1])
                window.draw(objects[i]->figure[1]);
            else counter--;
            if (!objects[i]->isDeleted[2])
                window.draw(objects[i]->figure[2]);
            else counter--;
            if (!objects[i]->isDeleted[3])
                window.draw(objects[i]->figure[3]);
            else counter--;
            if (counter == 0)
                objects.erase(objects.begin() + i);
        }
        window.display();
        
    }
}

