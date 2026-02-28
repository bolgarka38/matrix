#ifndef MTX_H_INCLUDED
#define MTX_H_INCLUDED

#include <stddef.h>

/* Неполный тип матрицы (инкапсуляция)
 * Полное определение находится в matrix.c */
typedef struct matrix matrix;

/* Создаёт новую матрицу размера w (столбцов) x h (строк)
 * Возвращает указатель на матрицу или NULL при ошибке выделения памяти
 * Выделенная память не инициализируется */
matrix *matrix_alloc(size_t w, size_t h);

/* Создаёт точную копию матрицы m
 * Возвращает новую матрицу или NULL при ошибке
 * Копию необходимо освободить через matrix_free() */
matrix *matrix_copy(const matrix *m);

/* Освобождает память, занятую матрицей
 * Если m == NULL, ничего не делает */
void matrix_free(matrix *m);

/* Возвращает указатель на элемент (i, j) без проверки границ
 * Позволяет изменять элемент */
double *matrix_ptr(matrix *m, size_t i, size_t j);

/* Возвращает константный указатель на элемент (i, j) без проверки границ */
const double *matrix_cptr(const matrix *m, size_t i, size_t j);

/* Устанавливает все элементы матрицы в ноль */
void matrix_set_zero(matrix *m);

/* Превращает матрицу в единичную (предполагается квадратная)
 * Возвращает m (для удобства цепочек вызовов) */
matrix *matrix_set_id(matrix *m);

/* Создаёт матрицу размера w x h, заполненную нулями */
matrix *matrix_alloc_zero(size_t w, size_t h);

/* Создаёт единичную матрицу размера w x w */
matrix *matrix_alloc_id(size_t w);

/* Присваивает матрице m1 содержимое матрицы m2
 * Матрицы должны иметь одинаковые размеры
 * Возвращает m1 в случае успеха, NULL при ошибке */
matrix *matrix_assign(matrix *m1, const matrix *m2);

/* Выводит матрицу в стандартный вывод */
void matrix_print(const matrix *m);

/* Читает матрицу со стандартного ввода
 * Ожидается: ширина высота, затем поэлементно по строкам
 * Возвращает новую матрицу или NULL при ошибке ввода/памяти */
matrix *matrix_scan(void);



/* m1 += m2 */
int matrix_add(matrix *m1, const matrix *m2);

/* m1 -= m2 */
int matrix_sub(matrix *m1, const matrix *m2);

/* Умножение всех элементов матрицы на число d */
void matrix_smul(matrix *m, double d);

/* Деление всех элементов матрицы на число d (d != 0) */
void matrix_sdiv(matrix *m, double d);

/* m = m1 + m2
 * Все матрицы должны иметь одинаковые размеры */
int matrix_add2(matrix *m, const matrix *m1, const matrix *m2);

/* m = m1 - m2 */
int matrix_sub2(matrix *m, const matrix *m1, const matrix *m2);

/* m = m1 * d */
int matrix_smul2(matrix *m, const matrix *m1, double d);

/* m = m1 / d  (d != 0) */
int matrix_sdiv2(matrix *m, const matrix *m1, double d);

/* Умножение матриц: m1 = m1 * m2.
 * Используется временная матрица, m1 может изменить размер */
int matrix_mul(matrix *m1, const matrix *m2);

/* m = m1 * m2.
 * Матрица m может совпадать с m1 или m2 (используется временная матрица) */
int matrix_mul2(matrix *m, const matrix *m1, const matrix *m2);



/* Транспонирование квадратной матрицы на месте */
int matrix_transpose(matrix *m);

/* Перестановка двух строк местами */
int matrix_swap_rows(matrix *m, size_t r1, size_t r2);

/* Перестановка двух столбцов местами */
int matrix_swap_cols(matrix *m, size_t c1, size_t c2);

/* Умножение строки r на число d */
int matrix_row_mul(matrix *m, size_t r, double d);

/* Деление строки r на число d (d != 0) */
int matrix_row_div(matrix *m, size_t r, double d);

/* Сложение строк: r1 = r1 + coeff * r2 */
int matrix_row_add(matrix *m, size_t r1, size_t r2, double coeff);

/* Вычисление бесконечной нормы матрицы:
 * максимум по строкам суммы модулей элементов */
double matrix_norm(const matrix *m);



/* Вычисление матричной экспоненты exp(A) с заданной точностью eps.
 * A должна быть квадратной. Используется ряд Тейлора.
 * Остановка, когда норма очередного члена < eps. */
matrix *matrix_exp(const matrix *A, double eps);

/* Решение системы линейных уравнений A * X = B методом Гаусса
 * с выбором главного элемента по столбцу.
 * A – квадратная невырожденная матрица, B – матрица правых частей. */
matrix *matrix_solve(const matrix *A, const matrix *B);


#endif // MTX_H_INCLUDED
