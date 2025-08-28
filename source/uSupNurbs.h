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
	int n,m; //graus nas duas direções da NURBS
	int Ku,Kv;  //números de nós sem multiplicidades, nas duas direções da NURBS
	int Kur,Kvr; //números de nós com multiplicidades, nas duas direções da NURBS
	int Lu,Lv; //quantidade de intervalos úteis (sem multiplicidades), nas duas direções da NURBS
	int cu,cv; //números de pontos de controle (vale: k+1=n+c) nas duas direções da NURBS
	Ponto **d; //pontos de controle em coords homogêneas da malha da NURBS
	double *u,*v; //vetores de nós da NURBS
	int *ru,*rv; // vetor de multiplicidade dos nós da NURBS
	int *uix, *vix; //vetores de índices de nós, considerando as multiplicidades
	int *uuteis,*vuteis; //vetores dos índices dos intervalos de domínio (úteis) em uix e vix

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
	vetor **t1normais;  //normais nos triângulos da carreira 1
	vetor **t2normais;  //normais nos triângulos da carreira 2

	void inicialize(int Npts, int Mpts);
	void desaloque();
	void Pcarregue(int ix, int jx, ponto P);
	void Ncarregue(bool t1, int ix, int jx, vetor v);
	void compute_normais(int Npts, int Mpts);
};
#endif
