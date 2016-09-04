# include <stdio.h>
# include <gtk/gtk.h>
# include <string.h>
# include <math.h>
# include <ctype.h>
# include <malloc.h>

GtkWidget *entrada;
GtkWidget *label_saida;
GtkWidget *label_saida1;
GtkWidget *label_saida2;
GtkWidget *label_saida3;
GtkWidget *label_saida4;
GtkWidget *label_saida5;
GtkWidget *label_saida6;
GtkWidget *label_saida7;
GtkWidget *label_saida8;
GtkWidget *label_saida9;
GtkWidget *label_saida10;
GtkWidget *label_saida11;
FILE *municipiosBrasil;
struct Cidade
{
	char nome[50];
	char uf[3];
	int id;
	double lat, lon, dist;
	struct Cidade *prox;
	struct Cidade *ant;
};
typedef struct Cidade cidade;
cidade *p = NULL;
cidade *escolhida = NULL;
int const max = 5510;
static const double DEG_TO_RAD = M_PI/180.0;
static const double RAD_TO_DEG = 180.0/M_PI;
static const double R = 6372.797560856;
char city[50];
double valor[5510];
int enche();
void calculaDistancia();
double Haversine(double lat1,double lon1, double lat2, double lon2);
void ordena(int ini, int fim);
void espelho();
void gotoxy(int x, int y);
void troca(cidade *i, cidade *j);
gboolean imprime(GtkButton *button, gpointer data);
void insere(int i);
int busca();
char nome();
main(int argc, char **argv)
{
	int i, j, cont = 0;
	enche();
	cidade *aux = p;
	GtkWidget *janela;
	GtkWidget *conteudo;
  	GtkWidget *label_entrada;
  	GtkWidget *botao;

  	gtk_init(&argc, &argv);

  	janela=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(janela), GTK_WIN_POS_CENTER);

  	conteudo=gtk_vbox_new(FALSE, 0);
  	gtk_container_add(GTK_CONTAINER(janela), conteudo);

  	label_entrada=gtk_label_new("Informe a cidade:");
  	gtk_box_pack_start(GTK_BOX(conteudo), label_entrada, FALSE, FALSE, 0);

  	entrada=gtk_entry_new();
  	gtk_box_pack_start(GTK_BOX(conteudo), entrada, FALSE, FALSE, 0);

  	botao=gtk_button_new_with_label("Procurar cidades");
  	gtk_box_pack_start(GTK_BOX(conteudo), botao, FALSE, FALSE, 0);
