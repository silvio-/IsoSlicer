//---------------------------------------------------------------------------
#include <vcl.h>
#ifndef uCurvaNurbsH
#define uCurvaNurbsH
#include "uMat.h"
#include "uCamera.h"
#include "uCurvaBezier.h"
//---------------------------------------------------------------------------
class cnurbs{
	public:
	int n; //grau dos segmentos de curva
	int K;  //número de nós sem multiplicidades
	int Kr; //número de nós com multiplicidades
	int L; //quantidade de intervalos úteis (sem multiplicidades)
	int c; //número de pontos de controle: Vale: k+1=n+c
	Ponto *d; //pontos de controle em coords homogêneas
	Ponto *_d; //pontos de controle auxiliares em coords homogêneas
	double *u; //vetor de nós da B-spline
	int *uix; //vetor de índices de nós, considerando as multiplicidades
	int *uuteis; //vetor dos índices dos intervalos de domínio (úteis) em uix, pelo ix do último múltiplo
	int *r; // vetor de multiplicidade dos nós
	void inicialize(int grau, int k, int kr); // inicializa estruturas internas
	void defina_intervalos_uteis(); // define os índices dos intervalos úteis
	void destrua(); // desaloca estruturas internas
	void Pcarregue(int i, Ponto P);
	void Ucarregue(int i, double U, int Mult);
	void _dcopie();
	Ponto  deBoor(int I, int r_, double U);
	void  oslo(cnurbs *esq, cnurbs *dir);
	void  oslo2(cnurbs *esq, cnurbs *dir);
	void desenhe(camera Cam, Graphics::TBitmap *Bitmap, double espacamento, byte R, byte G, byte B);
	void desenhe_poli(camera Cam, Graphics::TBitmap *Bitmap, byte R, byte G, byte B);
	cnurbs gere_nurbs_com_multiplicidade_do_noh_completa(int noh);
	cnurbs gere_nurbs_com_multiplicidade_do_ultimo_noh_completa();
	cnurbs gere_nurbs_com_multiplicidade_do_primeiro_noh_completa();
	cbezier *ToBezier();

};
#endif
