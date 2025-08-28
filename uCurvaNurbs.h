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
	int K;  //n�mero de n�s sem multiplicidades
	int Kr; //n�mero de n�s com multiplicidades
	int L; //quantidade de intervalos �teis (sem multiplicidades)
	int c; //n�mero de pontos de controle: Vale: k+1=n+c
	Ponto *d; //pontos de controle em coords homog�neas
	Ponto *_d; //pontos de controle auxiliares em coords homog�neas
	double *u; //vetor de n�s da B-spline
	int *uix; //vetor de �ndices de n�s, considerando as multiplicidades
	int *uuteis; //vetor dos �ndices dos intervalos de dom�nio (�teis) em uix, pelo ix do �ltimo m�ltiplo
	int *r; // vetor de multiplicidade dos n�s
	void inicialize(int grau, int k, int kr); // inicializa estruturas internas
	void defina_intervalos_uteis(); // define os �ndices dos intervalos �teis
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
