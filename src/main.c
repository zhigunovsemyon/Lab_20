/*Массив представляется указателем на вектор указателей на
строки. Количество строк хранится в -1 элементе вектора указателей.
Количество элементов строки хранится в -1 элементе строки. Память
выделяется одним блоком.

Описание последовательности шагов для следующих
действий:
*	создание массива;
*	доступ к элементам;
*	доступ к размерам массива (количество строк и длина произвольной строки);
*	сохранение в текстовый файл;
*	сохранение в бинарный файл;
*	создание из данных в текстовом файле;
*	создание из данных в бинарном файле;
*	уничтожение массива.
*/

/*Текстовый файл содержит в первой строке первым
 * числом количество строк, в следующих числах этой строки -- размеры строк,
 * дальнейшие строки содержат непосредственно числа*/
#include "RM.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void FillMatrixManualy(double **arr);
void NullMatrix(double **arr);
void FillMatrixManualy(double **arr);
double Random(double x, double y);
double **ManualCreate(void);
double **ReadTextFile(const char *fname);
uint8_t WriteTextFile(const char *fname, double **arr);
uint8_t WriteBinary(const char *fname, double **arr);
double **ReadBinary(const char *fname);
void Print(double **arr);

int main(int argc, const char **args)
{
	srand(time(NULL));

	if (argc != 2)
	{
		puts("Укажите имя файла!");
		return NOERR;
	}
	double **m = ReadTextFile(args[1]);
	if (!m)
	{
		puts("Получен NULL!");
		return FILEERR;
	}

	puts("Текст файл");
	Print(m);

	FILE *wrbin = fopen("files/bin", "wb");
	if (RM_WriteBinary(m, wrbin))
		return FILEERR;

	fclose(wrbin);
	RM_Free(m);

	puts("Бин файл");
	FILE *rbin = fopen("files/bin", "rb");
	m = RM_ReadBinFile(rbin);
	if (!m)
		return FILEERR;
	Print(m);

	fclose(rbin);

	FILE *wtext = fopen("files/fname", "wt");
	if (RM_WriteTxtFile(m, wtext))
		return FILEERR;

	fclose(wtext);

	RM_Free(m);
	return NOERR;
}

// Печать матрицы на экран
void Print(double **arr)
{
	RM_Print(arr, "%.3lf\t", stdout);
}

/*Чтение матрицы из бинарного файла с названием fname
 * при неполадках возвращает NULL */
double **ReadBinary(const char *fname)
{
	// Открытие нужного файла
	FILE *f = fopen(fname, "rb");

	// Попытка чтения этого файла, при неудаче возвращается NULL
	double **ret = RM_ReadBinFile(f);

	if (f)		   // Если файл удалось открыть
		fclose(f); // то осуществляется его закрытие
	return ret;	   // Возврат матрицы, или NULL
}

/*Запись матрицы в бинарный файл с названием fname
 * при неполадках возвращает NULL */
uint8_t WriteBinary(const char *fname, double **arr)
{
	// Открытие нужного файла
	FILE *f = fopen(fname, "wb");

	// Запись в открытый файл, сохранение возвращённого кода ошибки
	uint8_t ret = RM_WriteBinary(arr, f);

	if (!ret)	   // Если файл удалось открыть
		fclose(f); // то осуществляется его закрытие
	return ret;	   // Возврат кода ошибки
}

/*Запись матрицы в текстовый файл с названием fname
 * при неполадках возвращает NULL */
uint8_t WriteTextFile(const char *fname, double **arr)
{
	// Открытие нужного файла
	FILE *f = fopen(fname, "wt");

	// Запись в открытый файл, сохранение возвращённого кода ошибки
	uint8_t ret = RM_WriteTxtFile(arr, f);

	if (!ret)	   // Если файл удалось открыть
		fclose(f); // то осуществляется его закрытие
	return ret;	   // Возврат кода ошибки
}

/*Чтение матрицы из текстового файла с названием fname
 * при неполадках возвращает NULL */
double **ReadTextFile(const char *fname)
{
	// Открытие нужного файла
	FILE *f = fopen(fname, "rt");

	// Попытка чтения этого файла, при неудаче возвращается NULL
	double **ret = RM_ReadTxtFile(f);

	if (f)		   // Если файл удалось открыть
		fclose(f); // то осуществляется его закрытие
	return ret;	   // Возврат матрицы, или NULL
}

/*Ручной ввод создание матрицы пользователем*/
double **ManualCreate(void)
{
	uint16_t rows, *cols;
	printf("Укажите число столбцов в матрице: ");
	scanf("%hu", &rows);

	//Создание массива размеров
	cols = (uint16_t*)malloc(sizeof(uint16_t) * rows);
	if (!cols)
		return NULL;

	//Наполнение массива размеров
	for(uint16_t i = 0; i < rows; ++i)
	{
		printf("Введите размер %hu-ой строки", i);
		scanf("%hu", cols + i);
	}

	//Создание матрицы из данных размеров, будет возвращён NULL при неудаче
	double **m = RM_CreateArray(rows, cols);
	free(cols);
	return m;
}

//Возврат случайного числа из диапазона [a, b]. Требует предварительной инициализации посредством srand()
double Random(double x, double y)
{
	//Требует доработки для генерации нецелых чисел
	int a = (int)x;
	int b = (int)y;
	return (double)(rand() % (b - a + 1) + a);
}

//Заполнение матрицы arr случайными числами
void FillMatrixRandom(double **arr, double a, double b)
{
	if (a > b)	//"Переворот" чисел
	{
		double tmp = b;
		b = a;
		a = tmp;
	}

	for (uint16_t j, n, i = 0, m = RM_GetLineCount(arr); i < m; ++i)
		for (j = 0, n = RM_GetElCount(arr[i]); j < n; ++j)
			arr[i][j] = Random(a, b);
}

//Зануление матрицы arr
void NullMatrix(double **arr)
{
	for (uint16_t j, n, i = 0, m = RM_GetLineCount(arr); i < m; ++i)
		for (j = 0, n = RM_GetElCount(arr[i]); j < n; ++j)
			arr[i][j] = 0;
}

//Ручное наполнение матрицы arr
void FillMatrixManualy(double **arr)
{
	for (uint16_t j, n, i = 0, m = RM_GetLineCount(arr); i < m; ++i)
		for (j = 0, n = RM_GetElCount(arr[i]); j < n; ++j)
		{
			printf("Элемент [%hu][%hu] = ", i, j);
			scanf("%lf", arr[i] + j);
		}
}

