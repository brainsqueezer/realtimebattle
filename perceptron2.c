#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

struct neurona{
    double salida;
    double error;
    double (*funcionActivacion)(double x);
    double (*derivadaActivacion)(double x);
    double *pesos;
};


double funcionActivacion(double x);
double derivadaActivacion(double x);
void inicializa(int nentradas,int ncapas,int tamanocapa[],struct neurona **capas[]);
void activa(int nentradas,int ncapas,int tamanocapa[],double entradas[],struct neurona **capas[]);
void calculaerrores(int nentradas,int ncapas,int nsalidas,int tamanocapa[],double entradas[],double objetivos[],struct neurona **capas[]);
void entrena(int nentradas,int ncapas,int nsalidas,int tamanocapa[],double entradas[],double objetivos[],struct neurona **capas[]);

int main(int argc,char *argv[]){
    int i,tiempoinicial=0,tiempo=0;
    int ncapas=3;
    struct neurona **capas[ncapas];
    int tamanocapa[ncapas];//={80,20,1};  //ncapas comentado porque c dice que si es variable no inicializa.
    int nentradas=3,nsalidas=1;
    double entradas[nentradas];
    double objetivos[nsalidas];
    tamanocapa[0]=800;
    tamanocapa[1]=100;
    tamanocapa[2]=1;
    entradas[0]=2;
    entradas[1]=1;
    entradas[2]=1;
    objetivos[0]=0.5;

    tiempoinicial=time(NULL);
    printf("%d", tiempoinicial);

    inicializa(nentradas,ncapas,tamanocapa,capas);

    tiempo=time(NULL)-tiempoinicial;
    printf("\ntiempo:%d\n",tiempo);

    activa(nentradas,ncapas,tamanocapa,entradas,capas);

    printf("\ntiempo:%d\n",tiempo);
    printf("mierdainfernal\n");
    printf("salida %f\n",(*capas[2])[0].salida);
    printf("error %f\n",(*capas[2])[0].error);
    printf("salida %f\n",(*capas[1])[0].salida);
    printf("error %f\n",(*capas[1])[0].error);
    printf("salida %f\n",(*capas[0])[0].salida);
    printf("error %f\n",(*capas[0])[0].error);

    for(i=1500;i>0;i--){
	entrena(nentradas,ncapas,nsalidas,tamanocapa,entradas,objetivos,capas);
    }

    tiempo=time(NULL)-tiempoinicial;
    printf("\ntiempo:%d\n",tiempo);
    printf("mierdainfernal\n");
    printf("salida %f\n",(*capas[2])[0].salida);
    printf("error %f\n",(*capas[2])[0].error);
    printf("salida %f\n",(*capas[1])[0].salida);
    printf("error %f\n",(*capas[1])[0].error);
    printf("salida %f\n",(*capas[0])[0].salida);
    printf("error %f\n",(*capas[0])[0].error);

    return(0);
}

void inicializa(int nentradas,int ncapas,int tamanocapa[],struct neurona **capas[]){
    struct neurona *capa;
    struct neurona neuronaejemplo;//para malloc, sino peta.
    int i,neurona,peso;

    capa=(struct neurona *) malloc(tamanocapa[0]*sizeof(neuronaejemplo));//reservo memoria para esta capa de neuronas.
    capas[0]=&capa;//asigno un puntero a la capa a una posicion en el array de capas.*/
    capas[0]=(struct neurona **) malloc(sizeof(&neuronaejemplo));//reservo memoria para un puntero a un array de capa de neuronas.
    *capas[0]=(struct neurona *) malloc(tamanocapa[0]*sizeof(neuronaejemplo));
    capa=*capas[0];
    for (neurona=0; neurona<tamanocapa[0]; neurona++){
	capa[neurona].pesos=(double *)malloc(nentradas*sizeof(double));//reservo memoria para tantos pesos como entradas haya.
	capa[neurona].funcionActivacion=funcionActivacion;//asigno una funcion de activacion
	capa[neurona].derivadaActivacion=derivadaActivacion;//asigno derivada de funcion de activacion
	for (peso=0;peso<nentradas;peso++){
	    capa[neurona].pesos[peso]=(double)rand()/((double)(RAND_MAX)+(double)(1));//inicializo los pesos a un numero aleatorio entre 0 y 1
	}
    }
    for(i=1;i<ncapas;i++){
	capas[i]=(struct neurona **) malloc(sizeof(&neuronaejemplo));//reservo memoria para un puntero a un array de capa de neuronas.
	*capas[i]=(struct neurona *) malloc(tamanocapa[i]*sizeof(neuronaejemplo));
	capa=*capas[i];
	for (neurona=0; neurona<tamanocapa[i]; neurona++){
	    capa[neurona].pesos=(double *)malloc(tamanocapa[i-1]*sizeof(double));//reservo memoria para tantos pesos como neuronas hay en la capa anterior.
	    capa[neurona].funcionActivacion=funcionActivacion;//asigno una funcion de activacion
	    capa[neurona].derivadaActivacion=derivadaActivacion;//asigno derivada de funcion de activacion
	    for (peso=0;peso<tamanocapa[i-1];peso++){
		capa[neurona].pesos[peso]=(double)rand()/((double)(RAND_MAX)+(double)(1));//inicializo los pesos a un numero aleatorio entre 0 y 1
	    }
	}
    }
}

