/*Массив представляется указателем на вектор указателей на
строки. Количество строк хранится в -1 элементе вектора указателей.
Количество элементов строки хранится в -1 элементе строки. Память
выделяется одним блоком.

Описание последовательности шагов для следующих
действий:
	создание массива;
	доступ к элементам;
	доступ к размерам массива (количество строк и длина произвольной строки);
	сохранение в текстовый файл;
	сохранение в бинарный файл;
	создание из данных в текстовом файле;
	создание из данных в бинарном файле;
	уничтожение массива.
*/
/*Текстовый файл содержит в первой строке первым
 * числом количество строк, в следующих числах этой строки -- размеры строк,
 * дальнейшие строки содержат непосредственно числа*/
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>

// Определение типа элементов массива
#define Element long double
#define EL_SPEC "%Lf"

enum Errors
{
	NOERR,
};

/*Функция считает сумму элементов данного массива arr из элементов Amount */
int16_t SumOfArray(int16_t *arr, uint16_t Amount)
{
	if (!arr) // Если массива нет -- возврат 0
		return 0;

	/*Перебор элементов массива */
	int16_t Sum = arr[0];
	for (int16_t i = 1; i < Amount; ++i)
		Sum += arr[i];

	return Sum; // Возврат суммы
}

void PrintRiggedMatrix(Element **m)
{
	for (uint16_t i = 0; i < ((uint16_t*)m)[-1]; i++)
	{
		for (uint16_t j = 0; j < ((uint16_t*)(m[i]))[-1]; j++)
			printf("%Lf\t", m[i][j]);
		putchar('\n');
	}
}

/*Функция осуществляет чтение из текстового файла с названием fname
 * Возвращает NULL в случае ошибки, либо указатель на первый элемент массива*/
Element **ReadTextFile(const char *fname)
{
	FILE *f = fopen(fname, "rt");
	// Если не удалось открыть файл, то возвращается NULL
	if (!f)
		return NULL;

	uint16_t lines;
	// Чтение числа строк, выход с возвратом NULL при неудаче
	if (!fscanf(f, "%hu", &lines))
		return NULL;
	// Выделение памяти под массив величин строк, возврат NULL при неудаче
	uint16_t *cols = (uint16_t *)malloc(sizeof(uint16_t) * lines);
	if (!cols)
		return NULL;

	/*Запись размеров каждой строки в массив размеров*/
	for (uint16_t i = 0; i < lines; ++i)
	{
		// Чтение числа строк, выход с возвратом NULL при неудаче
		if (!fscanf(f, "%hu", cols + i))
			return NULL;
	}

	// Выделение памяти под массив и размеры
	int mem = (sizeof(uint16_t) * (1 + lines) + sizeof(Element) * SumOfArray(cols, lines));
	printf("Allocated memory: %d\n", mem);
	Element **p = (Element**)malloc(mem);
	if (!p)
		return NULL;

	p = (uint16_t *)p + 1; // Сдвиг указателя

	((uint16_t *)p)[-1] = lines;
	for (int i = 0; i < lines; ++i)
	{
		p[i] = p + sizeof(uint16_t *) * lines + i * (sizeof(uint16_t) + sizeof(Element) * cols[i] + sizeof(uint16_t));
		((uint16_t*)p[i])[0] = cols[i];
		p[i] = (uint16_t*)(p[i]) + 1;
		for (uint16_t j = 0; j < cols[i]; j++)
		{
			fscanf(f, EL_SPEC, p[i] + j);
		}
	}

	free(cols);
	fclose(f);
	return p;
}

int main(int argc, const char **args)
{
	if (argc != 2)
	{
		puts("Укажите имя файла!");
		return NOERR;
	}
	Element **m = ReadTextFile(args[1]);
	if (!m)
	{
		puts("Получен NULL!");
		return NOERR;
	}
	PrintRiggedMatrix(m);

	getchar();
	free(((uint16_t*)m)- 1);
	while (getchar() == '\n');
	return NOERR;
}