//	puts("testeooo");
  	label_saida=gtk_label_new("");
	gtk_box_pack_start(GTK_BOX(conteudo), label_saida, FALSE, FALSE, 0);

	label_saida1=gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(conteudo), label_saida1, FALSE, FALSE, 0);

	label_saida2=gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(conteudo), label_saida2, FALSE, FALSE, 0);

	label_saida3=gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(conteudo), label_saida3, FALSE, FALSE, 0);

	label_saida4=gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(conteudo), label_saida4, FALSE, FALSE, 0);        
	
	label_saida5=gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(conteudo), label_saida5, FALSE, FALSE, 0);        

	label_saida6=gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(conteudo), label_saida6, FALSE, FALSE, 0);        

	label_saida7=gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(conteudo), label_saida7, FALSE, FALSE, 0);        

	label_saida8=gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(conteudo), label_saida8, FALSE, FALSE, 0);        

	label_saida9=gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(conteudo), label_saida9, FALSE, FALSE, 0);        

	label_saida10=gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(conteudo), label_saida10, FALSE, FALSE, 0);

	label_saida11=gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(conteudo), label_saida11, FALSE, FALSE, 0);


  	gtk_widget_show_all(janela);
  	g_signal_connect(G_OBJECT(janela), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  	g_signal_connect(G_OBJECT(botao), "clicked", G_CALLBACK(imprime), NULL);

//  	gtk_widget_show_all(janela);
	/*do
	{
		system("clear");
		if(cont != 0)
		{
			cont = 0;
			printf("Esta cidade não existe.Tente novamente sem acentuação, com todas as letras em maiúculo e sem esaços.");
		}
	//	printf("| *Saiba quais são as dez cidades mais próximas* |%c ________________________________________________________________________________________%c", 10, 10);
		printf("Cidade: ");
		scanf("%s", city);
		for(i = 0; i < 50; i++)
		{
			if(city[i] == ' ')
			{
				for(j = i; j < 50; j++)
					city[j] = city[j + 1];
			}
		}
		for(i = 0; i < 50; i++)
		{
			city[i] = toupper(city[i]);
		}
		cont ++;
	}while(busca() == 0);*/
	
/*	calculaDistancia();
	ordena(1, max);
	espelho();
	//imprime();*/
  	gtk_main();
  	return 0;
}
void gotoxy(int x,int y)
{
	printf("%c[%d;%df",0x1B,y,x);
}
void espelho()
{
	int percorre = max, i;
	cidade *aux = p;
	cidade *auxb;
	while(percorre != 1)
	{
		if(valor[percorre] != aux->dist)
		{
			auxb = aux;
			do
			{
				auxb = auxb->prox;
				if(valor[percorre] && valor[percorre - 1] == 0.0)
					break;
			}
			while(auxb->dist != valor[percorre]);
			troca (aux, auxb);
		}
		aux = aux->prox;
		percorre--;
	}
}
void ordena(int esquerda, int direita)
{
	int i, j;
	double x, y;
    	i = esquerda;
	j = direita;
    	x = valor[(esquerda + direita) / 2];
 
    	while(i <= j)
    	{
        	while(valor[i] < x && i < direita)
        	{
            		i++;
        	}
        	while(valor[j] > x && j > esquerda)
        	{
            		j--;
        	}
        	if(i <= j)
        	{
            		y = valor[i];
	            	valor[i] = valor[j];
            		valor[j] = y;
            		i++;
            		j--;
        	}
    	}
    	if(j > esquerda)
    	{
        	ordena(esquerda, j);
    	}
    	if(i < direita)
    	{
        	ordena(i, direita);
    	}
}
void troca(cidade *i, cidade *j)
{
	cidade *aux = (cidade *) malloc(sizeof(cidade));
	aux->lat = i->lat;
	aux->lon = i->lon;
	strcpy(aux->nome, i->nome);
	aux->dist = i->dist;
	strcpy(aux->uf, i->uf);
	i->lat = j->lat;
        i->lon = j->lon;
        strcpy(i->nome, j->nome);
        i->dist = j->dist;
	strcpy(i->uf, j->uf);
	j->lat = aux->lat;
        j->lon = aux->lon;
        strcpy(j->nome, aux->nome);
        j->dist = aux->dist;
	strcpy(j->uf, aux->uf);
}
void calculaDistancia()
{
	cidade *aux = p;
	int i = 1;
	while(i < max)
	{
		valor[i] = aux->dist = Haversine(escolhida->lat, escolhida->lon, aux->lat, aux->lon);
		i++;
		aux = aux->prox;
	}
}
double Haversine(double lat1,double lon1, double lat2, double lon2)
{
        double dlon = (lon2 - lon1) * DEG_TO_RAD;
        double dlat = (lat2 - lat1) * DEG_TO_RAD;
        double a =  pow(sin(dlat * 0.5),2)+ cos(lat1*DEG_TO_RAD) * cos(lat2*DEG_TO_RAD) * pow(sin(dlon * 0.5),2);
        double c = 2.0 * atan2(sqrt(a), sqrt(1-a));
        return R * c;
}
int busca()
{
	cidade *aux = p;
	int i = 1;
	while(i < max)	
	{
		if(strcmp(city, aux->nome) == 0)
		{
			escolhida = aux;
			return 1;
		}
		aux = aux->prox;
		i++;
	}
	return 0;
}
int enche()
{
	int i;
	if((municipiosBrasil = fopen("municipiosBrasil.csv", "r")) == NULL)
	{
		puts("Erro. O arquivo não pode ser aberto para leitura!");
		return 0;
	}
	fseek(municipiosBrasil, 0, SEEK_SET);
	for(i = 1; i < max; i++)
	{
	
		insere(i);
	}
	fclose(municipiosBrasil);
}
void insere(int i)
{
	cidade *novo = (cidade *) malloc(sizeof(cidade));
	fscanf(municipiosBrasil,"%lf %lf %s %s",&novo->lat, &novo->lon, novo->nome, novo->uf);
	novo->id = i;
	if(p == NULL)
	{
		p = novo;
		novo->prox = NULL;
		novo->ant = NULL;
	}
	novo->ant = NULL;
	novo->prox = p;
	p->ant = novo;
	p = novo;
}
gboolean imprime(GtkButton *button, gpointer data)
{
	cidade *aux = p;
	char texto[150];
	char padrao[150];
	char erro[30];
	int tempo = 1, i;
	strcpy(city, gtk_entry_get_text(GTK_ENTRY(entrada)));
	for(i = 0; i < 50; i++)
        {
        	city[i] = toupper(city[i]);
        }
	while(busca() == 0)
	{
		sprintf(erro, "Cidade não existe");
                gtk_label_set_text(GTK_LABEL(label_saida), erro);

	}
        calculaDistancia();
        ordena(1, max);
        espelho();
	while(aux->id > 1)
	{
		aux = aux->prox;
	}
	while(tempo < 12)	
	{
		if(aux->id == 1)
		{
			sprintf(padrao, "As dez cidades mais próximas de você:%c", 10);
			gtk_label_set_text(GTK_LABEL(label_saida), padrao);
		}
		else
		{	
				sprintf(texto, "%d. %s-%s	%lf Kilômetros.%c", tempo - 1, aux->nome, aux->uf, aux->dist, 10);
			if(tempo == 1)
				gtk_label_set_text(GTK_LABEL(label_saida1), texto);
			else if(tempo == 2)
				gtk_label_set_text(GTK_LABEL(label_saida2), texto);
			else if(tempo == 3)
				gtk_label_set_text(GTK_LABEL(label_saida3), texto);
			else if(tempo == 4)
				gtk_label_set_text(GTK_LABEL(label_saida4), texto);
			else if(tempo == 5)
				gtk_label_set_text(GTK_LABEL(label_saida5), texto);
			else if(tempo == 6)
				gtk_label_set_text(GTK_LABEL(label_saida6), texto);
			else if(tempo == 7)
				gtk_label_set_text(GTK_LABEL(label_saida7), texto);
			else if(tempo == 8)
				gtk_label_set_text(GTK_LABEL(label_saida8), texto);
			else if(tempo == 9)
				gtk_label_set_text(GTK_LABEL(label_saida9), texto);
			else if(tempo == 10)
				gtk_label_set_text(GTK_LABEL(label_saida10), texto);
			else if(tempo == 11)
				gtk_label_set_text(GTK_LABEL(label_saida11), texto);

	
		}
		aux = aux->ant;
		tempo++;
	}
	return FALSE;
}
