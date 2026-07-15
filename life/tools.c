/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 11:47:48 by yuliano           #+#    #+#             */
/*   Updated: 2026/07/11 19:36:51 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools.h"


int read_commands(char *buffer, size_t size)
{
	int bytes_read;
	bytes_read = read(0, buffer, size);
	if (bytes_read < 0)
		return (-1);
	buffer[bytes_read] = '\0';
	return bytes_read;
}

t_point *init_pen(int width, int height)
{
	t_point *p = (t_point *)malloc(sizeof(t_point));
	if(p == NULL)
		return NULL;
	p->flag = 0;
	p->x = 0;
	p->y = 0;
	p->width = width;
	p->height = height;
	return p;
}


void delete_table(char ***matriz, int index)
{
	if(*matriz)
	{
		for (int i = 0; i < index; i++)
		{
			free((*matriz)[i]);
		}
		free(*matriz);
	}
	*matriz = NULL;
}

char **create_table(int row, int column)
{
	char **matriz;
	
	if(row < 0 || column < 0)
		return NULL;
	
	matriz =(char **)malloc(row*sizeof(char *));
	if (!matriz)
		return NULL;
	for (int i = 0; i < row; i++)
	{
		matriz[i] = (char *)malloc(column * sizeof(char));
		if (!matriz)
		{
			delete_table(&matriz, (i - 1));
			return NULL;
		}
	}

	return matriz;
}

void init_table(char **matriz, int row, int column)
{
	for (int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{
			matriz[i][j] = ' ';
		}
	}
}

char **copy_table(char **matriz, int row, int column)
{
	char **matriz_copy;

	matriz_copy = create_table(row, column);
	if (!matriz_copy)
		return NULL;
	for(int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; i++)
		{
			matriz_copy[i][j] = matriz[i][j];
		}
	}
	return matriz_copy;
}

void move_position(t_point *p, int x, int y)
{
	p->x = p->x + x;
	p->y = p->y + y;
}

void draw_table(char **matriz, char *buffer, size_t size, t_point *p)
{
	for(int i = 0; i < size; i++)
	{
		if(buffer[i] == 'x')
		{
			if (p->flag == 0)
			{
				p->flag = 1;
				matriz[p->y][p->x] = 'o';	
			}
			else
				p->flag = 0;
		}
		else if(buffer[i] == 'w' && p->y < p->height)
		{
			move_position(p, 0, -1);
			if (p->flag == 1)
				matriz[p->y][p->x] = 'o';
		}
		else if(buffer[i] == 's' && p->y < p->height)
		{
			move_position(p, 0,1);
			if (p->flag == 1)
			{
				matriz[p->y][p->x] = 'o';
			}
		}
		else if(buffer[i] == 'a' && p->x < p->width)
		{
			move_position(p, -1, 0);
			if (p->flag == 1)
			{
				matriz[p->y][p->x] = 'o';
			}
			
		}
		else if(buffer[i] == 'd' && p->x < p->width)
		{
			move_position(p, 1, 0);
			if (p->flag == 1)
			{
				matriz[p->y][p->x] = 'o';
			}
		}
	}
}

int is_alive(int x, int y, char **matriz)
{
	if (matriz[y][x] == 'o')
		return (1);
	else
		return (0);
}


int show(int *i, int *j, int x, int y, char** matriz, int row, int column)
{
	if ((*j + y) >= row || (*j + y) < 0 ||(*i + x) >= column ||(*i + x) < 0)
		return 0;
	if (matriz[*j + y][*i + x] == 'o')
		return 1;
	else
		return 0;
}


void modify_table(char **matriz, int y, int x, int count)
{
	if(is_alive(y, x, matriz))
	{
		if (count < 2 || count > 3)
			matriz[y][x] = ' ';
	}
	else
	{
		if(count == 3)
			matriz[y][x] = 'o';
	}
}


int count_neighbors(char **matriz, int row, int column, int *j, int *i)
{
	int count = 0;
	while (*i < column)
		{
			if (show(i, j, 0, -1, matriz, row, column))
				count ++;
			if (show(i, j, 0, 1, matriz, row, column))
				count ++;
			if (show(i, j, 1, 0, matriz, row, column))
				count++;
			if (show(i, j, -1, 0, matriz, row, column))
				count++;
			// arriba - derecha
			if (show(i, j, 1, -1, matriz, row, column))
				count++;
			//arriba - izq
			if (show(i, j, -1, -1, matriz, row, column))
				count++;
			//abajo - derecha
			if (show(i, j, 1, 1, matriz, row, column))
				count++;
			//abajo - izq
			if (show(i, j , -1 , 1, matriz, row, column))
				count++;
			(*i)++;
			if (*i ==  column)
			{
				*i = 0;
				(*j)++;
			}
				
			return count;
		}
	return 0;
}



void print_table(char **matriz, int row, int column)
{
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
		{
			putchar(matriz[i][j]);
			putchar(' ');
		}
		putchar('\n');
	}
}

void delete_pos(t_point **p)
{
	if (*p)
	{
		free(*p);
	}

	*p = NULL; 
}