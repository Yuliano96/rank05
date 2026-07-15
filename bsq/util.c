#include "util.h"

t_matriz *init_matriz(int rows, int columns)
{
	t_matriz *m = (t_matriz *)malloc(sizeof(t_matriz));
	if (!m)
		return NULL;
	m ->rows = rows;
	m ->columns = columns;
	m ->matriz = NULL;
}

void free_t_matriz(t_matriz **m)
{
	if(m == NULL || *m == NULL)
		return ;
	free(*m);
	*m = NULL;
}

t_square *init_squar()
{
	t_square *c = (t_square *)malloc(sizeof(t_square));
	if (!c)
		return NULL;
	c -> area = 0;
	c ->x = 0;
	c ->y = 0;
}

void free_matrizChar(char ***matriz, int index)
{
	if(matriz == NULL || *matriz == NULL)
		return ;
	for(int i = 0; i < index; i++)
	{
		free((*matriz)[i]);
	}
	free(*matriz);
	*matriz = NULL;
}

void free_matrizInt(int ***m, int index)
{
	if (m == NULL || *m == NULL)
		return ;
	for(int i = 0; i < index; i++)
	{
		free((*m)[i]);
	}
	free(*m);
	*m = NULL;
}

int read_file(char *filename, t_matriz *matriz)
{
	char *buffer = NULL;
	size_t len= 0;

	matriz->matriz = (char**)malloc(matriz->rows * sizeof(char *));
	if(matriz ->matriz == NULL)
		return -1;
	
	for(int i = 0; i < matriz->rows; i++)
	{
		matriz->matriz[i] = (char *)malloc(matriz->columns * sizeof(char));
		if (matriz->matriz[i] == NULL)
		{
			free_matrizChar(&matriz->matriz, i);
			return (-1);
		}
	}


	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		free_matrizChar(&matriz->matriz, matriz->rows);
		fclose(fp);
		return (-1);
	}
	getline(&buffer, &len, fp);
	for (int i = 0; i < matriz->rows; i++)
	{
		if (getline(&buffer, &len, fp) == -1)
		{
			free(buffer);
			free_matrizChar(&matriz->matriz, matriz->rows);
			fclose(fp);
			return (-1);
		}
		for(int j = 0; j < matriz->columns; j++)
		{
			matriz->matriz[i][j] = buffer[j];
		}
	}
}

void print(t_matriz *matriz)
{
	for(int i = 0; i < matriz->rows; i++)
	{
		for(int j = 0; j < matriz->columns; j++)
		{
			fprintf(stdout, "%c", matriz->matriz[i][j]);
		}
		fprintf(stdout, "\n");
	}
}

int neighbor(int x, int y, int rows,int columns, int **m)
{
	
	if (x < 0 || x >= columns || y < 0 || y >= columns)
		return 0;
	else
	{
		if (m[y][x] == -1)
			return 0;
		else if(m[y][x] == 0)
			return 0;
		else
			return (m[y][x]);
	}
}


int min(int a, int b, int c)
{
	if (a <= b && a <= c)
		return a;
	else if (b <= a && b <= c)
		return b;
	else
		return c;
}



int init_square(t_matriz *matriz, char empty, int ***m)
{
	*m = (int **)malloc(sizeof(int *));
	if (!m)
		return -1;
	for(int i = 0; i < matriz->rows; i++)
	{
		(*m)[i] = (int *)malloc((matriz->columns) * sizeof(int));
		if (!(*m)[i])
		{
			free_matrizInt(m, i);
			return -1;
		}
	}

	for(int i = 0; i < matriz->rows; i++)
	{
		for(int j = 0; j < matriz->columns; j++)
		{
			if (matriz->matriz[i][j] == empty)
				(*m)[i][j] = -1;
			else
				(*m)[i][j] = 0;
		}
	}
	return 0;

}

void print_int(int **m, int rows, int columns)
{
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			fprintf(stdout, "%d ", m[i][j]);
		}
		fprintf(stdout, "\n");
	}
}

int large_square(int **m, int rows, int columns)
{
	int arriba, arriba_izq, izq;

	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			if(m[i][j] == 0)
				continue;
			else
			{
				//arriba          x    y
				arriba = neighbor(j, (i -1), rows, columns, m);
				//arriba-izq          x   y
				arriba_izq = neighbor(j -1, i -1, rows, columns, m);
				//izq           x   y
				izq = neighbor(j-1, i, rows, columns, m);
				m[i][j] = min(arriba, arriba_izq, izq) + 1;
			}
		}
	}
}

// 1 1 1 1 1 1 1 
// 1 2 2 2 0 1 2 
// 1 2 3 3 1 1 2 
// 1 2 3 4 2 2 2 
// area= 4 x= 3 y= 3
// xxxx...
// xxxxo..
// xxxx...
// xxxx...

void win_square(int **m, int rows, int columns, t_square *c)
{
	c->area = 0;
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			
			if (m[i][j] > c->area)
			{
				c->area = m[i][j];
				c->x = j;
				c->y = i;
			}
		}
	}
	fprintf(stdout, "area= %d x= %d y= %d\n", c->area,c->x, c->y);
}

void	draw_square(t_matriz *m, t_square *c, char full)
{
	int	y_start;
	int	x_start;

	if (c->area <= 0)
	{
		print(m);
		return;
	}
	y_start = c->y - c->area + 1;
	while (y_start <= c->y)
	{
		x_start = c->x - c->area + 1;
		while (x_start <= c->x)
		{
			m->matriz[y_start][x_start] = full;
			x_start++;
		}
		y_start++;
	}
	print(m);
}
