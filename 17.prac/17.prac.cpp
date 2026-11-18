#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <cmath>
using namespace std;

struct Point
{
    int x;
    int y;
};

void gotoxy(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


class Shape
{
protected:
    string type;

public:
    Shape(string type)
    {
        this->type = type;
    }

    virtual void Print() const = 0;
    virtual void Save(ofstream& file) const = 0;

    virtual ~Shape()
    {
        cout << "Shape destructor: " << type << endl;
    }
};


class Line : public Shape
{
private:
    Point start;
    Point end;

public:
    Line(Point start, Point end) : Shape("Line")
    {
        this->start = start;
        this->end = end;
    }

    void Print() const override
    {
        int dx = end.x - start.x;
        int dy = end.y - start.y;

        int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

        if (steps == 0)
        {
            gotoxy(start.x, start.y);
            cout << "*";
            return;
        }

        for (int i = 0; i <= steps; i++)
        {
            int x = start.x + dx * i / steps;
            int y = start.y + dy * i / steps;

            gotoxy(x, y);
            cout << "*";
        }
    }

    void Save(ofstream& file) const override
    {
        file << type << " "
            << start.x << " " << start.y << " "
            << end.x << " " << end.y << endl;
    }

    ~Line() override
    {
        cout << "Line destructor" << endl;
    }
};


class Rectangle : public Shape
{
private:
    Point start;
    int width;
    int height;

public:
    Rectangle(Point start, int width, int height)
        : Shape("Rectangle")
    {
        this->start = start;
        this->width = width;
        this->height = height;
    }

    void Print() const override
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                gotoxy(start.x + x, start.y + y);

                if (y == 0 || y == height - 1 ||
                    x == 0 || x == width - 1)
                {
                    cout << "*";
                }
            }
        }
    }

    void Save(ofstream& file) const override
    {
        file << type << " "
            << start.x << " " << start.y << " "
            << width << " " << height << endl;
    }

    ~Rectangle() override
    {
        cout << "Rectangle destructor" << endl;
    }
};


class Polyline : public Shape
{
private:
    Point* points;
    int size;

public:
    Polyline(Point* arr, int size) : Shape("Polyline")
    {
        this->size = size;
        points = new Point[size];

        for (int i = 0; i < size; i++)
        {
            points[i] = arr[i];
        }
    }

    void DrawLine(Point a, Point b) const
    {
        int dx = b.x - a.x;
        int dy = b.y - a.y;

        int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

        if (steps == 0)
        {
            gotoxy(a.x, a.y);
            cout << "*";
            return;
        }

        for (int i = 0; i <= steps; i++)
        {
            int x = a.x + dx * i / steps;
            int y = a.y + dy * i / steps;

            gotoxy(x, y);
            cout << "*";
        }
    }

    void Print() const override
    {
        for (int i = 0; i < size - 1; i++)
        {
            DrawLine(points[i], points[i + 1]);
        }
    }

    void Save(ofstream& file) const override
    {
        file << type << " " << size;

        for (int i = 0; i < size; i++)
        {
            file << " " << points[i].x
                << " " << points[i].y;
        }

        file << endl;
    }

    ~Polyline() override
    {
        delete[] points;
        cout << "Polyline destructor" << endl;
    }
};

int main()
{
    Shape* shapes[3];

    shapes[0] = new Line({ 2, 2 }, { 20, 8 });
    shapes[1] = new Rectangle({ 25, 2 }, 15, 8);

    Point arr[] = {
        { 2, 12 },
        { 10, 18 },
        { 20, 12 },
        { 30, 18 }
    };

    shapes[2] = new Polyline(arr, 4);

    system("cls");


    for (int i = 0; i < 3; i++)
    {
        shapes[i]->Print();
    }


    ofstream file("shapes.txt");

    if (file.is_open())
    {
        for (int i = 0; i < 3; i++)
        {
            shapes[i]->Save(file);
        }

        file.close();
    }
    else
    {
        gotoxy(0, 22);
        cout << "File error!" << endl;
    }


    gotoxy(0, 23);


    for (int i = 0; i < 3; i++)
    {
        delete shapes[i];
    }

    return 0;
}