void activa(int nentradas,int ncapas,int tamanocapa[],double entradas[],struct neurona **capas[]){
    struct neurona *capa,*capaanterior;
    int i,neurona,peso;

    capa=*capas[0];
    for (neurona=0; neurona<tamanocapa[0]; neurona++){
	for (peso=0;peso<nentradas;peso++){
	    capa[neurona].salida=(capa[neurona].salida+(capa[neurona].pesos[peso]*entradas[peso]));
	}
	capa[neurona].salida=capa[neurona].funcionActivacion(capa[neurona].salida);
    }
    for(i=1;i<ncapas;i++){//empieza en la 2a capa
	capa=*capas[i];
        capaanterior=*capas[i-1];
	for (neurona=0; neurona<tamanocapa[i]; neurona++){
	     for (peso=0;peso<tamanocapa[i-1];peso++){
		 capa[neurona].salida=capa[neurona].salida+(capa[neurona].pesos[peso]*capaanterior[peso].salida);
	     }
	     capa[neurona].salida=capa[neurona].funcionActivacion(capa[neurona].salida);
	}
    }
}

void calculaerrores(int nentradas,int ncapas,int nsalidas,int tamanocapa[],double entradas[],double objetivos[],struct neurona **capas[]){
    struct neurona *capa,*capasiguiente;
    int i,neurona, neuronasiguiente;

    activa(nentradas,ncapas,tamanocapa,entradas,capas);
    capa=*capas[ncapas-1];//capa de salida
    for(neurona=0;neurona<tamanocapa[ncapas-1];neurona++){//no importa el orden de recorrido dentro de una capa
        capa[neurona].error=0.5*pow(objetivos[neurona]-capa[neurona].salida,2);
    }
    for(i=ncapas-2;i>=0;i--){//recorremos las capas hacia atras
	capa=*capas[i];
	capasiguiente=*capas[i+1];
	for(neurona=0;neurona<tamanocapa[i];neurona++){//no importa el orden de recorrido dentro de una capa
	    for (neuronasiguiente=0;neuronasiguiente<tamanocapa[i+1];neuronasiguiente++){
		capa[neurona].error=capasiguiente[neuronasiguiente].pesos[neurona]*capasiguiente[neuronasiguiente].error;
		//retropropagamos el error, es decir peso que une a la neurona siguiente con esta por el error de la neurona siguiente
	    }
            capa[neurona].error=capa[neurona].error*capa[neurona].derivadaActivacion(capa[neurona].salida);//terminamos de calcular el error
	}
    }
}

void entrena(int nentradas,int ncapas,int nsalidas,int tamanocapa[],double entradas[],double objetivos[],struct neurona **capas[]){
    struct neurona *capa;
    int i,neurona,peso;

    activa(nentradas,ncapas,tamanocapa,entradas,capas);
    calculaerrores(nentradas,ncapas,nsalidas,tamanocapa,entradas,objetivos,capas);

    capa=*capas[0];
    for (neurona=0; neurona<tamanocapa[0]; neurona++){
	for (peso=0;peso<nentradas;peso++){
            capa[neurona].pesos[peso]=capa[neurona].pesos[peso]-(0.01*capa[neurona].salida*capa[neurona].error);
	}
    }
    for(i=1;i<ncapas;i++){//empieza en la 2a capa
	capa=*capas[i];
	for (neurona=0; neurona<tamanocapa[i]; neurona++){
	    for (peso=0;peso<tamanocapa[i-1];peso++){
		capa[neurona].pesos[peso]=capa[neurona].pesos[peso]-(0.01*capa[neurona].salida*capa[neurona].error);
	    }
	}
    }
}


double funcionActivacion(double x){
    return tanh(x);//tanh ralentiza todo el algoritmo 10x vs funcion identidad
}

double derivadaActivacion(double x){
   return  1-(x*x);
}



