/*
Program to solve Sudoku
Author: Alexander Ivanov
Date: 14.01.2016
Example input.txt:
    0 0 5 3 0 0 0 0 0
    8 0 0 0 0 0 0 2 0
    0 7 0 0 1 0 5 0 0
    4 0 0 0 0 5 3 0 0
    0 1 0 0 7 0 0 0 6
    0 0 3 2 0 0 0 8 0
    0 6 0 5 0 0 0 0 9
    0 0 4 0 0 0 0 3 0
    0 0 0 0 0 9 7 0 0
N is field size
K is small square size (K|N)
Please, don't change DEBUG and USEALLOCATOR
*/
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#define N 9
#define K 3
#define DEBUG 0
#define USEALLOCATOR 1
struct Cell
{
    size_t i;
    size_t j;
    int Can[N + 1];
    int Var;
};
struct MemoryAllocator
{
    int Size;
    int SizeStep;
    int FreeCount;
    int* CanBeUsed;
    int*** FreeMemory;
};
inline struct MemoryAllocator* CreateMemoryAllocator(int AllocatorStep)
{
    size_t i, j;
    struct MemoryAllocator* NewAllocator = (struct MemoryAllocator*)malloc(sizeof(struct MemoryAllocator));
    NewAllocator->Size = AllocatorStep;
    NewAllocator->SizeStep = AllocatorStep;
    NewAllocator->FreeCount = AllocatorStep;
    NewAllocator->CanBeUsed = (int*)malloc(sizeof(int) * AllocatorStep);
    for(i = 0; i < AllocatorStep; i++)
        NewAllocator->CanBeUsed[i] = 1;
    NewAllocator->FreeMemory = (int***)malloc(sizeof(int**) * AllocatorStep);
    for(i = 0; i < AllocatorStep; i++)
    {
        NewAllocator->FreeMemory[i] = (int**)calloc(N, sizeof(int*));
        for(j = 0; j < N; j++)
            NewAllocator->FreeMemory[i][j] = (int*)calloc(N, sizeof(int));
    }
    return NewAllocator;
};
inline int** GetMemory(struct MemoryAllocator* Allocator)
{
    if(DEBUG)
    {
        size_t iter;
        printf("\nGetMemory start\n");
        printf("Allocator params: \n  Size=%d  SizeStep=%d  FreeCount=%d\n", Allocator->Size, Allocator->SizeStep, Allocator->FreeCount);
        printf("  CanBeUsed & FreeMemory:\n      ");
        for(iter = 0; iter < Allocator->Size; iter++)
        {
            printf("x%03d: %d :x%09lld   ", iter, Allocator->CanBeUsed[iter], (long long int)Allocator->FreeMemory[iter]);
            if(iter % Allocator->SizeStep == Allocator->SizeStep - 1)
                printf("\n      ");
        }
    }
    if(Allocator->FreeCount > 0)
    {
        size_t iter;
        for(iter = 0; iter < Allocator->Size; iter++)
            if(Allocator->CanBeUsed[iter])
            {
                Allocator->CanBeUsed[iter] = 0;
                Allocator->FreeCount--;
                if(DEBUG) printf("GetMemory end without system memory allocation\n");
                return Allocator->FreeMemory[iter];
            }
    }
    else
    {
        size_t iter, j;
        Allocator->FreeCount += Allocator->SizeStep - 1;
        Allocator->CanBeUsed = (int*)realloc(Allocator->CanBeUsed, 
	sizeof(int) * (Allocator->Size + Allocator->SizeStep));
        Allocator->FreeMemory = (int***)realloc(Allocator->FreeMemory, 
	sizeof(int**) * (Allocator->Size + Allocator->SizeStep));
        for(iter = Allocator->Size; iter < Allocator->Size + Allocator->SizeStep; iter++)
        {
            Allocator->FreeMemory[iter] = (int**)calloc(N, sizeof(int*));
            for(j = 0; j < N; j++)
                Allocator->FreeMemory[iter][j] = (int*)calloc(N, sizeof(int));
            Allocator->CanBeUsed[iter] = 1;
        }
        Allocator->Size += Allocator->SizeStep;

        if(DEBUG) printf("GetMemory end with system memory allocation\n");
        Allocator->CanBeUsed[Allocator->Size - Allocator->SizeStep] = 0;
        return Allocator->FreeMemory[Allocator->Size - Allocator->SizeStep];
    }
    if(DEBUG) printf("GetMemory end with segmentation fault\n");
    return NULL;
}
inline int FreeMemory(struct MemoryAllocator* Allocator, int** Memory)
{
    if(DEBUG)
    {
        size_t iter;
        printf("\nFreeMemory start\n");
        printf("  Memory=x%09lld\n", (long long int)Memory);
        printf("  Allocator params: \n  Size=%d  SizeStep=%d  FreeCount=%d\n", Allocator->Size, Allocator->SizeStep, Allocator->FreeCount);
        printf("    CanBeUsed & FreeMemory:\n      ");
        for(iter = 0; iter < Allocator->Size; iter++)
        {
            printf("x%03d: %d :x%09lld   ", iter, Allocator->CanBeUsed[iter], (long long int)Allocator->FreeMemory[iter]);
            if(iter % Allocator->SizeStep == Allocator->SizeStep - 1)
                printf("\n      ");
        }
        printf("\n");
    }

    size_t iter, j;
    for(iter = 0; iter < Allocator->Size; iter++)
        if(Allocator->FreeMemory[iter] == Memory)
        {
            if(DEBUG)
            {
                printf("  Found: %d\n", iter);
                printf("  CanBeUsed[%d]=%d\n", iter, Allocator->CanBeUsed[iter]);
                if( Allocator->CanBeUsed[iter] == 1)
                    exit(0);
            }
            for(j = 0; j < N; j++)
                memset(Memory[j], 0, N);

            Allocator->CanBeUsed[iter] = 1;
            Allocator->FreeCount++;
            return 1;
        }
    return 0;
}
inline void DestroyMemoryAllocator(struct MemoryAllocator* Allocator)
{
    size_t iter, j;
    free(Allocator->CanBeUsed);
    for(iter = 0; iter < Allocator->Size; iter++)
    {
        for(j = 0; j < N; j++)
            free(Allocator->FreeMemory[iter][j]);
        free(Allocator->FreeMemory[iter]);
    }
    free(Allocator);
}
inline int** CreateArray(struct MemoryAllocator* Allocator)
{
    if(USEALLOCATOR == 0)
    {
        size_t i;
        int** NewArray = (int**)calloc(N, sizeof(int*));
        for(i = 0; i < N; i++)
            NewArray[i] = (int*)calloc(N, sizeof(int));
        return NewArray;
    }
    else
        return GetMemory(Allocator);
    return NULL;
}
inline int** CopyArray(int** From, int** To)
{
    size_t i;
    for(i = 0; i < N; i++)
        memcpy((void*)To[i], (void*)From[i], N * sizeof(int));
}
inline void DestroyArray(struct MemoryAllocator* Allocator, int** Array)
{
    if(USEALLOCATOR == 0)
    {
        size_t i;
        for(i = 0; i < N; i++)
            free(Array[i]);
        free(Array);
    }
    else
        FreeMemory(Allocator, Array);
}
inline int FScanfArray(int** Array, const char* File)
{
    FILE* Input = fopen(File, "r");
    if(Input == NULL)
        return 0;
    size_t i, j;
    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++)
            if(fscanf(Input, "%d", &(Array[i][j])) <= 0)
                return 0;
    close(Input);
    return 1;
}
inline void PrintfArray(int** Array)
{
    size_t i, j;
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            if(Array[i][j] > 0)
                if(N >= 10)
                    printf("%2d ", Array[i][j]);
                else
                    printf("%d ", Array[i][j]);
            else
                if(N >= 10)
                    printf("-- ", Array[i][j]);
                else
                    printf("- ", Array[i][j]);
            if(j % K == K - 1)
                printf("  ");
        }
        printf("\n");
        if(i % K == K - 1)
            printf("\n");
    }
    printf("\n\n");
}
inline struct Cell CalcField(int** Field)
{
    size_t i, j;
    size_t ki, kj, iter;

    struct Cell Result, LeastOptionsCell;
    Result.Var = N + 1;

    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++)
            if(Field[i][j] == 0)
            {
                LeastOptionsCell.i = i;
                LeastOptionsCell.j = j;
                for(iter = 0; iter < N + 1; iter++)
                    LeastOptionsCell.Can[iter] = 1;
                LeastOptionsCell.Var = 9;

                for(kj = 0; kj < N; kj++)
                    LeastOptionsCell.Can[Field[i][kj]] = 0;

                for(ki = 0; ki < N; ki++)
                    LeastOptionsCell.Can[Field[ki][j]] = 0;

                for(ki = i - (i % K); ki < i + K - (i % K); ki++)
                    for(kj = j - (j % K); kj < j + K - (j % K); kj++)
                        LeastOptionsCell.Can[Field[ki][kj]] = 0;

                for(iter = 1; iter < N + 1; iter++)
                    if(!LeastOptionsCell.Can[iter])
                        LeastOptionsCell.Var--;

                if(LeastOptionsCell.Var < Result.Var)
                    Result = LeastOptionsCell;
            }
    return Result;
}
inline int FieldIsCorrect(int** Field)
{
    size_t i, j;
    size_t ki, kj, iter;

    struct Cell LeastOptionsCell;
    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++)
            if(Field[i][j] != 0)
            {
                LeastOptionsCell.i = i;
                LeastOptionsCell.j = j;
                for(iter = 0; iter < N + 1; iter++)
                    LeastOptionsCell.Can[iter] = 1;
                LeastOptionsCell.Var = N + 1;

                for(kj = 0; kj < N; kj++)
                    if(kj != j)
                        LeastOptionsCell.Can[Field[i][kj]] = 0;

                for(ki = 0; ki < N; ki++)
                    if(ki != i)
                        LeastOptionsCell.Can[Field[ki][j]] = 0;

                for(ki = i - (i % K); ki < i + K - (i % K); ki++)
                    for(kj = j - (j % K); kj < j + K - (j % K); kj++)
                        if(ki != i || kj != j)
                            LeastOptionsCell.Can[Field[ki][kj]] = 0;

                if(LeastOptionsCell.Can[Field[i][j]] == 0)
                {
                    if(DEBUG)
                        printf("Data isn't correct in (%d, %d)\n", i + 1, j + 1 );
                    return 0;
                }
            }
    return 1;
}
inline int ArrayIsFull(int** Array)
{
    size_t i, j;
    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++)
            if(Array[i][j] == 0)
                return 0;
    return 1;
}
int RecursionStep(int** Field, struct MemoryAllocator* Allocator)
{
    if(ArrayIsFull(Field))
        return 1;

    struct Cell Next = CalcField(Field);
    if(Next.Var == 0)
        return 0;

    int** Backup = CreateArray(Allocator);
    CopyArray(Field, Backup);

    size_t k;
    for(k = 1; k < N + 1; k++)
        if(Next.Can[k])
        {
            Field[Next.i][Next.j] = k;
            if(RecursionStep(Field, Allocator))
            {
                DestroyArray(Allocator, Backup);
                return 1;
            }
            CopyArray(Backup, Field);
        }
    DestroyArray(Allocator, Backup);
    return 0;
}
int main(void)
{
    system("COLOR 0E");
    setlocale(LC_ALL, "rus");
    struct MemoryAllocator* Allocator = CreateMemoryAllocator(10);
    int** Field = CreateArray(Allocator);
    double WorkTime = 0;

    if(!FScanfArray(Field, "input.txt"))
    {
        printf("Can't open file or read data.\n");
        return 0;
    }
    PrintfArray(Field);
    if(!FieldIsCorrect(Field))
    {
        printf("Data isn't correct!");
        return 0;
    }
    WorkTime = clock();
    if(RecursionStep(Field, Allocator))
    {
        printf("Completed\n");
        PrintfArray(Field);
    }
    else
    {
       printf("No solution\n");
    }

    WorkTime = clock() - WorkTime;
    printf("WorkTime = %lg\n", WorkTime / CLOCKS_PER_SEC);

    DestroyArray(Allocator, Field);
    DestroyMemoryAllocator(Allocator);

    system("Pause");
    return 0;
}
