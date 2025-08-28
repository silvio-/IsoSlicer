//---------------------------------------------------------------------------
#include <vcl.h>
#ifndef uCameraH
#define uCameraH
#include "uMat.h"
//---------------------------------------------------------------------------
class camera{

 public:

	 ponto C;
	 vetor U,V,N;
	 float d,hx,hy;
	 int largura, altura;

	 float **zbuffer;

	camera(ponto Co, ponto Mira, vetor Up, float dd, float hhx, float hhy, int larg, int alt);
	camera(int larg, int alt);
	void atualize(ponto Co, ponto Mira, vetor Up, float dd, float hhx, float hhy);
	ponto coordsvista(ponto P);
	vetor coordsvista(vetor V);
	ponto perspectiva(ponto Pv);
	void inicialize_zbuffer();
	void desaloque_zbuffer();

};

class Luz_objeto{
	public:
	vetor Ia, Ip, Od, It;
	ponto Pl;
	float Ka, Kd, Ks;
	void carregue(vetor Iamb, vetor Iluz, vetor od, ponto Pluz, float ka, float kd, float ks);
	vetor equacao_iluminacao(vetor N, ponto P);
	void Phong(camera Camera1, int i1, int j1, int i2, int j2, int i3, int j3, int Npts, int Mpts, ponto P1, ponto P2, ponto P3, vetor N1,
					   vetor N2, vetor N3, vetor N,	float *Rxmin, float *Rxmax, float deltaMin, float deltaMax,
					   int ymin, int ymax, float t_numerador, ponto p1, ponto p2, ponto p3, float Kt, bool wireframe,bool trim,
					   TBitmap *Bitmap, TBitmap *Textura);
	void determina_lado_visivel();
	void varredura(camera Camera1, int i1, int j1, int i2, int j2, int i3, int j3, int Npts, int Mpts,
							ponto P1, ponto P2, ponto P3, vetor N1, vetor N2, vetor N3, vetor N, float Kt,
							bool wireframe,bool trim,TBitmap *Bitmap,TBitmap *Textura);

};



#endif

