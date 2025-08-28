//---------------------------------------------------------------------------

#pragma hdrstop
#include <stdlib.h>
#include "uCurvaBezier.h"
#include "uImagem.h"
#include <string>


//---------------------------------------------------------------------------
#pragma package(smart_init)

void cbezier::inicialize(int grau)
{
	n = grau;
	b =  (Ponto *) calloc (n+1, sizeof(Ponto));
	_b = (Ponto *) calloc (n+1, sizeof(Ponto));
}

void cbezier::destrua()
{
	delete(b);
    delete(_b);
}

void cbezier::carregue(int i, Ponto P)
{
	b[i] = P;
}

void cbezier::_bcopie()
{
	for(int i=0; i<=n; i++)
		_b[i] = b[i];
}

Ponto lerp(float t, Ponto A, Ponto B)
{
     Ponto P((1-t)*A.x + t*B.x, (1-t)*A.y + t*B.y,
			(1-t)*A.z + t*B.z, (1-t)*A.w + t*B.w);
	return(P);
}

Ponto cbezier::deCasteljau(float t)
{
	_bcopie();
	for(int r=1; r<=n; r++)
		for(int i=0; i<=n-r; i++){
		   _b[i] = lerp(t, _b[i], _b[i+1]);
		}
	return(_b[0]);
}

//subdivide a curva no valor t. As curvas de retorno devem ser alocadas
//e desalocadas fora.
void cbezier::subdivida(float t, cbezier *bez1, cbezier *bez2)
{
	_bcopie();
	bez1->b[0] = b[0];
	bez2->b[n] = b[n];
	for(int r=1; r<=n; r++){
		for(int i=0; i<=n-r; i++){
		   _b[i] = lerp(t, _b[i], _b[i+1]);
		}
		bez1->b[r]=_b[0];
        bez2->b[n-r]=_b[n-r];
	}
}

typedef struct pilha{
	cbezier *bez;
	float ti,tf;
	pilha *embaixo;
}Pilha;

pilha *topo;

void empilha(cbezier *bezier, float ti, float tf)
{
	pilha *novo;

	novo = (pilha *)calloc(1,sizeof(pilha));
	novo->ti = ti;
	novo->tf = tf;
	novo->bez = bezier;
	novo->embaixo = topo;
	topo = novo;
}

cbezier *desempilha(float *ti, float *tf)
{
	pilha *puxa;
	cbezier *bezier;
	if (topo==NULL) return(NULL);
	puxa = topo;
	*ti = topo->ti;
    *tf = topo->tf;
	topo = topo->embaixo;
	bezier = puxa->bez;
	delete(puxa);
	return(bezier);
}

bool pilha_vazia()
{
    return(topo==NULL);
}

typedef struct {
	ponto Pmin, Pmax;
	float deltax, deltay, deltaz;
}BB;

BB extraia_bounding_box(cbezier bezier)
{
	BB BB1;
	ponto P;

	BB1.Pmin.x = BB1.Pmin.y = BB1.Pmin.z = 100000;
	BB1.Pmax.x = BB1.Pmax.y = BB1.Pmax.z = -100000;
	for(int i=0; i<=bezier.n; i++){
		P= bezier.b[i].projete();

		if (P.x<BB1.Pmin.x)  BB1.Pmin.x=P.x;
		if (P.y<BB1.Pmin.y)  BB1.Pmin.y=P.y;
		if (P.z<BB1.Pmin.z)  BB1.Pmin.z=P.z;

		if (P.x>BB1.Pmax.x)  BB1.Pmax.x=P.x;
		if (P.y>BB1.Pmax.y)  BB1.Pmax.y=P.y;
		if (P.z>BB1.Pmax.z)  BB1.Pmax.z=P.z;

	}
	BB1.deltax = BB1.Pmax.x - BB1.Pmin.x;
	BB1.deltay = BB1.Pmax.y - BB1.Pmin.y;
	BB1.deltaz = BB1.Pmax.z - BB1.Pmin.z;
	return(BB1);

}




