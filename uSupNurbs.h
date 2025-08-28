//---------------------------------------------------------------------------

#ifndef uSupNurbsH
#define uSupNurbsH
#include "uMat.h"
#include "uCamera.h"
#include "uCurvaNurbs.h"
#include "uImagem.h"
//---------------------------------------------------------------------------

class snurbs{
	public:
	int n,m; //graus nas duas dire��es da NURBS
	int Ku,Kv;  //n�meros de n�s sem multiplicidades, nas duas dire��es da NURBS
	int Kur,Kvr; //n�meros de n�s com multiplicidades, nas duas dire��es da NURBS
	int Lu,Lv; //quantidade de intervalos �teis (sem multiplicidades), nas duas dire��es da NURBS
	int cu,cv; //n�meros de pontos de controle (vale: k+1=n+c) nas duas dire��es da NURBS
	Ponto **d; //pontos de controle em coords homog�neas da malha da NURBS
	double *u,*v; //vetores de n�s da NURBS
	int *ru,*rv; // vetor de multiplicidade dos n�s da NURBS
	int *uix, *vix; //vetores de �ndices de n�s, considerando as multiplicidades
	int *uuteis,*vuteis; //vetores dos �ndices dos intervalos de dom�nio (�teis) em uix e vix

	void inicialize(int grauu, int grauv, int ku, int kv, int kur, int kvr);
	void defina_intervalos_uteis();
	snurbs snurbs::clone();
	void destrua();
	void Ucarregue(int i, double U, int Mult);
	void Vcarregue(int i, double V, int Mult);
	void Pcarregue(int i, int j, Ponto P);
	void encontre_min_max_Z(double *minZ, double *maxZ);
    void encontre_min_max_Z_malha(double *minZ, double *maxZ);
	void desenhemalha(camera Cam, Graphics::TBitmap *Bitmap, byte R, byte G, byte B);
	void desenheisoparametricas(camera Cam, Graphics::TBitmap *Bitmap, double espacamento, byte R, byte G, byte B);
	void desenhe_1_isoparametrica(camera Cam, Graphics::TBitmap *Bitmap, bool compoli, int val, double valor,
									double espacamento, byte R, byte G, byte B);

	void pinte(Luz_objeto cena, camera Cam, bool wireframe, bool trim, double espacamento,  TBitmap *Bitmap,TBitmap *Textura);
	double desenhe_intersecoes_com_plano(int *npts, float Z,int var, double espacamento, bool zbuf, camera Cam, Graphics::TBitmap *Bitmap, byte R, byte G, byte B);

};


class triangulos{
	public:
	int npts,mpts;
	ponto **Pt;
	vetor **pnormais;
	vetor **t1normais;  //normais nos tri�ngulos da carreira 1
	vetor **t2normais;  //normais nos tri�ngulos da carreira 2

	void inicialize(int Npts, int Mpts);
	void desaloque();
	void Pcarregue(int ix, int jx, ponto P);
	void Ncarregue(bool t1, int ix, int jx, vetor v);
	void compute_normais(int Npts, int Mpts);
};
#endif
