# Spreadsheet
Проект представляет собой реализацию электронной таблицы на C++ с поддержкой формул, ссылок на ячейки, отслеживания зависимостей и автоматической инвалидации кэша.
Логика работы аналогична Excel или Google Sheets.

## Требования для сборки

### Обязательное ПО
- Компилятор C++ с поддержкой C++17
  - GCC 7+, Clang 5+, MSVC 2017 15.7+
- CMake версии 3.8 или выше
- Java Runtime Environment (JRE) – для запуска ANTLR-генератора
- ANTLR 4.13.2
  - Инструмент ANTLR (JAR-файл)
  - Исходные коды ANTLR C++ runtime

## Получение зависимостей

### Если зависимости уже включены в проект
Этот шаг можно пропустить, если в клонируемом репозитории есть в наличии:
- `antlr-4.13.2-complete.jar` в корневой папке
- папка `antlr4_runtime/` с корректно распакованными исходниками ANTLR C++ runtime

### Если зависимости нужно скачать вручную
1. **ANTLR tool**
Скачайте `antlr-4.13.2-complete.jar`:
```
wget https://www.antlr.org/download/antlr-4.13.2-complete.jar
```
Инструкцию по установке ANTLR можно найти на сайте [antlr.org](https://www.antlr.org/).

После загрузки переместите файл `antlr-4.13.2-complete.jar` в корень проекта.

2. **ANTLR C++ runtime**
Скачайте исходные коды:

https://www.antlr.org/download/antlr4-cpp-runtime-4.13.2-source.zip

Распакуйте содержимое в папку `antlr4_runtime/` внутри проекта.

После распаковки структура должна выглядеть так:

```
spreadsheet/
├── CMakeLists.txt
├── FindANTLR.cmake
├── Formula.g4
├── antlr-4.13.2-complete.jar
├── antlr4_runtime/
│   ├── CMakeLists.txt
│   └── runtime/
│       └── src/ ...
└── ...
```

**Важно:** В `CMakeLists.txt` ожидается, что ANTLR runtime находится именно в поддиректории `antlr4_runtime/`, а JAR-файл — в корне.
При изменении путей потребуется адаптировать скрипт.

## Сборка проекта
Сборка выполняется через CMake. Рекомендуется использовать отдельную папку для сборки.

### 1. Конфигурация
Из корня проекта выполните:
```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release   # или Debug
```
**Для Windows (MSVC):**

При необходимости укажите генератор, например:
```
cmake .. -G "Visual Studio 17 2022" -A x64
```
Если **ANTLR JAR**-файл находится не в корне, укажите путь явно:
```
cmake .. -DANTLR_EXECUTABLE=/путь/к/antlr-4.13.2-complete.jar
```
### 2. Компиляция
```
cmake --build .
```
После успешной сборки в папке `build` появится исполняемый файл:
- **Linux / macOS:** `spreadsheet`
- **Windows:** `spreadsheet.exe`
## Запуск
В текущей реализации `main.cpp` содержит **только модульные тесты**. Запустите программу:
```
./spreadsheet   # или spreadsheet.exe
```
При успешном прохождении всех тестов вы увидите:
```
TestEmpty OK
TestInvalidPosition OK
TestSetCellPlainText OK
TestClearCell OK
TestPrint OK
```
Все тесты должны завершаться без ошибок.

### 🛠 Стек технологий и навыки

| Категория | Технологии и навыки |
| :--- | :--- |
| **Languages** | ![C++](https://img.shields.io/badge/C++%2017/20-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white) ![SQL](https://img.shields.io/badge/SQL-4479A1?style=for-the-badge&logo=sqlite&logoColor=white) |
| **Libraries** | ![Boost](https://img.shields.io/badge/Boost-00599C?style=for-the-badge&logo=boost&logoColor=white) ![STL](https://img.shields.io/badge/STL-C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![ANTLR4](https://img.shields.io/badge/ANTLR4-C22D2D?style=for-the-badge) |
| **Tools** | ![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white) ![Docker](https://img.shields.io/badge/Docker-2496ED?style=for-the-badge&logo=docker&logoColor=white) ![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black) ![Git](https://img.shields.io/badge/Git-F05032?style=for-the-badge&logo=git&logoColor=white) ![Conan](https://img.shields.io/badge/Conan-6699CB?style=for-the-badge&logo=conan&logoColor=white) |
| **DBMS** | ![PostgreSQL](https://img.shields.io/badge/PostgreSQL-4169E1?style=for-the-badge&logo=postgresql&logoColor=white) |
| **CS Skills** | ![Algorithms](https://img.shields.io/badge/Algorithms-Trees%20&%20Graphs-D35400?style=for-the-badge) ![Multithreading](https://img.shields.io/badge/Multithreading-Parallel-4B0082?style=for-the-badge) |
