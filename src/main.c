/*Массив представляется указателем на вектор указателей на
строки. Количество строк хранится в -1 элементе вектора указателей.
Количество элементов строки хранится в -1 элементе строки. Память
выделяется одним блоком.

Описание последовательности шагов для следующих
действий:
*	создание массива;
*	доступ к элементам;
*	доступ к размерам массива (количество строк и длина произвольной строки);
	сохранение в текстовый файл;
*	сохранение в бинарный файл;
*	создание из данных в текстовом файле;
*	создание из данных в бинарном файле;
*	уничтожение массива.
*/

/*Текстовый файл содержит в первой строке первым
 * числом количество строк, в следующих числах этой строки -- размеры строк,
 * дальнейшие строки содержат непосредственно числа*/
#include <stdint.h>
#include <stdio.h>
#include "RM.h"

// Печать матрицы на экран
void Print(double **arr)
{
	RM_Print(arr, "%.3lf\t", stdout);
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

int main(int argc, const char **args)
{
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