void insere_na_plist(ponto p, float t, float uval)
{
	if (pini==NULL) {
		pini = (ptlist *)calloc(1,sizeof(ptlist));
		pini->prox = NULL;
		pini->P = p;
		pini->t = t;
		pini->uval = uval;
		pfin=pini;
	}
	else {
		ptlist *pnovo;
		pnovo = (ptlist *)calloc(1,sizeof(ptlist));
		pnovo->prox = pini;
		pnovo->t = t;
		pnovo->uval = uval;
		pini = pnovo;
		pini->P = p;
	}
}

void   inicialize_ptlist()
{
	pini=NULL;
	pfin=NULL;
}

ptlist *cbezier::intersecte_com_plano_Z_igual_a(float Z, float uval)
{

	cbezier *bez,*besq, *bdir;
	float ti, tf;
	BB BBox;

	inicialize_ptlist();

	empilha(this,0.0,1.0);
	while (!pilha_vazia()){
		bez = desempilha(&ti, &tf);
		besq = (cbezier *)calloc(1,sizeof(cbezier));
		bdir = (cbezier *)calloc(1,sizeof(cbezier));
		besq->inicialize(bez->n);
		bdir->inicialize(bez->n);
		bez->subdivida(0.5, besq, bdir);
		BBox = extraia_bounding_box(*besq);
		if ((BBox.Pmin.z<=Z)&&(BBox.Pmax.z>=Z)) {
			if ((BBox.deltax<BBTOL)&&(BBox.deltay<BBTOL)&&(BBox.deltaz<BBTOL)) {
				ponto p;
				p.carregue(0.5*(BBox.Pmax.x+BBox.Pmin.x),0.5*(BBox.Pmax.y+BBox.Pmin.y),Z);
				insere_na_plist(p,0.5*(ti+tf),uval);
			}
			else  empilha(besq,ti,0.5*(ti+tf));
		}
		else {
			besq->destrua();
			delete(besq);
		}
		BBox = extraia_bounding_box(*bdir);
		if ((BBox.Pmin.z<=Z)&&(BBox.Pmax.z>=Z)) {
			if ((BBox.deltax<BBTOL)&&(BBox.deltay<BBTOL)&&(BBox.deltaz<BBTOL)) {
				ponto p;
				p.carregue(0.5*(BBox.Pmax.x+BBox.Pmin.x),0.5*(BBox.Pmax.y+BBox.Pmin.y),Z);
				insere_na_plist(p,0.5*(ti+tf),uval);
			}
			else  empilha(bdir,0.5*(ti+tf),tf);
		}
		else {
			bdir->destrua();
			delete(bdir);
		}

	}
	return(pini);
}



ptlist *cbezier::intersecte_com_plano_Z_igual_a(float Z, float uval, int *npilhas)
{

	cbezier *bez,*besq, *bdir;
	float ti, tf;
	BB BBox;

	(*npilhas)=1;

	inicialize_ptlist();

	empilha(this,0.0,1.0);
	while (!pilha_vazia()){
		bez = desempilha(&ti, &tf);
		besq = (cbezier *)calloc(1,sizeof(cbezier));
		bdir = (cbezier *)calloc(1,sizeof(cbezier));
		besq->inicialize(bez->n);
		bdir->inicialize(bez->n);
		bez->subdivida(0.5, besq, bdir);
		BBox = extraia_bounding_box(*besq);
		if ((BBox.Pmin.z<Z)&&(BBox.Pmax.z>Z)) {
			if ((BBox.deltax<BBTOL)&&(BBox.deltay<BBTOL)&&(BBox.deltaz<BBTOL)) {
				ponto p;
				p.carregue(0.5*(BBox.Pmax.x+BBox.Pmin.x),0.5*(BBox.Pmax.y+BBox.Pmin.y),Z);
				insere_na_plist(p,0.5*(ti+tf),uval);
			}
			else  {
				empilha(besq,ti,0.5*(ti+tf));
				(*npilhas)++;
			}
		}
		else {
			besq->destrua();
			delete(besq);
		}
		BBox = extraia_bounding_box(*bdir);
		if ((BBox.Pmin.z<Z)&&(BBox.Pmax.z>Z)) {
			if ((BBox.deltax<BBTOL)&&(BBox.deltay<BBTOL)&&(BBox.deltaz<BBTOL)) {
				ponto p;
				p.carregue(0.5*(BBox.Pmax.x+BBox.Pmin.x),0.5*(BBox.Pmax.y+BBox.Pmin.y),Z);
				insere_na_plist(p,0.5*(ti+tf),uval);
			}
			else  {
				empilha(bdir,0.5*(ti+tf),tf);
				(*npilhas)++;
			}
		}
		else {
			bdir->destrua();
			delete(bdir);
		}

	}
	return(pini);
}

