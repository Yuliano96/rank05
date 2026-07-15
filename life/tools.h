/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 21:32:33 by yuliano           #+#    #+#             */
/*   Updated: 2026/07/11 19:37:26 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOL_H
#define TOOL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 

#define MAX_SIZE 100

typedef struct s_point
{
	int x;
	int y;
	int flag;
	int width;
	int height;
}	t_point;

t_point *init_pen(int widt, int height);
int read_commands(char *buffer, size_t size);
void delete_table(char ***matriz, int index);
char **create_table(int row, int column);
void init_table(char **matriz, int row, int column);
void print_table(char **matriz, int row, int column);
void move_position(t_point *p, int x, int y);
void draw_table(char **matriz, char *buffer, size_t size, t_point *p);
char **copy_table(char **matriz, int row, int column);
void modify_table(char **matriz, int y, int x, int count);
int show(int *i, int *j, int x, int y, char** matriz, int row, int column);
int count_neighbors(char **matriz, int row, int column, int *j, int *i);
int is_alive(int y, int x, char **matriz);
void delete_pos(t_point **p);

#endif