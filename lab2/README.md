# Основной функционал класса MatrixDense

Конструктор и деструктор:

Конструктор принимает размеры матрицы и создает массив для хранения элементов.
Деструктор освобождает память.
Доступ к элементам:

Перегружены операторы () для доступа и изменения элементов матрицы с проверкой выхода за границы.
Арифметические операции:

+ и -: Сложение и вычитание матриц с одинаковыми размерами.
elementwise_multiply: Поэлементное умножение.
*: Матричное умножение (размеры должны соответствовать правилам линейной алгебры).
Транспонирование:

Метод transpose возвращает транспонированную матрицу.
Работа с файлами:

exportToFile: Сохраняет матрицу в файл.
importFromFile: Загружает матрицу из файла.
Вывод:

print: Печатает матрицу в консоль с форматированием.

# Пример использования (в функции main)

Создаются две матрицы A и B размером 2×3.
Выполняются операции: сложение, вычитание, поэлементное умножение, транспонирование.
Матрица A сохраняется в файл, а затем загружается обратно.
Выполняется матричное умножение матрицы A на другую матрицу G (размер 3×2).
Все результаты выводятся в консоль.
Код также обрабатывает исключения, такие как выход за границы массива или несоответствие размеров матриц для операций.
