/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablondel <ablondel@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 16:39:32 by ablondel          #+#    #+#             */
/*   Updated: 2022/01/10 10:58:12 by ablondel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

typedef struct	s_map
{
	int			w;
	int			h;
	char		c;
	char		map[302][302];
}				t_map;

typedef struct	s_r
{
	char		type;
	float		xtl;
	float		xbr;
	float		ytl;
	float		ybr;
	float		w;
	float		h;
	char		c;
}				t_r;

int    ft_putstr(char *s)
{
    while (*s)
        write(1, s++, 1);
	return (1);
}

void	display(t_map *map)
{
	int i = 0;
	while (i < map->h)
		ft_putstr(map->map[i++]);
}

void	set_map(t_map *map)
{
	int i = -1;
	int j = -1;

	while (++i < map->h)
	{
		while (++j < map->w)
			map->map[i][j] = map->c;
		map->map[i][j] = '\n';
		map->map[i][j + 1] = '\0';
		j = -1;
	}
}

int		background_error(t_map *map)
{
	return (map->w < 1 || map->w > 300 || map->h < 1 || map->h > 300);
}

int		is_in(int x, int y, t_r *rect)
{
	return (rect->xtl <= x && x <= rect->xbr && rect->ytl <= y && y <= rect->ybr);
}

int		is_border(float x, float y, t_r *rect)
{
	return (((is_in(x, y, rect)) && ((rect->xtl + rect->w - x < 1.0 || rect->ytl + rect->h - y < 1.0) || (x - rect->xtl < 1.0 || y - rect->ytl < 1.0))));
}

void	draw_rect(t_map *map, t_r *rect)
{
	int i = -1;
	int j = -1;

	while (++i < map->h)
	{
		while (++j < map->w)
		{
			if (rect->type == 'r')
			{
				if (is_border(j, i, rect) == 1)
					map->map[i][j] = rect->c;
			}
			else if (rect->type == 'R')
			{
				if (is_in(j, i, rect) == 1)
					map->map[i][j] = rect->c;
			}
		}
		j = -1;
	}
}

int     main(int ac, char **av)
{
	FILE	*f = NULL;
	t_map	map;
	t_r		rect;
	int		background = 0;
	int		new_rect = 0;

    if (ac != 2)
		return (ft_putstr("Error: argument\n"));
	f = fopen(av[1], "r");
	if (f == NULL || feof(f))
		return (1);
	background = fscanf(f, "%d %d %c \n", &map.w, &map.h, &map.c);
	if (background != 3 || background_error(&map) == 1)
		return (ft_putstr("Error: Operation file corrupted\n"));
	set_map(&map);
	new_rect = fscanf(f, "%c %f %f %f %f %c \n", 
	&rect.type, &rect.xtl, &rect.ytl, &rect.w, &rect.h, &rect.c);
	while (new_rect == 6)
	{
		rect.ybr = rect.ytl + rect.h;
		rect.xbr = rect.xtl + rect.w;
		if ((rect.type != 'r' && rect.type != 'R') || 
		(rect.w <= 0.0 || rect.h <= 0.0))
			return (ft_putstr("Error: Operation file corrupted\n"));
		draw_rect(&map, &rect);
		new_rect = fscanf(f, "%c %f %f %f %f %c \n", 
		&rect.type, &rect.xtl, &rect.ytl, &rect.w, &rect.h, &rect.c);
	}
	if (new_rect != -1)
		return (ft_putstr("Error: Operation file corrupted\n"));
	display(&map);
	if (fclose(f) != 0)
		return (1);
	return (0);
}
