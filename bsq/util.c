#include "util.h"

t_matriz	*init_matriz(int rows, int columns)
{
	t_matriz	*m;

	m = (t_matriz *)malloc(sizeof(t_matriz));
	if (m == NULL)
		return (NULL);
	m->rows = rows;
	m->columns = columns;
	m->matriz = NULL;

	/*
	** Antes faltaba devolver el puntero creado.
	** Sin este return, el valor recibido en main era indeterminado.
	*/
	return (m);
}

void	free_t_matriz(t_matriz **m)
{
	if (m == NULL || *m == NULL)
		return ;
	free(*m);
	*m = NULL;
}

void	free_matrizChar(char ***matriz, int index)
{
	int	i;

	if (matriz == NULL || *matriz == NULL)
		return ;
	i = 0;
	while (i < index)
	{
		free((*matriz)[i]);
		i++;
	}
	free(*matriz);
	*matriz = NULL;
}

void	free_matrizInt(int ***m, int index)
{
	int	i;

	if (m == NULL || *m == NULL)
		return ;
	i = 0;
	while (i < index)
	{
		free((*m)[i]);
		i++;
	}
	free(*m);
	*m = NULL;
}

int	read_file(char *filename, t_matriz *matriz)
{
	char	*buffer;
	size_t	capacity;
	FILE	*fp;
	int		i;
	int		j;

	buffer = NULL;
	capacity = 0;
	matriz->matriz = (char **)malloc(
			sizeof(char *) * matriz->rows);
	if (matriz->matriz == NULL)
		return (-1);
	i = 0;
	while (i < matriz->rows)
	{
		matriz->matriz[i] = (char *)malloc(
				sizeof(char) * matriz->columns);
		if (matriz->matriz[i] == NULL)
		{
			/*
			** i representa la cantidad de filas creadas correctamente.
			** Se liberan desde 0 hasta i - 1.
			*/
			free_matrizChar(&matriz->matriz, i);
			return (-1);
		}
		i++;
	}
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		/*
		** Antes se hacía fclose(fp) aunque fp fuera NULL.
		** fclose(NULL) no es válido.
		*/
		free_matrizChar(&matriz->matriz, matriz->rows);
		return (-1);
	}

	/*
	** Saltamos la cabecera. La función de validación ya comprobó
	** que esta primera línea tiene un formato válido.
	*/
	if (getline(&buffer, &capacity, fp) == -1)
	{
		free(buffer);
		fclose(fp);
		free_matrizChar(&matriz->matriz, matriz->rows);
		return (-1);
	}
	i = 0;
	while (i < matriz->rows)
	{
		if (getline(&buffer, &capacity, fp) == -1)
		{
			free(buffer);
			fclose(fp);
			free_matrizChar(&matriz->matriz, matriz->rows);
			return (-1);
		}
		j = 0;
		while (j < matriz->columns)
		{
			matriz->matriz[i][j] = buffer[j];
			j++;
		}
		i++;
	}

	/*
	** Antes la función terminaba sin liberar buffer, sin cerrar fp
	** y sin devolver un resultado.
	*/
	free(buffer);
	fclose(fp);
	return (0);
}

void	print(t_matriz *matriz)
{
	int	i;
	int	j;

	i = 0;
	while (i < matriz->rows)
	{
		j = 0;
		while (j < matriz->columns)
		{
			fprintf(stdout, "%c", matriz->matriz[i][j]);
			j++;
		}
		fprintf(stdout, "\n");
		i++;
	}
}

int	neighbor(int x, int y, int rows, int columns, int **m)
{
	/*
	** Antes comprobabas y >= columns.
	** y es una fila y debe compararse con rows.
	*/
	if (x < 0 || x >= columns || y < 0 || y >= rows)
		return (0);

	/*
	** -1 representa una casilla vacía todavía no procesada.
	** Fuera del mapa, obstáculos y casillas no procesadas
	** aportan 0 para el cálculo del mínimo.
	*/
	if (m[y][x] <= 0)
		return (0);
	return (m[y][x]);
}

int	min(int a, int b, int c)
{
	if (a <= b && a <= c)
		return (a);
	else if (b <= a && b <= c)
		return (b);
	return (c);
}

int	init_square(t_matriz *matriz, char empty, int ***m)
{
	int	i;
	int	j;

	/*
	** Antes reservabas sizeof(int *), espacio para una sola fila.
	**
	** Después escribías (*m)[1], (*m)[2]..., fuera de la reserva.
	** Eso corrompía el heap y provocaba:
	** "double free or corruption" al liberar.
	*/
	*m = (int **)malloc(sizeof(int *) * matriz->rows);
	if (*m == NULL)
		return (-1);
	i = 0;
	while (i < matriz->rows)
	{
		(*m)[i] = (int *)malloc(
				sizeof(int) * matriz->columns);
		if ((*m)[i] == NULL)
		{
			free_matrizInt(m, i);
			return (-1);
		}
		i++;
	}
	i = 0;
	while (i < matriz->rows)
	{
		j = 0;
		while (j < matriz->columns)
		{
			if (matriz->matriz[i][j] == empty)
				(*m)[i][j] = -1;
			else
				(*m)[i][j] = 0;
			j++;
		}
		i++;
	}
	return (0);
}

void	print_int(int **m, int rows, int columns)
{
	int	i;
	int	j;

	i = 0;
	while (i < rows)
	{
		j = 0;
		while (j < columns)
		{
			fprintf(stdout, "%d ", m[i][j]);
			j++;
		}
		fprintf(stdout, "\n");
		i++;
	}
}

void	large_square(int **m, int rows, int columns)
{
	int	arriba;
	int	arriba_izq;
	int	izq;
	int	i;
	int	j;

	i = 0;
	while (i < rows)
	{
		j = 0;
		while (j < columns)
		{
			if (m[i][j] != 0)
			{
				arriba = neighbor(j, i - 1,
						rows, columns, m);
				arriba_izq = neighbor(j - 1, i - 1,
						rows, columns, m);
				izq = neighbor(j - 1, i,
						rows, columns, m);
				m[i][j] = min(arriba, arriba_izq, izq) + 1;
			}
			j++;
		}
		i++;
	}
}

void	win_square(int **m, int rows, int columns, t_square *c)
{
	int	i;
	int	j;

	c->area = 0;
	c->x = 0;
	c->y = 0;
	i = 0;
	while (i < rows)
	{
		j = 0;
		while (j < columns)
		{
			/*
			** Solo actualizamos si el valor es estrictamente mayor.
			** En caso de empate conservamos el primero encontrado:
			** el más arriba y, después, el más a la izquierda.
			*/
			if (m[i][j] > c->area)
			{
				c->area = m[i][j];
				c->x = j;
				c->y = i;
			}
			j++;
		}
		i++;
	}
}

void	draw_square(t_matriz *m, t_square *c, char full)
{
	int	y_start;
	int	x_start;

	if (c->area <= 0)
	{
		print(m);
		return ;
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