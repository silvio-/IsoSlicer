//---------------------------------------------------------------------------
#include<math.h>
#ifndef uMatH
#define uMatH
#define BBTOL 0.01
#define TOL 0.00001
//---------------------------------------------------------------------------

class vetor {
public:
	double x,y,z;
	void carregue(double xx, double yy, double zz);
	vetor X(vetor v);
	double vezes(vetor v);
	void vezes(double k);
	double norma();
	void normalize();
	vetor menos(vetor v);
    vetor subtraia(vetor v);
	void copie(vetor v);
	vetor projete_sobre(vetor v);
	vetor multiescalar(double k);
	vetor gramschmidt(vetor v);
};


class ponto {
public:
	double x,y,z;
	void some(vetor v);
	vetor menos(ponto p);
	void origemmais(vetor v);
	void carregue(double xx, double yy, double zz);
	void combine(ponto P, vetor v);
	double dist(ponto P);
};

typedef struct plist{
	ponto P;
	float t, uval;
	plist *prox,*ant;
}ptlist;


class Ponto {
public:
	double x,y,z,w;
	Ponto(double xx, double yy, double zz, double ww);
	void carregue(double xx, double yy, double zz, double ww);
	Ponto();
	void normalize();
	ponto projete();
    vetor v();

};

class baricentrico {
public:
	float alfa, beta;
	void coordenadas_baricentricas(ponto P1, ponto P2, ponto P3,float px,float py);
};

float norma(baricentrico d);

//classe criada para implementar o algoritmo NEXKSB do Nijenhuis-Wilf
//NEXKSB=próximo subconjunto de tamanho k de um conjunto de tamanho n
//a rotina next_k_subset deixa a máscara mask com os elementos selecionados
//com um "1" ao invés de "0" dos elementos não selecionados para fazer parte
//do próximo  k-subsonjunto.
class nexksb{
    int m,n,k,h,next;
    public:
    int *a,*mask;
	void prepare(int tam, int maxr, int **mark);
	void reinicie(int subtam);
	int next_k_subset();
    void desaloque();
};

class dmemo{
	public:
	int n;
	int npts;
	double *dw;
	vetor *db;
	bool *existe_db, *existe_dw;
	void desaloque();
	void inicialize(int nn);
	void Guarde_dw(int *mask, double dW);
	void Guarde_db(int *mask, vetor dB);
	bool Existe_dw(int *mask);
	bool Existe_db(int *mask);
	double Retorne_dw(int *mask);
	vetor Retorne_db(int *mask);
};


//- Resolução de sistemas lineares n x n

class SisLin{
	public:
	int n;
	double **A;
	void inicialize(int nn);
	void desaloque();
	void zere_matriz();
	void simetrize();
	void carregue(int i, int j, double val);
	void adicione(int i, int j, double val);
	double *egaussiana();
};



#endif


