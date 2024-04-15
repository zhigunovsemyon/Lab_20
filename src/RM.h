#ifndef RM_H_
#define RM_H_
#include <stdint.h>
#include <malloc.h>

enum Errors
{
	NOERR,
	FILEERR
};


// // Получение адреса начала сегмента (строки или всего массива)
// void *int_GetMinusOne(void *p);
//
// /*Функция считает сумму элементов данного массива arr из элементов Amount */
// uint16_t int_SumOfArray(uint16_t *arr, uint16_t Amount);

// Освобождение памаяти массива
void RM_Free(double **arr);

// Извлечение числа элементов строки
uint16_t RM_GetElCount(double *line);

// Извлечение числа строк из массива
uint16_t RM_GetLineCount(double **arr);

// Печать матрицы m формата spec в сток recepient
void RM_Print(double **m, const char *spec, FILE *recepient);

// Выделение памяти под массив. Возвращает NULL при неудаче
double **RM_CreateArray(uint16_t lines, uint16_t *cols);

/*Функция осуществляет чтение из текстового источника f
 * Возвращает NULL в случае ошибки, либо указатель на первый элемент массива*/
double **RM_ReadTxtFile(FILE *f);

// Запись матрицы arr в файл f. Возвращает 0 при штатной работе, либо код ошибки
uint8_t RM_WriteBinary(double **arr, FILE *f);

/*Чтение матрицы из бинарного источника f
 * возвращает NULL при ошибке*/
double **RM_ReadBinFile(FILE *f);

// Сохранение матрицы arr в текстовый сток f. Возвращает 0 при штатной работе, либо код ошибки
uint8_t RM_WriteTxtFile(double **arr, FILE *f);

#endif 
