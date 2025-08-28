//---------------------------------------------------------------------------

#pragma hdrstop

#include "uMat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#define tol 0.000001


//-----------------VETORES


void vetor::carregue(double xx, double yy, double zz)
{
	this->x = xx;
	this->y = yy;
	this->z = zz;
}

//  Produto vetorial

vetor vetor::X(vetor v)
{
  vetor w;
  w.x = this->y*v.z - v.y*this->z;
  w.y = v.x*this->z - this->x*v.z;
  w.z = this->x*v.y - v.x*this->y;
  return(w);
}

//  Produto interno

double vetor::vezes(vetor v)
{
	return(this->x*v.x + this->y*v.y + this->z*v.z);
}

double vetor::norma()
{
	return(sqrt(this->vezes(*this)));
}

vetor vetor::menos(vetor v)
{
	static vetor w;
	w.x = this->x - v.x;
	w.y = this->y - v.y;
	w.z = this->z - v.z;
	return(w);
}

vetor vetor::subtraia(vetor v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
}

void vetor::vezes(double k)
{
   this->x *= k;
   this->y *= k;
   this->z *= k;
}

vetor vetor::multiescalar(double k)
{
	static vetor w;
	w.x = k*this->x;
	w.y = k*this->y;
	w.z = k*this->z;
	return(w);
}

vetor vetor::projete_sobre(vetor v)
{
   return(v.multiescalar( this->vezes(v) / v.vezes(v) ));
}

vetor vetor::gramschmidt(vetor v)
{
	return(this->menos(this->projete_sobre(v)));
}

