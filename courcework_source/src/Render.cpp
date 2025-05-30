#include "../include/Render.h"
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <iomanip>
#include <sstream>
#include "../include/GUItextRectangle.h"
#include <algorithm>
#include <iostream>


#ifdef _DEBUG
#include <Debugapi.h> 
struct debug_print
{
    template<class C>
    debug_print& operator<<(const C& a)
    {
        OutputDebugStringA((std::stringstream() << a).str().c_str());
        return *this;
    }
} debout;
#else
struct debug_print
{
    template<class C>
    debug_print& operator<<(const C& a)
    {
        return *this;
    }
} debout;
#endif

//библиотека для разгрузки изображений
//https://github.com/nothings/stb
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//внутренняя логика "движка"
#include "MyOGL.h"
extern OpenGL gl;
#include "Light.h"
Light light;
#include "Camera.h"
Camera camera;


bool texturing = true;
bool lightning = true;
bool alpha = false;


//переключение режимов освещения, текстурирования, альфаналожения
void switchModes(OpenGL* sender, KeyEventArg arg)
{
    //конвертируем код клавиши в букву
    auto key = LOWORD(MapVirtualKeyA(arg.key, MAPVK_VK_TO_CHAR));

    switch (key)
    {
    case 'L':
        lightning = !lightning;
        break;
    case 'T':
        texturing = !texturing;
        break;
    case 'A':
        alpha = !alpha;
        break;
    }
}


GuiTextRectangle text;

GLuint texId;

void initRender()
{
    //==============НАСТРОЙКА ТЕКСТУР================
    //4 байта на хранение пикселя
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);


    glGenTextures(1, &texId);


    glBindTexture(GL_TEXTURE_2D, texId);


    int x, y, n;

    unsigned char* data = stbi_load("../resources/texture.png", &x, &y, &n, 4);

    unsigned char* _tmp = new unsigned char[x * 4];
    for (int i = 0; i < y / 2; ++i)
    {
        std::memcpy(_tmp, data + i * x * 4, x * 4);
        std::memcpy(data + i * x * 4, data + (y - 1 - i) * x * 4, x * 4);
        std::memcpy(data + (y - 1 - i) * x * 4, _tmp, x * 4);
    }
    delete[] _tmp;


    //загрузка изображения в видеопамять
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    //выгрузка изображения из опперативной памяти
    stbi_image_free(data);


    //настройка режима наложения текстур
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //настройка фильтрации
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //======================================================

    //================НАСТРОЙКА КАМЕРЫ======================
    camera.caclulateCameraPos();

    //привязываем камеру к событиям "движка"
    gl.WheelEvent.reaction(&camera, &Camera::Zoom);
    gl.MouseMovieEvent.reaction(&camera, &Camera::MouseMovie);
    gl.MouseLeaveEvent.reaction(&camera, &Camera::MouseLeave);
    gl.MouseLdownEvent.reaction(&camera, &Camera::MouseStartDrag);
    gl.MouseLupEvent.reaction(&camera, &Camera::MouseStopDrag);
    //==============НАСТРОЙКА СВЕТА===========================
    //привязываем свет к событиям "движка"
    gl.MouseMovieEvent.reaction(&light, &Light::MoveLight);
    gl.KeyDownEvent.reaction(&light, &Light::StartDrug);
    gl.KeyUpEvent.reaction(&light, &Light::StopDrug);
    //========================================================
    //====================Прочее==============================
    gl.KeyDownEvent.reaction(switchModes);
    text.setSize(512, 180);
    //========================================================


    camera.setPosition(2, 1.5, 1.5);
}

class Point {
public:
    GLdouble x;
    GLdouble y;

    Point(GLdouble x, GLdouble y) {
        this->x = x;
        this->y = y;
    }
};

void setRandomColor(int index) {
    double r, g, b;

    r = 0.1;
    g = 0.1;
    b = 0.1;
    glColor3d(r, g, b);

}

