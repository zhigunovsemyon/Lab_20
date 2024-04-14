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
	сохранение в бинарный файл;
*	создание из данных в текстовом файле;
	создание из данных в бинарном файле;
*	уничтожение массива.
*/

/*Текстовый файл содержит в первой строке первым
 * числом количество строк, в следующих числах этой строки -- размеры строк,
 * дальнейшие строки содержат непосредственно числа*/
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
// #include <math.h>

enum Errors
{
	NOERR,
	FILEERR
};

void *GetMinusOne(void *p)
{
	return (void *)(((uint16_t *)p) - 1);
}

// Освобождение памаяти массива
void RM_Free(double **arr)
{
	free(GetMinusOne(arr));
}

// Извлечение числа элементов строки
uint16_t RM_GetElCount(double *line)
{
	return *((uint16_t *)GetMinusOne(line));
}

// Извлечение числа строк из массива
uint16_t RM_GetLineCount(double **arr)
{
	return *((uint16_t *)GetMinusOne(arr));
}

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

void RM_Print(double **m, const char *spec)
{
	for (uint16_t i = 0; i < RM_GetLineCount(m); i++)
	{
		for (uint16_t j = 0; j < RM_GetElCount(m[i]); j++)
			printf(spec, m[i][j]);
		putchar('\n');
	}
}

/*Функция осуществляет чтение из текстового файла с названием fname
 * Возвращает NULL в случае ошибки, либо указатель на первый элемент массива*/
double **RM_ReadTxtFile(FILE *f)
{
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
	int mem = sizeof(double) * SumOfArray(cols, lines) +		//Память под непосредственно элементы 
			lines * (sizeof(uint16_t) + sizeof(uint16_t *)) +	//Память под размеры строк и вектор указателей
			sizeof(uint16_t);									//Память под количество строк

	double **p = (double **)malloc(mem);
	if (!p)
		return NULL;

	p = (double **)((uint16_t *)p + 1); // Сдвиг указателя
	((uint16_t *)p)[-1] = lines;		 // Запись количества строк

	for (int i = 0; i < lines; ++i)
	{
		p[i] = (double *)					//Запись в i-й элемент вектора указателей
			((void*)p +						//Начала вектора указателей +
			sizeof(uint16_t *) * lines +	//Пространства под вектор указателей +
			((!i) ? 0 :
			i * (							//Пространства под i записанных размеров строк +
				  sizeof(double) * cols[i - 1] +//Пространства под i записанных строк, размер которых в cols[i] 
				  sizeof(uint16_t))));		//Пространства под следующий размер строки

		((uint16_t *)p[i])[0] = cols[i];
		p[i] = (double *)((uint16_t *)(p[i]) + 1);
		for (uint16_t j = 0; j < cols[i]; j++)
		{
			if(!fscanf(f, "%lf", p[i] + j))
				return NULL;
		}
	}

	free(cols);
	return p;
}

uint8_t RM_WriteBinary(double **arr, FILE *f)
{
	if (!f)
		return FILEERR;

	uint16_t lines = RM_GetLineCount(arr);
	//Запись числа строк
	fwrite(&lines, sizeof(uint16_t), 1, f);

	//Запись с начала области элементов
	fwrite(GetMinusOne(arr[0]), sizeof(uint16_t) + sizeof(double), lines, f);

	return NOERR;
}

double **RM_ReadBinFile(FILE *f)
{
	if (!f)
		return NULL;

	//Запись числа памяти для элементов матрицы и размеров
	fseek(f, 0 , SEEK_END);
	int mem = ftell(f);
	rewind(f);//Перемотка файла в начало
	
	//Чтение числа элементов
	uint16_t lines;
	fread(&lines, sizeof(uint16_t), 1, f);
	mem += lines * sizeof(double *); //Добавление памяти для вектора указателей
	
	//Выделение памяти для массива
	double **p = (double **)malloc(mem);
	if (!p)
		return NULL;

	p = (double **)((uint16_t *)p + 1); // Сдвиг указателя
	((uint16_t *)p)[-1] = lines;		 // Запись количества строк

	//Побайтное чтение файла в массив после оставления места под вектор указателей
	fread(p + lines, 1, mem - 1, f);

	for (uint16_t i = 0; i < lines; ++i)
	{
		p[i] = (double *)				//Запись в i-й элемент вектора указателей
			((void *)p +				//Начала вектора указателей +
			sizeof(uint16_t *) * lines +//Пространства под вектор указателей +
			sizeof(uint16_t) +			//Пространства под размер строки
			( (!i) ? 0 :
			 i * (sizeof(uint16_t) +	//Пространства под i записанных размеров строк +
				  sizeof(double) * RM_GetElCount(p[i - 1]) //Пространства под i записанных строк, размер которых в cols[i] 
				  )));		//Пространства под следующий размер строки
	}
	return p;
}

double **ReadTextFile(const char *fname)
{
	FILE *f = fopen(fname, "rt");

	double **ret = RM_ReadTxtFile(f);

	if (f)
		fclose(f);
	return ret;
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
	RM_Print(m,"%lf\t");

	FILE *wrbin = fopen("files/bin", "wb");
	if(RM_WriteBinary(m, wrbin))
		return FILEERR;
	
	fclose(wrbin);
	RM_Free(m);

	puts("Бин файл");
	FILE *rbin = fopen("files/bin", "rb");
	m = RM_ReadBinFile(rbin);
	if (!m)
		return FILEERR;
	RM_Print(m,"%lf\t");

	fclose(rbin);
	RM_Free(m);
	return NOERR;
}
