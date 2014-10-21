#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <gdk/gdkkeysyms-compat.h>

#define min(a, b)		((a) < (b)? (a): (b))
#define sqr(a)			((a) * (a))

struct _obj {
	int tp;
	int x, y;
};
struct _lst {
	struct _lst *n;
	int x, y;
};

typedef struct _obj obj;
typedef struct _lst lst;

	GtkWidget *window;
	GtkWidget *canvas;

	static int trys[] = {
		1,  0,
		-1,  0,
		1,  1,
		-1,  1,
		1, -1,
		-1, -1,
		0,  1,
		0, -1,
	};

int w, h;
int *fd;
int monsts;
int blocks;
obj *pers;

void gen();
void print(){
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			fprintf(stderr, "%c", fd[y * w + x] == 0x100? '#': fd[y * w + x] == 0x10? 'X': fd[y * w + x] == 1? '0': '_');
		//	fprintf(stderr, "%d %d  = %d\n", x, y, fd[y * w + x]);
		}
		fprintf(stderr, "\n");
	}
}
void loose(){
	fprintf(stderr, "LOOSE\n");
	gen();
}
void win(){
	fprintf(stderr, "WIN\n");
	gen();
}
int move_monst_try(int i, int x, int y){
	if(x >= 0 && x < w && y >= 0 && y < h){
		if(fd[y * w + x] == 0 || fd[y * w + x] == 1){
			if(fd[y * w + x] == 0){
				fd[pers[i].y * w + pers[i].x] = 0;

				pers[i].x = x;
				pers[i].y = y;
				fd[pers[i].y * w + pers[i].x] = pers[i].tp;
			}else{
				loose();
			}
			return 1;
		}
	}

	return 0;
}
void calc_dist(int *arr, int x, int y){
	for(int i = 0; i < w * h; i++)
		arr[i] = w * h + 10;
	
	lst *st = calloc(1, sizeof(lst));
	st->x = x;
	st->y = y;
	lst *end = st;
	arr[st->y * w + st->x] = 0;

	do{
		int x = st->x, y = st->y;
		int s = y * w + x;
		for(int j = 0; j < 16; j += 2){
			int nx = x + trys[j];
			int ny = y + trys[j + 1];
			if(nx >= 0 && nx < w && ny >= 0 && ny < h){
				int s1 = ny * w + nx;
				if(fd[s1] == 0 && arr[s1] > arr[s] + 1){
					lst *tmp = calloc(1, sizeof(lst));
					tmp->x = nx;
					tmp->y = ny;
					end = end->n = tmp;
					arr[s1] = arr[s] + 1;
				}
			}
		}
		lst *n = st->n;
		free(st);
		st = n;
	}while(st != NULL);
}
int find_path(int a, int b, int *r_x, int *r_y){
	int *arr = malloc(w * h * sizeof(*arr));
	
	calc_dist(arr, pers[b].x, pers[b].y);

	int dist = w * h + 1000;
	int _x = -1, _y = -1;
	{
		int x = pers[a].x;
		int y = pers[a].y;
		for(int j = 0; j < 16; j += 2){
			int nx = x + trys[j];
			int ny = y + trys[j + 1];
			if(nx >= 0 && nx < w && ny >= 0 && ny < h){
				int s1 = ny * w + nx;
				if(arr[s1] < dist){
					dist = arr[s1];
					_x = nx;
					_y = ny;
				}
			}
		}
	}
	dist ++;

	if(dist < w * h){
		*r_x = _x;
		*r_y = _y;
		free(arr);
		return dist;
	}

	calc_dist(arr, pers[a].x, pers[b].y);

	int bx = pers[monsts].x;
	int by = pers[monsts].y;
	int _min = w * h;
	int nr_x = -1, nr_y = -1;
	for(int Y = 0; Y < h; Y++)
		for(int X = 0; X < w; X++){
			if(arr[Y * w + X] < w * h){
				int d = sqr(X - bx) + sqr(Y - by);
				if(d < _min){
					_min = d;
					nr_x = X;
					nr_y = Y;
				}
			}
		}

	calc_dist(arr, nr_x, nr_y);
	
	dist = w * h + 1000;
	_x = -1, _y = -1;
	{
		int x = pers[a].x;
		int y = pers[a].y;
		for(int j = 0; j < 16; j += 2){
			int nx = x + trys[j];
			int ny = y + trys[j + 1];
			if(nx >= 0 && nx < w && ny >= 0 && ny < h){
				int s1 = ny * w + nx;
				if(arr[s1] < dist){
					dist = arr[s1];
					_x = nx;
					_y = ny;
				}
			}
		}
	}
	dist ++;

	*r_x = _x;
	*r_y = _y;

	free(arr);

	return -1;
}
void move_monst(int i){
	double rnd = (double)rand() / RAND_MAX;
	if(rnd < 0.9){
		int moved = 0;
		rnd = (double)rand() / RAND_MAX;
		int x = pers[i].x, y = pers[i].y;
		if(rnd < 0.5){
			int nx, ny;
			int r = find_path(i, monsts, &nx, &ny);
			if(r > 0){
				if(move_monst_try(i, nx, ny))
					moved = 1;
			//	fprintf(stderr, "i follow you  %d %d -> %d %d  -> %d %d  (%d)\n", x, y, nx, ny, pers[monsts].x, pers[monsts].y, r);
			}else if(rnd < 0.3){
				if(move_monst_try(i, nx, ny))
					moved = 1;
			//	fprintf(stderr, "i am near you  %d %d -> %d %d  -> %d %d  (%d)\n", x, y, nx, ny, pers[monsts].x, pers[monsts].y, r);
			}
		}
		if(!moved){
			int ms[8] = {0, 1, 2, 3, 4, 5, 6, 7};
			for(int j = 0; j < 20; j++){
				int a = rand() % 8;
				int b = rand() % 8;
				int q = ms[a];
				ms[a] = ms[b];
				ms[b] = q;
			}
			for(int j = 0; j < 8; j ++){
				if(move_monst_try(i, x + trys[ms[j] * 2], y + trys[ms[j] * 2 + 1]))
					break;
			}
		}
	}
}
int monst_can_move(){
	for(int i = 0; i < monsts; i++){
		int mx = pers[i].x, my = pers[i].y;

		int j = 0;
		do{
			int x = mx + trys[j], y = my + trys[j + 1];

			if(x >= 0 && x < w && y >= 0 && y < h)
				if(fd[y * w + x] == 0 || fd[y * w + x] == 1)
					return 1;

			j += 2;
		}while(j < 16);
	}

	return 0;
}
guint tick_1(gpointer data){
//	fprintf(stderr, "time clocks  %p\n", data);

	for(int i = 0; i < monsts; i++)
		move_monst(i);
	
//	print();

	gtk_widget_queue_draw(canvas);

	return TRUE;
}
void move_me(int dir){
	int x = pers[monsts].x;
	int y = pers[monsts].y;

	static int moves[] = {
			0, -1,
			-1, 0,
			0, 1,
			1, 0
		};

	int dx = moves[dir * 2 + 0];
	int dy = moves[dir * 2 + 1];

	int nx = x + dx, ny = y + dy;
	
	int len = 0;
	while(nx >= 0 && nx < w && ny >= 0 && ny < h){
		int tp = fd[ny * w + nx];
		if(tp == 0){
			fd[ny * w + nx] = 0x100;

			fd[y * w + x] = 0x0;

			fd[(y + dy) * w + (x + dx)] = 1;

			pers[monsts].x = x + dx;
			pers[monsts].y = y + dy;

			break;
		}else if(tp & 0xf0){
			if(len == 0){
				loose();
				return;
			}else{
				break;
			}
		}

		nx += dx, ny += dy;
		len ++;
	}

	if(! monst_can_move()){
		win();
	}

	gtk_widget_queue_draw(canvas);
}
void gen(){
	for(int i = 0; i < w * h; i++)
		fd[i] = 0;

	int i = 0;
	while(i < monsts + 1){
		int x = rand() % w;
		int y = rand() % h;

		if(fd[y * w + x] == 0){
			pers[i].tp = 0x10;
			pers[i].x = x;
			pers[i].y = y;

			fd[pers[i].y * w + pers[i].x] = pers[i].tp;

			i++;
		}
	}

	i --;
	fprintf(stderr, "me %d at  %d %d\n", i, pers[i].x, pers[i].y);

	pers[i].tp = 0x1;
	fd[pers[i].y * w + pers[i].x] = pers[i].tp;

	i = 0;
	do{
		int x = rand() % w;
		int y = rand() % h;
		if(fd[y * w + x] == 0){
			fd[y * w + x] = 0x100;
			i++;
		}
	}while(i < blocks);

	print();
}
void init(){
	w = 40, h = 27;
	monsts = w * h * 0.01;
	blocks = w * h * 0.3;
	fd = calloc(w * h, sizeof(*fd));
	pers = calloc(monsts + 1, sizeof(*pers));

	gen();
}
void deinit(){
	free(fd);
	free(pers);
}
guint key(GtkWidget *canv, GdkEventKey *ev, gpointer data){
	static int keys[10] = {119, 97, 115, 100,
							0xff52, 0xff51, 0xff54, 0xff53};

	int k = ev->keyval;

	switch(k){
		case GDK_a:
		case GDK_A:
		case GDK_w:
		case GDK_W:
		case GDK_s:
		case GDK_S:
		case GDK_d:
		case GDK_D:
		case GDK_Up:
		case GDK_Left:
		case GDK_Down:
		case GDK_Right:
		//	fprintf(stderr, "i know this key: %s %d\n", gdk_keyval_name(ev->keyval), ev->keyval);
			for(int i = 0; i < 8; i++){
				if(keys[i] == k){
					if(i >= 4)
						i -= 4;
					move_me(i);
					break;
				}
			}
			break;
		default:
			break;
	}

	return FALSE;
}
guint draw(GtkWidget *canv, cairo_t *cr, gpointer data){
	cairo_set_source_rgb(cr, .7, .7, .9);
	cairo_paint(cr);

	guint ww = gtk_widget_get_allocated_width(canv);
	guint wh = gtk_widget_get_allocated_height(canv);

//	fprintf(stderr, "draw %u %u\n", ww, wh);

	double st_x = (double)ww / w;
	double st_y = (double)wh / h;
	double bd = 2;
	double sz_x = st_x - bd * 2;
	double sz_y = st_y - bd * 2;

	for(int y = 0; y < h; y++)
		for(int x = 0; x < w; x++){
			int tp = fd[y * w + x];
			if(tp > 0){
				double px = x * st_x, py = y * st_y;

				double r, g, b;

				switch(tp){
					case 0x1:
						r = 1, g = 0, b = 0;
						break;
					case 0x10:
						r = 0.5, g = 0.5, b = 1;
						break;
					case 0x100:
						r = 0.8, g = 0.1, b = 0.8;
						break;
					default:
						r = 0.3, g = 0.3, b = 0.3;
						break;
				}
				cairo_set_source_rgb(cr, r, g, b);
				cairo_rectangle(cr, px + bd, py + bd, sz_x, sz_y);
				cairo_set_line_width(cr, bd / 2);
				cairo_fill(cr);
			}
		}
/*
	cairo_arc(cr, x, y, r, 0, 2 * G_PI);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_fill_preserve(cr);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0.42, 0.65, 0.80);
	cairo_set_line_width(cr,6);
	cairo_rectangle (cr, 3, 3, 100, 100);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0.17, 0.63, 0.12);
	cairo_set_line_width(cr,2);
	cairo_arc(cr, 150, 210, 20, 0, 2*G_PI);
	cairo_stroke(cr);

	cairo_set_source_rgb(cr, 0.77, 0.16, 0.13);
	cairo_set_line_width(cr, 6);
	cairo_move_to(cr, 80,160);
	cairo_line_to(cr, 200, 160);
	cairo_stroke(cr);
	*/

	return FALSE;
}

int main(int argc, char *argv[]){
	srand(time(NULL));
	init();

	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW (window), 1600, 900);
	gtk_window_set_title(GTK_WINDOW(window), "Здравствуй, мир!");
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	canvas = gtk_drawing_area_new();
	gtk_widget_set_size_request(canvas, 640, 600);
	g_signal_connect(canvas, "draw", G_CALLBACK(draw), NULL);
	gtk_container_add(GTK_CONTAINER(window), canvas);

	g_signal_connect(window, "key_press_event", G_CALLBACK(key), NULL);
//	g_signal_connect(window, "key_release_event", G_CALLBACK(key), NULL);
	g_timeout_add(1000, (GSourceFunc)tick_1, NULL);

	fprintf(stderr, "init: win %p\n", window);

//	GtkWidget *label;
//	label = gtk_label_new("Здравствуй, мир!");
//	gtk_container_add(GTK_CONTAINER(window), label);

	gtk_widget_show_all(window);

	gtk_main();

	deinit();
	
	return 0;
}