void border(Point p1, Point p2, int index) {
    glBegin(GL_QUADS);
    setRandomColor(index);
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    // Нормаль: (-dy, dx, 0) - перпендикулярна вектору (dx, dy) в плоскости XY
    double length = sqrt(dx * dx + dy * dy);
    if (length > 0) {
        glNormal3d(dy / length, -dx / length, 0);
    }

    glVertex3d(p1.x, p1.y, 0);
    glVertex3d(p1.x, p1.y, 3);
    glVertex3d(p2.x, p2.y, 3);
    glVertex3d(p2.x, p2.y, 0);
    glEnd();

    //glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex2d((p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0);
    glVertex2d((p1.x + p2.x) / 2.0 + (dy / length) * 2, (p1.y + p2.y) / 2.0 + (-dx / length) * 2);
    glEnd();

}

void topAndBottom(Point p1, Point p2, Point p3, Point p4,
    Point p5, Point p6, Point p7, Point p8, int index) {


    // Верхняя грань (с текстурой)
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);

    // Рассчитываем границы для текстурных координат
    double minX = -5;
    double maxX = 7;
    double minY = -7;
    double maxY = 9;

    // Рисуем с текстурными координатами
    glNormal3d(0, 0, 1);
    glBegin(GL_POLYGON);
    // Текстурные координаты в диапазоне [0,1]
    glTexCoord2d((p1.x - minX) / (maxX - minX), (p1.y - minY) / (maxY - minY)); glVertex3d(p1.x, p1.y, 3);
    glTexCoord2d((p2.x - minX) / (maxX - minX), (p2.y - minY) / (maxY - minY)); glVertex3d(p2.x, p2.y, 3);
    glTexCoord2d((p3.x - minX) / (maxX - minX), (p3.y - minY) / (maxY - minY)); glVertex3d(p3.x, p3.y, 3);
    glTexCoord2d((p4.x - minX) / (maxX - minX), (p4.y - minY) / (maxY - minY)); glVertex3d(p4.x, p4.y, 3);
    glTexCoord2d((p5.x - minX) / (maxX - minX), (p5.y - minY) / (maxY - minY)); glVertex3d(p5.x, p5.y, 3);
    glTexCoord2d((p6.x - minX) / (maxX - minX), (p6.y - minY) / (maxY - minY)); glVertex3d(p6.x, p6.y, 3);
    glTexCoord2d((p7.x - minX) / (maxX - minX), (p7.y - minY) / (maxY - minY)); glVertex3d(p7.x, p7.y, 3);
    glTexCoord2d((p8.x - minX) / (maxX - minX), (p8.y - minY) / (maxY - minY)); glVertex3d(p8.x, p8.y, 3);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // Нижняя грань (без текстуры)
    glEnable(GL_BLEND);
    //задаем масштабные факторы
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4d(0, 0.6, 0.3, 0.7);
    glNormal3d(0, 0, -1);
    glBegin(GL_POLYGON);
    glVertex3d(p1.x, p1.y, 0);
    glVertex3d(p2.x, p2.y, 0);
    glVertex3d(p3.x, p3.y, 0);
    glVertex3d(p4.x, p4.y, 0);
    glVertex3d(p5.x, p5.y, 0);
    glVertex3d(p6.x, p6.y, 0);
    glVertex3d(p7.x, p7.y, 0);
    glVertex3d(p8.x, p8.y, 0);
    glEnd();
    glDisable(GL_BLEND);
}



void createBoreders() {

    int i = 10;
    border(Point(-2, 9), Point(-5, 2), i);
    border(Point(-5, 2), Point(-1, 0), i);
    border(Point(-1, 0), Point(-4, -3), i);
    border(Point(-4, -3), Point(0, -7), i);
    border(Point(0, -7), Point(7, -2), i);
    border(Point(7, -2), Point(1, 0), i);
    border(Point(1, 0), Point(2, 6), i);
    border(Point(2, 6), Point(-2, 9), i);

    glNormal3d(0, 0, 1);
    glNormal3d(0, 0, -1);

    topAndBottom
    (Point(1, 0), Point(2, 6), Point(-2, 9), Point(-5, 2), Point(-1, 0), Point(-4, -3), Point(0, -7), Point(7, -2), i);
}

