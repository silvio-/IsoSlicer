	//---------------------------------------------------------------------------
#include <vcl.h>
#ifndef uCurvaBezierH
#define uCurvaBezierH
#include "uMat.h"
#include "uCamera.h"

//---------------------------------------------------------------------------

class cbezier {
public:
	int n; //grau da curva
	Ponto *b; //pontos de controle em coords homogêneas
	Ponto *_b; //pontos de controle auxiliares em coords homogêneas

	void inicialize(int grau); // inicializa estruturas internas
    void destrua(); // desaloca estruturas internas
	void carregue(int i, Ponto P);
	void _bcopie();
	Ponto deCasteljau(float t);
	void desenhe(camera Cam, Graphics::TBitmap *Bitmap, int namostras, byte R, byte G, byte B);
	void desenhe_poli(camera Cam, Graphics::TBitmap *Bitmap, int namostras, byte R, byte G, byte B);
	void avalie_e_desenhe_1_pt(float t, camera Cam, Graphics::TBitmap *Bitmap, byte R, byte G, byte B);
	void subdivida(float t, cbezier *bez1, cbezier *bez2);
	ptlist *intersecte_com_plano_Z_igual_a(float Z, float uval);
	ptlist *intersecte_com_plano_Z_igual_a(float Z, float uval, int *npilhas);
	ptlist *retorne_fim_da_lista();
};

ptlist *pini,*pfin;
void insere_na_plist(ponto p, float t, float uval);
void  inicialize_ptlist();
#endif