void vetor::copie(vetor v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

void vetor::normalize()
{
	double n = this->norma();
	if (n<tol) return;
	this->x = this->x / n;
	this->y = this->y / n;
	this->z = this->z / n;
}

//-------------------Pontos   3D

void ponto::carregue(double xx, double yy, double zz)
{
	this->x = xx;
	this->y = yy;
	this->z = zz;
}

void ponto::some(vetor v)
{
	this->x = this->x + v.x;
	this->y = this->y + v.y;
	this->z = this->z + v.z;
}

void ponto::combine(ponto P, vetor v)
{
	this->x = P.x + v.x;
	this->y = P.y + v.y;
	this->z = P.z + v.z;
}

vetor ponto::menos(ponto p)
{
	static vetor w;
	w.x = this->x - p.x;
	w.y = this->y - p.y;
	w.z = this->z - p.z;
	return(w);
}

void ponto::origemmais(vetor v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

double ponto::dist(ponto P)
{
	return(sqrt((this->x - P.x)*(this->x - P.x)+(this->y - P.y)*(this->y - P.y)));
}

//-------------------Pontos  4D

Ponto::Ponto(double xx, double yy, double zz, double ww)
{
	x =xx; y = yy; z = zz;  w = ww;
}

void Ponto::carregue(double xx, double yy, double zz, double ww)
{
	this->x = xx;
	this->y = yy;
	this->z = zz;
	this->w = ww;
}


Ponto::Ponto()
{
	x =0; y = 0; z = 0;  w = 1;
}

void Ponto::normalize()
{
	if (fabs(w)>tol) {
	   x /= w;
	   y /= w;
	   z /= w;
	   w = 1.0;
	}
}

ponto Ponto::projete()
{
	static ponto P;

	if (fabs(w)>tol) {
	   P.x = x / w;
	   P.y = y / w;
	   P.z = z / w;
	}
	else P.x = P.y = P.z = 0.0;

	return(P);
}


vetor Ponto::v()
{
	vetor vp;
	vp.carregue(x,y,z);
    return(vp);
}

float norma(baricentrico d)
{
	return(sqrt(d.alfa*d.alfa + d.beta*d.beta + (d.alfa+d.beta)*(d.alfa+d.beta)));
}


void baricentrico::coordenadas_baricentricas(ponto P1, ponto P2, ponto P3,float px,float py)
{
	float  area_total;
	area_total =(P1.x - P3.x)*(P2.y - P3.y) - (P1.y - P3.y)*(P2.x - P3.x);
	if (fabs(area_total)<TOL){
		alfa=0; beta=0;  //triângulo degenerado-> coords inválidas
	}
	else {
	  alfa =((px - P3.x)*(P2.y - P3.y) - (py - P3.y)*(P2.x - P3.x))/area_total;
	  beta =((P1.x - P3.x)*(py - P3.y) - (P1.y - P3.y)*(px - P3.x))/area_total;
	}
}


//------NEXKSB: próximo sub-conjunto de tamanho K de um conjunto de tamanho n

void nexksb::prepare(int tam, int maxr, int **mark)
{
	n=maxr;
	a=(int *)calloc(tam+1,sizeof(int));
	mask=(int *)calloc(tam+1,sizeof(int));
	*mark=mask;
}

void nexksb::reinicie(int subtam)
{
	m=0; h=k=subtam;
	next=0;
}
void nexksb::desaloque()
{
	free(a);
    free(mask);
}
int nexksb::next_k_subset()
{
	if (a[1]<n-k+1){
		next++;
		for(int j=1; j<=h;j++)
			a[k+j-h]=m+j;
        for(int i=1; i<=n; i++) mask[i]=0;
		for(int i=1; i<=k; i++) mask[a[i]]=1;
        if (m<n-h) h=0;
        h=h+1; m=a[k+1-h];
        return(1);
    }
	else return(0);
}


void dmemo::inicialize(int nn)
{
    n=nn;
    npts=pow(2,nn);
    dw=(double *)calloc(npts,sizeof(double));
	db=(vetor *)calloc(npts,sizeof(vetor));
	existe_db=(bool *)calloc(npts, sizeof(bool));
	existe_dw=(bool *)calloc(npts, sizeof(bool));
	for(int i=0; i<npts; i++){
		existe_db[i]=false;
		existe_dw[i]=false;
	}
}

void dmemo::Guarde_dw(int *mask, double dW)
{
	int ix=0,ibit;
	for(int i=1; i<=n; i++)
		if (mask[i]){
	        ibit = 1;
			ibit=ibit << (i-1);
			ix = ix | ibit;
		}
	dw[ix]=dW;
	existe_dw[ix]=true;
}

void dmemo::Guarde_db(int *mask, vetor dB)
{
	int ix=0,ibit;
	for(int i=1; i<=n; i++)
		if (mask[i]){
			ibit = 1;
			ibit=ibit << (i-1);
			ix = ix | ibit;
		}
	db[ix]=dB;
	existe_db[ix]=true;
}

bool dmemo::Existe_dw(int *mask)
{
	int ix=0,ibit;
	for(int i=1; i<=n; i++)
		if (mask[i]){
			ibit = 1;
			ibit=ibit << (i-1);
			ix = ix | ibit;
		}
	return(existe_dw[ix]);
}

bool dmemo::Existe_db(int *mask)
{
	int ix=0,ibit;
	for(int i=1; i<=n; i++)
		if (mask[i]){
			ibit = 1;
			ibit=ibit << (i-1);
			ix = ix | ibit;
		}
	return(existe_db[ix]);
}

double dmemo::Retorne_dw(int *mask)
{
	int ix=0,ibit;
	for(int i=1; i<=n; i++)
		if (mask[i]){
			ibit = 1;
			ibit=ibit << (i-1);
			ix = ix | ibit;
		}
	return(dw[ix]);
}

vetor dmemo::Retorne_db(int *mask)
{
	int ix=0,ibit;
	for(int i=1; i<=n; i++)
		if (mask[i]){
			ibit = 1;
			ibit=ibit << (i-1);
			ix = ix | ibit;
		}
	return(db[ix]);
}

void dmemo::desaloque()
{
	free(dw);
	free(db);
	free(existe_dw);
	free(existe_db);
}


//----------------Resolução de sistemas lineares n x n

void SisLin::inicialize(int nn)
{
	n=nn;
	A = (double **)calloc(n, sizeof(double *));
	for(int i=0; i<n; i++)
		A[i]=(double *)calloc(n+1, sizeof(double));
}

void SisLin::desaloque()
{
	for(int i=0; i<n; i++)
		free(A[i]);
	free(A);
}


void SisLin::carregue(int i, int j, double val)
{
	A[i][j] = val;
}

void SisLin::simetrize()
{
	for(int i=0; i<n-1; i++)
		for(int j=i+1; j<n; j++)
			A[j][i]=A[i][j];

}


void SisLin::adicione(int i, int j, double val)
{
	A[i][j] += val;
}

void SisLin::zere_matriz()
{
	for(int i=0; i<n; i++)
		for(int j=0; j<=n; j++)
			A[i][j]=0.0;
}

// Eliminação gaussiana; se não houver solução única, ele retorna NULL.
// Os termos independentes devem estar já na última coluna da matriz A
//Ou seja, A[i][n] (já que começa de 0).


double *SisLin::egaussiana()
{
	double *troca_linha;
	double pivo;
	int maxix;
	for(int col=0; col<n-1; col++){
		pivo=A[col][col];
		maxix=col;
		for(int k=col+1; k<n; k++)
			if (fabs(A[k][col])>pivo){
				pivo = A[k][col];
				maxix = k;
			}
		if (fabs(pivo)<tol) return NULL;
		if (maxix!=col){
			troca_linha=A[col];
			A[col] = A[maxix];
			A[maxix] = troca_linha;
		}
		for(int i=col+1; i<n; i++){
			for(int j=col+1; j<=n; j++)
				A[i][j] = A[i][j] - (A[col][j]*A[i][col])/A[col][col];
			A[i][col]=0.0;
		}
	}
	if (fabs(A[n-1][n-1])<tol) return NULL;
	double *ret;
	ret=(double *)calloc(n, sizeof(double));
	ret[n-1] =  A[n-1][n]/A[n-1][n-1];
	for(int i=n-2; i>=0; i--) {
		double soma = 0.0;
		for(int j=i+1; j<n; j++)
			soma = soma + A[i][j]*ret[j];
		ret[i] = (A[i][n] - soma)/A[i][i];
	}
	return(ret);
}
