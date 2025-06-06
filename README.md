# Курсовая работа по компьютерной графике

## 📌 Описание
Данный проект выполнен в рамках курсовой работы по дисциплине «Компьютерная графика». Основная задача — продемонстрировать реализацию освещения и текстурирования объектов в 3D-пространстве с использованием OpenGL.

Фигура визуализируется с использованием нескольких текстур, освещения и альфа-прозрачности. Реализована реакция на пользовательские действия: смена режимов визуализации и перемещение источника света.

---

## 📂 Структура проекта

```pqsql
coursework/
├── courcework_source/
│   ├── KGlab.sln                  # Visual Studio solution
│   ├── KGlab.vcxproj              # VS project file
│   ├── KGlab.vcxproj.filters      # Visual Studio filters
│   ├── src/                       # Исходники (.cpp)
│   │   ├── main.cpp
│   │   ├── Camera.cpp
│   │   ├── GUItextRectangle.cpp
│   │   ├── Light.cpp
│   │   ├── MyOGL.cpp
│   │   ├── Render.cpp
│   ├── include/                   # Заголовки (.h)
│   │   ├── Camera.h
│   │   ├── Event.h
│   │   ├── GUItextRectangle.h
│   │   ├── Light.h
│   │   ├── MyOGL.h
│   │   ├── Render.h
│   │   ├── Vector3.h
│   │   ├── stb_image.h
│   ├── resources/                 # Ресурсы (текстуры и т.п.)
│   │   ├── texture.png
│   │   ├── texture_alpha.png
├── images/
│   ├── screenshot_1.png
│   ├── screenshot_2.png
│   ├── screenshot_3.png
│   ├── screenshot_4.png
├── .gitignore
├── README.md
```

---

## ⚙️ Сборка и запуск

1. Клонировать репозиторий:

   ```bash
   git clone https://github.com/your-username/courcework.git

   ```

2. Убедитесь, что у вас установлено:
   ```bash
   Visual Studio с поддержкой C++ и OpenGL
   ```

3. Откройте файл:
   ```bash
   KGlab.sln
   ```

4. Постройте проект и запустите его.

---

## 🕹 Управление

| Кнопка         | Действие                                     |
|----------------|----------------------------------------------|
| `T`            | Включение/выключение текстур                 |
| `L`            | Включение/выключение освещения               |
| `A`            | Включение/выключение альфа-прозрачности      |
| `F`            | Переключение освещения от камеры             |
| `G`            | Горизонтальное перемещение источника света   |
| `Shift + ЛКМ`  | Вертикальное перемещение источника света     |

Подробнее см. скриншоты ниже (см. рис. 1 и далее).

---

## 📸 Скриншоты


1. <a href="#fig1">Визуализация объекта с освещением</a> 

2. <a href="#fig2">Визуализация объекта с отключённым освещением</a> 

3. <a href="#fig3">Альфа-прозрачность</a> 

4. <a href="#fig4">Перемещение источника света (рис. 4.1)</a>

5. <a href="#fig5">Перемещение источника света (рис. 4.2)</a>

---

<a id="fig1"></a>
<p align="center">
  <img src="/images/screenshot_1.png" alt="Рис. 1" width="600">
<h3 align="center">Рис. 1</h3>
</p>

---

<a id="fig2"></a>
<p align="center">
  <img src="/images/screenshot_2.png" alt="Рис. 2" width="600">
<h3 align="center">Рис. 2</h3>
</p>

---

<a id="fig3"></a>
<p align="center">
  <img src="/images/screenshot_3.png" alt="Рис. 3" width="600">
<h3 align="center">Рис. 3</h3>
</p>

---

<a id="fig4"></a>
<p align="center">
  <img src="/images/screenshot_4.png" alt="Рис. 4" width="600">
<h3 align="center">Рис. 4</h3>
</p>

---

<a id="fig5"></a>
<p align="center">
  <img src="/images/screenshot_5.png" alt="Рис. 5" width="600">
<h3 align="center">Рис. 5</h3>
</p>
