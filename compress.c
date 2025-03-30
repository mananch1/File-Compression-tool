#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct a{
	char c;
	struct a *left;
	struct a *right;
} TreeNode;

typedef struct
{
	int freq;
	char a;
} pair;


typedef struct
{
	pair **a;
	int n;
} queue;

void init_pair(pair *p,int freq,int a);

void init_queue(queue *q);
void enqueue(queue *q, pair *p);
pair* dequeue(queue *q);
void print_queue(queue* q);

int file_exists(const char *filename);
char* find_extension(char *in);
void file_print(char * name);

int main(int argc,char **argv)
{
	if(argc!=2) // This check returns feedback to the user saying that no filename is attached
	{
		printf("Please only pass the filename as an argument");
		return 1;
	}
	if(!file_exists(argv[1]))//this check prevents the user from entering a file which does not exist
	{
		printf("Error 404 : File not found");
		return 404;
	}
	file_print(argv[1]);
	return 0;
}

int file_exists(const char *filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0 ? 1 : 0;
}

char* find_extension(char *in)
{
	int ext;
	int i=0;
	int len;
	char *out;
	while(in[i]!='\0')
	{
		if(in[i]=='.')
			ext=i;
		i++;
	}
	len=i;
	out = (char *)malloc((ext+1+3)*sizeof(char));
	for(int i=0;i<=ext;i++)
		out[i]=in[i];
	out[ext+1]='o';
	out[ext+2]='u';
	out[ext+3]='t';
	return out;
}

void file_print(char * name)
{
	size_t *bytesFrequency = (size_t*) calloc(256, sizeof(size_t));
	unsigned char data[1024];
	char *ext =find_extension(name);

	FILE *fp = fopen(name,"rb");
	FILE *op = fopen(ext,"wb");
	
	size_t br;
	while((br=fread(data,1,sizeof(data),fp))>0)
	{
		fwrite(data,1,br,op);
		//printf("%s",data);
		for(int i=0;i<br;i++)
		{
			//printf("\n%c: %i",data[i],data[i]);
			bytesFrequency[data[i]]++;
		}
	}

	//Inserting into a priortiy queue
	queue q;
	init_queue(&q);
	for(int i=0;i<255;i++)
	{
		if(bytesFrequency[i]!=0)
		{
			pair* t= (pair *)malloc(sizeof(pair));
			init_pair(t,bytesFrequency[i],i);
			enqueue(&q,t);
		}
	}
	
}


void init_queue(queue *q)
{
	q->a = (pair **)malloc(256*sizeof(pair));
	q->n = -1;
}

void print_queue(queue* q)
{
	int i=0;
	for(i=0;i<q->n;i++)
		printf("\n%c %i",q->a[i]->a,q->a[i]->freq);
}

void init_pair(pair *p,int freq,int a)
{
	p->freq=freq;
	p->a=a;
}

void enqueue(queue *q, pair *p)
{
	if(q->n==-1)
	{
		q->n++;
		q->a[q->n]=p;
	}
	else
	{
		int i=0;
		while(i<=q->n&&(q->a[i]->freq)<(p->freq))
			i++;
		int c=i;
		q->n++;
		i=q->n;
		while(i>c)
		{
			q->a[i]=q->a[i-1];
			i--;
		}
		q->a[i]=p;
	}
}

pair* dequeue(queue *q)
{
	pair *p = q->a[0];
	int i;
	for(i=0;i<q->n;i++)
		q->a[i]=q->a[i+1];
	q->n--;
	return p;
}