ptlist *cbezier::retorne_fim_da_lista()
{
	return(pfin);
}


void cbezier::desenhe(camera Cam, Graphics::TBitmap *Bitmap, int namostras, byte R, byte G, byte B)
{
	Bitmap->Canvas->Pen->Width = 1;
	Bitmap->Canvas->Pen->Color= (TColor) RGB (R, G, B);


	float t;
	Ponto P(0.0,0.0,0.0,1.0);
	ponto p;


	for(int j=0; j<=namostras; j++){
		t = j / (float) namostras;
		P=deCasteljau(t);
		p=P.projete();
		p=Cam.coordsvista(p);
		p=Cam.perspectiva(p);
		paratela(&p, Bitmap);
		if (j) 	Bitmap->Canvas->LineTo((int)p.x, (int)p.y);
		else  Bitmap->Canvas->MoveTo((int)p.x, (int)p.y);
	}


}

void cbezier::desenhe_poli(camera Cam, Graphics::TBitmap *Bitmap, int namostras, byte R, byte G, byte B)
{
	Bitmap->Canvas->Pen->Width = 2;
	Bitmap->Canvas->Pen->Color= (TColor) RGB (R, G, B);

	ponto p;
	for(int i=0; i<=n; i++) {
		p=b[i].projete();
		p=Cam.coordsvista(p);
		p=Cam.perspectiva(p);
		paratela(&p, Bitmap);
		if (i) 	Bitmap->Canvas->LineTo((int)p.x, (int)p.y);
		else  Bitmap->Canvas->MoveTo((int)p.x, (int)p.y);
		Bitmap->Canvas->Pen->Color= (TColor) RGB (0, 0, 255);
		Bitmap->Canvas->Rectangle((int)(p.x-3),(int)(p.y-3), (int)(p.x+3),(int)(p.y+3));
		Bitmap->Canvas->Pen->Color= (TColor) RGB (255, 0, 0);
		Bitmap->Canvas->Rectangle((int)(p.x-2),(int)(p.y-2), (int)(p.x+2),(int)(p.y+2));
		Bitmap->Canvas->Pen->Color= (TColor) RGB (R, G, B);
	}

}


void cbezier::avalie_e_desenhe_1_pt(float t, camera Cam, Graphics::TBitmap *Bitmap, byte R, byte G, byte B)
{
	ponto p1;
	Ponto P(0.0,0.0,0.0,1.0);

	Bitmap->Canvas->Pen->Width = 2;
	Bitmap->Canvas->Pen->Color= (TColor) RGB (R, G, B);
	P=deCasteljau(t);
	p1=P.projete();
	p1=Cam.coordsvista(p1);
	p1=Cam.perspectiva(p1);
	paratela(&p1, Bitmap);
	Bitmap->Canvas->Pen->Color= (TColor) RGB (0, 0, 0);
	Bitmap->Canvas->Rectangle((int)(p1.x-3),(int)(p1.y-3), (int)(p1.x+3),(int)(p1.y+3));
	Bitmap->Canvas->Pen->Color= (TColor) RGB (R, G, B);
	Bitmap->Canvas->Rectangle((int)(p1.x-2),(int)(p1.y-2), (int)(p1.x+2),(int)(p1.y+2));

}