void Render(double delta_time)
{
    glEnable(GL_DEPTH_TEST);


    if (gl.isKeyPressed('F')) //если нажата F - свет из камеры
    {
        light.SetPosition(camera.x(), camera.y(), camera.z());
    }
    camera.SetUpCamera();
    light.SetUpLight();


    //рисуем оси
    gl.DrawAxes();

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);


    //включаем режимы, в зависимости от нажания клавиш. см void switchModes(OpenGL *sender, KeyEventArg arg)
    if (lightning)
        glEnable(GL_LIGHTING);
    if (texturing)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0); //сбрасываем текущую текстуру
    }

    if (alpha)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    //=============НАСТРОЙКА МАТЕРИАЛА==============


    //настройка материала, все что рисуется ниже будет иметь этот метериал.
    //массивы с настройками материала
    float  amb[] = { 0.2, 0.2, 0.1, 1. };
    float dif[] = { 0.4, 0.65, 0.5, 1. };
    float spec[] = { 0.9, 0.8, 0.3, 1. };
    float sh = 0.2f * 256;
    //фоновая
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    //дифузная
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
    //зеркальная
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    //размер блика
    glMaterialf(GL_FRONT, GL_SHININESS, sh);


    glShadeModel(GL_SMOOTH);
    //(GL_SMOOTH - плоская закраска)

    glBindTexture(GL_TEXTURE_2D, texId);

    createBoreders();

    glBindTexture(GL_TEXTURE_2D, texId);
    glBegin(GL_QUADS);
    glNormal3d(0, 0, 1);

    glTexCoord2d(10, 10);
    glTexCoord2d(10, 0);
    glTexCoord2d(0, 10);
    glTexCoord2d(0, 0);
    glEnd();

    //рисуем источник света
    light.DrawLightGizmo();

    //================Сообщение в верхнем левом углу=======================

    //переключаемся на матрицу проекции
    glMatrixMode(GL_PROJECTION);
    //сохраняем текущую матрицу проекции с перспективным преобразованием
    glPushMatrix();
    //загружаем единичную матрицу в матрицу проекции
    glLoadIdentity();

    //устанавливаем матрицу паралельной проекции
    glOrtho(0, gl.getWidth() - 1, 0, gl.getHeight() - 1, 0, 1);

    //переключаемся на моделвью матрицу
    glMatrixMode(GL_MODELVIEW);
    //сохраняем матрицу
    glPushMatrix();
    //сбразываем все трансформации и настройки камеры загрузкой единичной матрицы
    glLoadIdentity();



    std::wstringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << "T - " << (texturing ? L"[вкл]выкл  " : L" вкл[выкл] ") << L"текстур" << std::endl;
    ss << "L - " << (lightning ? L"[вкл]выкл  " : L" вкл[выкл] ") << L"освещение" << std::endl;
    ss << "A - " << (alpha ? L"[вкл]выкл  " : L" вкл[выкл] ") << L"альфа-наложение" << std::endl;
    ss << L"F - Свет из камеры" << std::endl;
    ss << L"G - двигать свет по горизонтали" << std::endl;
    ss << L"G+ЛКМ двигать свет по вертекали" << std::endl;
    ss << L"Коорд. света: (" << std::setw(7) << light.x() << "," << std::setw(7) << light.y() << "," << std::setw(7) << light.z() << ")" << std::endl;
    ss << L"Коорд. камеры: (" << std::setw(7) << camera.x() << "," << std::setw(7) << camera.y() << "," << std::setw(7) << camera.z() << ")" << std::endl;
    ss << L"Параметры камеры: R=" << std::setw(7) << camera.distance() << ",fi1=" << std::setw(7) << camera.fi1() << ",fi2=" << std::setw(7) << camera.fi2() << std::endl;
    ss << L"delta_time: " << std::setprecision(5) << delta_time << std::endl;


    text.setPosition(10, gl.getHeight() - 10 - 180);
    text.setText(ss.str().c_str());
    text.Draw();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();


}

