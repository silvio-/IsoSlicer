//---------------------------------------------------------------------------

#pragma hdrstop
#include <chrono>

#include "uSupNurbs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#define TOL_FINA 0.000001

void snurbs::inicialize(int grauu, int grauv, int ku, int kv, int kur, int kvr)
{
	n=grauu;
	m=grauv;
	Ku=ku;
	Kv=kv;
	Kur=kur;
	Kvr=kvr;
	cu=Kur+1-n;
	cv=Kvr+1-m;
	d=(Ponto **) calloc (cu, sizeof(Ponto *));
	for(int i=0; i<cu; i++)
		d[i]=(Ponto *) calloc (cv, sizeof(Ponto));
	u = (double *) calloc (Ku, sizeof(double));
	v = (double *) calloc (Kv, sizeof(double));
	ru = (int *) calloc (Ku, sizeof(int));
	rv = (int *) calloc (Kv, sizeof(int));
	uix=(int *) calloc (Kur, sizeof(int));
	vix=(int *) calloc (Kvr, sizeof(int));
}


void snurbs::Pcarregue(int i, int j, Ponto P)
{
	d[i][j] = P;
}

void snurbs::Ucarregue(int i, double U, int Mult)
{
	u[i] = U;
	ru[i] = (Mult>n)?n:Mult;
}

void snurbs::Vcarregue(int i, double V, int Mult)
{
	v[i] = V;
	rv[i] = (Mult>m)?m:Mult;
}



//necessita que as multiplicidades estejam carregadas
void snurbs::defina_intervalos_uteis()
{
	int rix=0;
	for(int i=0; i<Ku; i++)
		for(int j=1; j<=ru[i]; j++)
			uix[rix++]=i;

	Lu=uix[Kur-n-1]-uix[n-1]+1;
	uuteis=(int *) calloc (Lu, sizeof(int));
	uuteis[0]=n-1;
	uuteis[Lu-1]=Kur-n-1;
	int j=n;
	for(int i=1; i<Lu-1; i++) {
		j += ru[uix[j]];
		uuteis[i]=j-1;
	}

	rix=0;
	for(int i=0; i<Kv; i++)
		for(int j=1; j<=rv[i]; j++)
			vix[rix++]=i;

	Lv=vix[Kvr-m-1]-vix[m-1]+1;
	vuteis=(int *) calloc (Lv, sizeof(int));
	vuteis[0]=m-1;
	vuteis[Lv-1]=Kvr-m-1;
	j=m;
	for(int i=1; i<Lv-1; i++) {
		j += rv[vix[j]];
		vuteis[i]=j-1;
	}
}

snurbs snurbs::clone()
{
	snurbs RNurbs;

	RNurbs.inicialize( n,  m,  Ku,  Kv,  Kur,  Kvr);
	for(int i=0; i<cu; i++)
		for(int j=0; j<cv; j++)
			RNurbs.d[i][j] =  d[i][j];
	for(int i=0; i<Ku; i++)
		RNurbs.u[i] = u[i];
	for(int i=0; i<Kv; i++)
		RNurbs.v[i] = v[i];
	for(int i=0; i<Ku; i++)
		RNurbs.ru[i] = ru[i];
	for(int i=0; i<Kv; i++)
		RNurbs.rv[i] = rv[i];
	for(int i=0; i<Kur; i++)
		RNurbs.uix[i] = uix[i];
	for(int i=0; i<Kvr; i++)
		RNurbs.vix[i] = vix[i];
	RNurbs.defina_intervalos_uteis();
	return(RNurbs);
}

void snurbs::destrua()
{
	for(int i=0; i<cu; i++)
		delete(d[i]);
	delete(d);
	delete(u);
	delete(ru);
	delete(rv);
	delete(uix);
	delete(vix);
	delete(uuteis);
	delete(vuteis);
}

void snurbs::encontre_min_max_Z_malha(double *minZ, double *maxZ)
{
	Ponto P(0.0,0.0,0.0,1.0);
	ponto p;

	*minZ=100000;
	*maxZ=(-100000);

	for(int i=0; i<cu; i++)
		for(int j=0; j<cv; j++){
			P = d[i][j];
			p=P.projete();
			if (*minZ>p.z) *minZ=p.z;
			if (*maxZ<p.z) *maxZ=p.z;
		}
}


void snurbs::desenhemalha(camera Cam, Graphics::TBitmap *Bitmap, byte R, byte G, byte B)
{
	Bitmap->Canvas->Pen->Width = 2;
	Bitmap->Canvas->Pen->Color= (TColor) RGB (R, G, B);
	Ponto P(0.0,0.0,0.0,1.0);
	ponto p;

	for(int i=0; i<cu; i++)
		for(int j=0; j<cv; j++){
			P = d[i][j];
			p=P.projete();
			p=Cam.coordsvista(p);
			p=Cam.perspectiva(p);
			paratela(&p, Bitmap);
			if (j==0) 	Bitmap->Canvas->MoveTo((int)p.x, (int)p.y);
			else		Bitmap->Canvas->LineTo((int)p.x, (int)p.y);
		}
	Bitmap->Canvas->Pen->Color= (TColor) RGB (B, R, G);
	for(int j=0; j<cv; j++)
		for(int i=0; i<cu; i++){
			P = d[i][j];
			p=P.projete();
			p=Cam.coordsvista(p);
			p=Cam.perspectiva(p);
			paratela(&p, Bitmap);
			if (i==0) 	Bitmap->Canvas->MoveTo((int)p.x, (int)p.y);
			else		Bitmap->Canvas->LineTo((int)p.x, (int)p.y);
		}

}

void snurbs::encontre_min_max_Z(double *minZ, double *maxZ)
{
	cnurbs isonurbs_u,isonurbs_v;
	Ponto P(0.0,0.0,0.0,1.0);
	ponto p;

	*minZ=100000;
	*maxZ=(-100000);

	isonurbs_u.inicialize(n,Ku,Kur);
	isonurbs_v.inicialize(m,Kv,Kvr);
	for(int i=0; i<Ku; i++)
		isonurbs_u.Ucarregue(i,u[i],ru[i]);
	for(int i=0; i<Kv; i++)
		isonurbs_v.Ucarregue(i,v[i],rv[i]);

	isonurbs_u.defina_intervalos_uteis();
	isonurbs_v.defina_intervalos_uteis();

	int r_;
	int I;
	double z1,z2;

	r_=ru[uix[uuteis[0]]];
	for(int k=0; k<cv; k++) {
		for(int ix=0; ix<cu; ix++)
			isonurbs_u.Pcarregue(ix,d[ix][k]);
		P=isonurbs_u.deBoor(uuteis[0],r_,u[uix[uuteis[0]]]);
		isonurbs_v.Pcarregue(k,P);
	}
	P=isonurbs_v.deBoor(vuteis[0],rv[vix[vuteis[0]]],v[vix[vuteis[0]]]);
	p=P.projete();
	if (*minZ>p.z) *minZ=p.z;
	if (*maxZ<p.z) *maxZ=p.z;
	int ixv,tixv;
	if (Kvr==2*m) {
		ixv = vix[vuteis[0]]+1;
		tixv = 0;
	}
	else {
		ixv = vix[vuteis[Lv-1]];
		tixv = Lv-2;
	}
	P=isonurbs_v.deBoor(vuteis[tixv]+1,rv[ixv],v[ixv]);
	p=P.projete();
	if (*minZ>p.z) *minZ=p.z;
	if (*maxZ<p.z) *maxZ=p.z;



	int ixu,tixu;
	if (Kur==2*n) {
		ixu = uix[uuteis[0]]+1;
		tixu = 0;
	}
	else {
		ixu = uix[uuteis[Lu-1]];
		tixu = Lu-2;
	}
	for(int k=0; k<cv; k++) {
		for(int ix=0; ix<cu; ix++)
			isonurbs_u.Pcarregue(ix,d[ix][k]);
		P=isonurbs_u.deBoor(uuteis[tixu]+1,ru[ixu],u[ixu]);
		isonurbs_v.Pcarregue(k,P);
	}
	P=isonurbs_v.deBoor(vuteis[0],rv[vix[vuteis[0]]],v[vix[vuteis[0]]]);
	p=P.projete();
	if (*minZ>p.z) *minZ=p.z;
	if (*maxZ<p.z) *maxZ=p.z;

	if (Kvr==2*m) {
		ixv = vix[vuteis[0]]+1;
		tixv = 0;
	}
	else {
		ixv = vix[vuteis[Lv-1]];
		tixv = Lv-2;
	}
	P=isonurbs_v.deBoor(vuteis[tixv]+1,rv[ixv],v[ixv]);
	p=P.projete();
	if (*minZ>p.z) *minZ=p.z;
	if (*maxZ<p.z) *maxZ=p.z;

	isonurbs_u.destrua();
	isonurbs_v.destrua();
}

void snurbs::desenheisoparametricas(camera Cam, Graphics::TBitmap *Bitmap, double espacamento, byte R, byte G, byte B)
{
	Bitmap->Canvas->Pen->Width = 1;
	Bitmap->Canvas->Pen->Color= (TColor) RGB (R, G, B);


	Ponto P(0.0,0.0,0.0,1.0);
	ponto p;
	bool prime=true;
	cnurbs isonurbs_u,isonurbs_v;

	isonurbs_u.inicialize(n,Ku,Kur);
	isonurbs_v.inicialize(m,Kv,Kvr);
	for(int i=0; i<Ku; i++)
		isonurbs_u.Ucarregue(i,u[i],ru[i]);
	for(int i=0; i<Kv; i++)
		isonurbs_v.Ucarregue(i,v[i],rv[i]);

	isonurbs_u.defina_intervalos_uteis();
	isonurbs_v.defina_intervalos_uteis();

	int namostras;
	double U, deltau;
	int r_;
	for(int i=0; i<Lu; i++) {
		namostras = floor( ( u[uix[uuteis[i]+1]] - u[uix[uuteis[i]]] )/espacamento);
		deltau = ( u[uix[uuteis[i]+1]] - u[uix[uuteis[i]]] )/ (double) namostras;
		for(int j=0; j<=namostras; j++){
			r_=0;
			if (j==0)  r_=ru[uix[uuteis[i]]];
			U =  u[uix[uuteis[i]]] + j*deltau;
			for(int k=0; k<cv; k++) {
				for(int ix=0; ix<cu; ix++)
					isonurbs_u.Pcarregue(ix,d[ix][k]);
				P=isonurbs_u.deBoor(uuteis[i],r_,U);
				isonurbs_v.Pcarregue(k,P);
			}
			isonurbs_v.desenhe( Cam, Bitmap, espacamento, R, G, B);
		}
	}
	double V, deltav;
	for(int i=0; i<Lv; i++) {
		namostras = floor( ( v[vix[vuteis[i]+1]] - v[vix[vuteis[i]]] )/espacamento);
		deltav = ( v[vix[vuteis[i]+1]] - v[vix[vuteis[i]]] )/ (double) namostras;
		for(int j=0; j<=namostras; j++){
			r_=0;
			if (j==0)  r_=rv[vix[vuteis[i]]];
			V =  v[vix[vuteis[i]]] + j*deltav;
			for(int k=0; k<cu; k++) {
				for(int ix=0; ix<cv; ix++)
					isonurbs_v.Pcarregue(ix,d[k][ix]);
				P=isonurbs_v.deBoor(vuteis[i],r_,V);
				isonurbs_u.Pcarregue(k,P);
			}
			isonurbs_u.desenhe( Cam, Bitmap, espacamento, R, G, B);
		}
	}

	isonurbs_u.destrua();
	isonurbs_v.destrua();
}


void snurbs::desenhe_1_isoparametrica(camera Cam, Graphics::TBitmap *Bitmap, bool compoli, int var, double valor, double espacamento, byte R, byte G, byte B)
{

	Ponto P(0.0,0.0,0.0,1.0);
	ponto p;
	bool prime=true;
	cnurbs isonurbs_u,isonurbs_v;

	isonurbs_u.inicialize(n,Ku,Kur);
	isonurbs_v.inicialize(m,Kv,Kvr);
	for(int i=0; i<Ku; i++)
		isonurbs_u.Ucarregue(i,u[i],ru[i]);
	for(int i=0; i<Kv; i++)
		isonurbs_v.Ucarregue(i,v[i],rv[i]);

	isonurbs_u.defina_intervalos_uteis();
	isonurbs_v.defina_intervalos_uteis();

	int namostras;
	int r_;
	int I;
	if (var==0){
		double U=valor;
		for(I=0; I<Lu-1; I++)
			if ((U>=u[uix[uuteis[I]]])&&(U<u[uix[uuteis[I+1]]])) break;
		r_=0;
		if (fabs(U-u[uix[uuteis[I]]])<TOL_FINA) r_=ru[uix[uuteis[I]]];

		for(int k=0; k<cv; k++) {
			for(int ix=0; ix<cu; ix++)
				isonurbs_u.Pcarregue(ix,d[ix][k]);
			P=isonurbs_u.deBoor(uuteis[I],r_,U);
			isonurbs_v.Pcarregue(k,P);
		}
		isonurbs_v.desenhe( Cam, Bitmap, espacamento, R, G, B);
		if (compoli)   isonurbs_v.desenhe_poli(Cam, Bitmap , 60,200,255);

		cnurbs esq, dir;
		isonurbs_v.oslo2(&esq, &dir);
		esq.desenhe( Cam, Bitmap, espacamento, 10, 15, 15);
		if (compoli)  esq.desenhe_poli(Cam, Bitmap , 200,200,15);
		esq.destrua();
		dir.desenhe( Cam, Bitmap, espacamento, 255, 10, 10);
		if (compoli)  dir.desenhe_poli(Cam, Bitmap , 60,200,120);
		dir.destrua();

	}
	else{
		double V=valor;
		for(I=0; I<Lv-1; I++)
			if ((V>=v[vix[vuteis[I]]])&&(V<v[vix[vuteis[I+1]]])) break;
		r_=0;
		if (fabs(V-v[vix[vuteis[I]]])<TOL_FINA) r_=rv[vix[vuteis[I]]];

		for(int k=0; k<cu; k++) {
			for(int ix=0; ix<cv; ix++)
				isonurbs_v.Pcarregue(ix,d[k][ix]);
			P=isonurbs_v.deBoor(vuteis[I],r_,V);
			isonurbs_u.Pcarregue(k,P);
		}
		isonurbs_u.desenhe( Cam, Bitmap, espacamento, R, G, B);
		if (compoli)   isonurbs_u.desenhe_poli(Cam, Bitmap , 60,200,255);
	}



	isonurbs_u.destrua();
	isonurbs_v.destrua();
}


void triangulos::inicialize(int Npts, int Mpts)
{
	npts=Npts;
	mpts=Mpts;
	Pt=(ponto **) calloc (npts, sizeof(ponto *));
	for(int i=0; i<npts; i++)
		Pt[i]=(ponto *) calloc (mpts, sizeof(ponto));

	pnormais=(vetor **) calloc (npts, sizeof(vetor *));
	for(int i=0; i<npts; i++)
		pnormais[i]=(vetor *) calloc (mpts, sizeof(vetor));

	t1normais=(vetor **) calloc (npts-1, sizeof(vetor *));
	for(int i=0; i<npts-1; i++)
		t1normais[i]=(vetor *) calloc (mpts-1, sizeof(vetor));

	t2normais=(vetor **) calloc (npts-1, sizeof(vetor *));
	for(int i=0; i<npts-1; i++)
		t2normais[i]=(vetor *) calloc (mpts-1, sizeof(vetor));

}


void triangulos::desaloque()
{
	for(int i=0; i<npts; i++)
		delete(Pt[i]);
	delete(Pt);

	for(int i=0; i<npts; i++)
		delete(pnormais[i]);
	delete(pnormais);

	for(int i=0; i<npts-1; i++)
		delete(t1normais[i]);
	delete(t1normais);

	for(int i=0; i<npts-1; i++)
		delete(t2normais[i]);
	delete(t2normais);

}

void  triangulos::Pcarregue(int ix, int jx, ponto P)
{
	Pt[ix][jx]=P;
}


void  triangulos::Ncarregue(bool t1, int ix, int jx, vetor v)
{
	if (t1)
		t1normais[ix][jx]=v;
	else
		t2normais[ix][jx]=v;
}


void  triangulos::compute_normais(int Npts, int Mpts)
{
	vetor v1, v2, N;

	for(int i=1; i<Npts; i++)
		for(int j=1; j<Mpts; j++) {
			v1.carregue(Pt[i-1][j].x - Pt[i][j].x,
						Pt[i-1][j].y - Pt[i][j].y,
						Pt[i-1][j].z - Pt[i][j].z);
			v2.carregue(Pt[i-1][j-1].x - Pt[i][j].x,
						Pt[i-1][j-1].y - Pt[i][j].y,
						Pt[i-1][j-1].z - Pt[i][j].z);
			N = v1.X(v2);
			N.normalize();
			Ncarregue(true, i-1, j-1, N);
			v1.carregue(Pt[i][j-1].x - Pt[i][j].x,
						Pt[i][j-1].y - Pt[i][j].y,
						Pt[i][j-1].z - Pt[i][j].z);
			N = v2.X(v1);
			N.normalize();
			Ncarregue(false, i-1, j-1, N);

		}
	for(int i=0; i<Npts; i++)
		for(int j=0; j<Mpts; j++)
			pnormais[i][j].carregue(0.0,0.0,0.0);

	for(int i=1; i<Npts; i++)
		for(int j=1; j<Mpts; j++) {
			pnormais[i][j].x += t1normais[i-1][j-1].x;
			pnormais[i][j].y += t1normais[i-1][j-1].y;
			pnormais[i][j].z += t1normais[i-1][j-1].z;

			pnormais[i-1][j-1].x += t1normais[i-1][j-1].x;
			pnormais[i-1][j-1].y += t1normais[i-1][j-1].y;
			pnormais[i-1][j-1].z += t1normais[i-1][j-1].z;

			pnormais[i-1][j].x += t1normais[i-1][j-1].x;
			pnormais[i-1][j].y += t1normais[i-1][j-1].y;
			pnormais[i-1][j].z += t1normais[i-1][j-1].z;

			pnormais[i][j].x += t2normais[i-1][j-1].x;
			pnormais[i][j].y += t2normais[i-1][j-1].y;
			pnormais[i][j].z += t2normais[i-1][j-1].z;

			pnormais[i-1][j-1].x += t2normais[i-1][j-1].x;
			pnormais[i-1][j-1].y += t2normais[i-1][j-1].y;
			pnormais[i-1][j-1].z += t2normais[i-1][j-1].z;

			pnormais[i][j-1].x += t2normais[i-1][j-1].x;
			pnormais[i][j-1].y += t2normais[i-1][j-1].y;
			pnormais[i][j-1].z += t2normais[i-1][j-1].z;
		}
	for(int i=0; i<Npts; i++)
		for(int j=0; j<Mpts; j++)
			pnormais[i][j].normalize();
}


void snurbs::pinte(Luz_objeto cena, camera Cam, bool wireframe,bool trim, double espacamento, TBitmap *Bitmap, TBitmap *Textura)
{
	triangulos triangs;
	int Npts=0, Mpts=0;

	for(int i=0; i<Lu; i++)
		Npts += ceil( ( u[uix[uuteis[i]+1]] - u[uix[uuteis[i]]] )/espacamento)+1;
	for(int i=0; i<Lv; i++)
		Mpts += ceil( ( v[vix[vuteis[i]+1]] - v[vix[vuteis[i]]] )/espacamento)+1;

	triangs.inicialize(Npts, Mpts);

	Ponto P(0.0,0.0,0.0,1.0);
	ponto p;
	bool prime=true;
	cnurbs isonurbs_u,isonurbs_v;

	isonurbs_u.inicialize(n,Ku,Kur);
	isonurbs_v.inicialize(m,Kv,Kvr);
	for(int i=0; i<Ku; i++)
		isonurbs_u.Ucarregue(i,u[i],ru[i]);
	for(int i=0; i<Kv; i++)
		isonurbs_v.Ucarregue(i,v[i],rv[i]);

	isonurbs_u.defina_intervalos_uteis();
	isonurbs_v.defina_intervalos_uteis();

	int namostras,mamostras;
	double U,V, deltau,deltav;
	int r_;
	int ix=0,jx=0;
	for(int i=0; i<Lu; i++) {
		namostras = floor( ( u[uix[uuteis[i]+1]] - u[uix[uuteis[i]]] )/espacamento);
		deltau = ( u[uix[uuteis[i]+1]] - u[uix[uuteis[i]]] )/ (double) namostras;
		int nlimite=(i==Lu-1)?namostras:namostras-1;
		for(int j=0; j<=nlimite; j++){
			r_=0;
			if (j==0)  r_=ru[uix[uuteis[i]]];
			U =  u[uix[uuteis[i]]] + j*deltau;
			if (j==namostras)
				U =  u[uix[uuteis[i]+1]]-TOL;
			for(int k=0; k<cv; k++) {
				for(int ix=0; ix<cu; ix++)
					isonurbs_u.Pcarregue(ix,d[ix][k]);
				P=isonurbs_u.deBoor(uuteis[i],r_,U);
				isonurbs_v.Pcarregue(k,P);
			}
			jx=0;
			for(int k=0; k<Lv; k++) {
				mamostras = floor(( v[vix[vuteis[k]+1]] - v[vix[vuteis[k]]] )/espacamento);
				deltav = ( v[vix[vuteis[k]+1]] - v[vix[vuteis[k]]] )/ (double) mamostras;
				int mlimite=(k==Lv-1)?mamostras:mamostras-1;
				for(int l=0; l<=mlimite; l++){
					V =  v[vix[vuteis[k]]] + l*deltav;
					r_=0;
					if (l==0)  r_=rv[vix[vuteis[k]]];
					if (l==mamostras)
						V= v[vix[vuteis[k]+1]]-TOL;
					P=isonurbs_v.deBoor(vuteis[k],r_,V);
					p=P.projete();
					p=Cam.coordsvista(p);
					triangs.Pcarregue(ix, jx, p);
					jx++;
				}
			}
			ix++;
		}
	}
	Npts=ix;
	Mpts=jx;


	triangs.compute_normais(Npts,Mpts);
	for(int i=1; i<Npts; i++) {
		for(int j=1; j<Mpts; j++) {
			cena.varredura(Cam, i,j,i-1,j,i-1,j-1, Npts, Mpts, triangs.Pt[i][j], triangs.Pt[i-1][j], triangs.Pt[i-1][j-1],
						   triangs.pnormais[i][j], triangs.pnormais[i-1][j],
						   triangs.pnormais[i-1][j-1], triangs.t1normais[i-1][j-1], 0, wireframe, trim, Bitmap,Textura);
			cena.varredura(Cam, i,j, i-1, j-1, i, j-1, Npts, Mpts, triangs.Pt[i][j], triangs.Pt[i-1][j-1], triangs.Pt[i][j-1],
						   triangs.pnormais[i][j], triangs.pnormais[i-1][j-1],
						   triangs.pnormais[i][j-1], triangs.t2normais[i-1][j-1], 0, wireframe, trim, Bitmap,Textura);
		}
	}
 	triangs.desaloque();
	isonurbs_u.destrua();
	isonurbs_v.destrua();
}


#define ZTOL 0.05
double snurbs::desenhe_intersecoes_com_plano(int *npts, float Z,int var, double espacamento, bool zbuf, camera Cam, Graphics::TBitmap *Bitmap, byte R, byte G, byte B)

{
	double Tempo=0;
	*npts=0;
	Bitmap->Canvas->Pen->Width = 1;
	Bitmap->Canvas->Pen->Color= (TColor) RGB (R, G, B);


	Ponto P(0.0,0.0,0.0,1.0);
	ponto p;
	bool prime=true;
	cnurbs isonurbs_u,isonurbs_v;

	isonurbs_u.inicialize(n,Ku,Kur);
	isonurbs_v.inicialize(m,Kv,Kvr);
	for(int i=0; i<Ku; i++)
		isonurbs_u.Ucarregue(i,u[i],ru[i]);
	for(int i=0; i<Kv; i++)
		isonurbs_v.Ucarregue(i,v[i],rv[i]);

	isonurbs_u.defina_intervalos_uteis();
	isonurbs_v.defina_intervalos_uteis();

	int namostras;
	int r_;
	ptlist *ptl,*PTin=NULL, *rtl;
	cbezier *bezier;

	if (var==0){
		double U, deltau;
		for(int i=0; i<Lu; i++) {
			namostras = floor( ( u[uix[uuteis[i]+1]] - u[uix[uuteis[i]]] )/espacamento);
			deltau = ( u[uix[uuteis[i]+1]] - u[uix[uuteis[i]]] )/ (double) namostras;
			for(int j=0; j<=namostras; j++){
				r_=0;
				if (j==0)  r_=ru[uix[uuteis[i]]];
				U =  u[uix[uuteis[i]]] + j*deltau;
				for(int k=0; k<cv; k++) {
					for(int ix=0; ix<cu; ix++)
						isonurbs_u.Pcarregue(ix,d[ix][k]);
					P=isonurbs_u.deBoor(uuteis[i],r_,U);
					isonurbs_v.Pcarregue(k,P);
				}
				bezier = isonurbs_v.ToBezier();
				auto start = std::chrono::steady_clock::now();
				for(int k=0; k< isonurbs_v.L; k++){
					if (PTin==NULL)
						PTin=bezier[k].intersecte_com_plano_Z_igual_a(Z,U);
					else{
						ptl=PTin;
						while (ptl->prox!=NULL)	ptl=ptl->prox;
						ptl->prox=bezier[k].intersecte_com_plano_Z_igual_a(Z,U);
					}
					bezier[k].destrua();
				}
				auto end = std::chrono::steady_clock::now();
				std::chrono::duration<double> elapsed_seconds = end-start;
				Tempo += elapsed_seconds.count()*1000;
				delete(bezier);
				ptl=PTin;
				//----------------------------
				while (ptl!=NULL) {
					ptl->P = Cam.coordsvista(ptl->P);
					*npts = *npts + 1;
					if (zbuf) {
						float z=ptl->P.z;
						ptl->P = Cam.perspectiva(ptl->P);
						paratela(&(ptl->P), Bitmap);
						if ( ((int)(ptl->P.x)<Cam.largura)&&((int)(ptl->P.x)>0)&&
							 ((int)(ptl->P.y)<Cam.altura) &&((int)(ptl->P.y)>0) ) {
							if ( (z < Cam.zbuffer[(int)(ptl->P.x)][(int)(ptl->P.y)] + ZTOL)&&( z > Cam.zbuffer[(int)(ptl->P.x)][(int)(ptl->P.y)] - ZTOL)){
								Bitmap->Canvas->Pixels[(int)ptl->P.x][(int)ptl->P.y]= (TColor)RGB (0, 255, 255);
								Bitmap->Canvas->Pixels[(int)(ptl->P.x-1)][(int)ptl->P.y]=(TColor)RGB (0, 00, 0);
								Bitmap->Canvas->Pixels[(int)(ptl->P.x+1)][(int)ptl->P.y]=(TColor)RGB (0, 00, 0);
								Bitmap->Canvas->Pixels[(int)ptl->P.x][(int)(ptl->P.y-1)]=(TColor)RGB (0, 00, 0);
								Bitmap->Canvas->Pixels[(int)ptl->P.x][(int)(ptl->P.y+1)]=(TColor)RGB (0, 00, 0);
							}
						}
					}
					else{
						ptl->P = Cam.perspectiva(ptl->P);
						paratela(&(ptl->P), Bitmap);
						if ( ((int)(ptl->P.x)<Cam.largura)&&((int)(ptl->P.x)>0)&&
							 ((int)(ptl->P.y)<Cam.altura) &&((int)(ptl->P.y)>0) ) {
							Bitmap->Canvas->Pen->Color= (TColor) RGB (0, 0, 0);
							Bitmap->Canvas->Ellipse((int)(ptl->P.x-3),(int)(ptl->P.y-3), (int)(ptl->P.x+3),(int)(ptl->P.y+3));
							Bitmap->Canvas->Pen->Color= (TColor) RGB (R, G, B);
							Bitmap->Canvas->Ellipse((int)(ptl->P.x-2),(int)(ptl->P.y-2), (int)(ptl->P.x+2),(int)(ptl->P.y+2));
						}
					}
					rtl = ptl;
					ptl = ptl->prox;
					delete(rtl);
				}
				PTin=NULL;
				//-------------------
			}
		}
	}
	else{
		double V, deltav;
		for(int i=0; i<Lv; i++) {
			namostras = floor( ( v[vix[vuteis[i]+1]] - v[vix[vuteis[i]]] )/espacamento);
			deltav = ( v[vix[vuteis[i]+1]] - v[vix[vuteis[i]]] )/ (double) namostras;
			for(int j=0; j<=namostras; j++){
				r_=0;
				if (j==0)  r_=rv[vix[vuteis[i]]];
				V =  v[vix[vuteis[i]]] + j*deltav;
				for(int k=0; k<cu; k++) {
					for(int ix=0; ix<cv; ix++)
						isonurbs_v.Pcarregue(ix,d[k][ix]);
					P=isonurbs_v.deBoor(vuteis[i],r_,V);
					isonurbs_u.Pcarregue(k,P);
				}
//----------------------------------------
				PTin=NULL;
				bezier = isonurbs_u.ToBezier();
				auto start = std::chrono::steady_clock::now();
				for(int k=0; k< isonurbs_v.L; k++){
					if (PTin==NULL)
						PTin=bezier[k].intersecte_com_plano_Z_igual_a(Z,V);
					else{
						ptl=PTin;
						while (ptl->prox!=NULL)	ptl=ptl->prox;
						ptl->prox=bezier[k].intersecte_com_plano_Z_igual_a(Z,V);
					}
					bezier[k].destrua();
				}
				auto end = std::chrono::steady_clock::now();
				std::chrono::duration<double> elapsed_seconds = end-start;
				Tempo += elapsed_seconds.count()*1000;
				delete(bezier);
				ptl=PTin;
				//----------------------------
				while (ptl!=NULL) {
					ptl->P = Cam.coordsvista(ptl->P);
					*npts = *npts + 1;
					if (zbuf) {
						float z=ptl->P.z;
						ptl->P = Cam.perspectiva(ptl->P);
						paratela(&(ptl->P), Bitmap);
						if ( ((int)(ptl->P.x)<Cam.largura)&&((int)(ptl->P.x)>0)&&
							 ((int)(ptl->P.y)<Cam.altura) &&((int)(ptl->P.y)>0) ) {
							if ( (z < Cam.zbuffer[(int)(ptl->P.x)][(int)(ptl->P.y)] + ZTOL)&&( z > Cam.zbuffer[(int)(ptl->P.x)][(int)(ptl->P.y)] - ZTOL)){
								Bitmap->Canvas->Pixels[(int)ptl->P.x][(int)ptl->P.y]= (TColor)RGB (0, 255, 255);
								Bitmap->Canvas->Pixels[(int)(ptl->P.x-1)][(int)ptl->P.y]=(TColor)RGB (0, 00, 0);
								Bitmap->Canvas->Pixels[(int)(ptl->P.x+1)][(int)ptl->P.y]=(TColor)RGB (0, 00, 0);
								Bitmap->Canvas->Pixels[(int)ptl->P.x][(int)(ptl->P.y-1)]=(TColor)RGB (0, 00, 0);
								Bitmap->Canvas->Pixels[(int)ptl->P.x][(int)(ptl->P.y+1)]=(TColor)RGB (0, 00, 0);
							}
						}
					}
					else{
						ptl->P = Cam.perspectiva(ptl->P);
						paratela(&(ptl->P), Bitmap);
						if ( ((int)(ptl->P.x)<Cam.largura)&&((int)(ptl->P.x)>0)&&
							 ((int)(ptl->P.y)<Cam.altura) &&((int)(ptl->P.y)>0) ) {
							Bitmap->Canvas->Pen->Color= (TColor) RGB (0, 0, 0);
							Bitmap->Canvas->Ellipse((int)(ptl->P.x-3),(int)(ptl->P.y-3), (int)(ptl->P.x+3),(int)(ptl->P.y+3));
							Bitmap->Canvas->Pen->Color= (TColor) RGB (R, G, B);
							Bitmap->Canvas->Ellipse((int)(ptl->P.x-2),(int)(ptl->P.y-2), (int)(ptl->P.x+2),(int)(ptl->P.y+2));
						}
					}
					rtl = ptl;
					ptl = ptl->prox;
					delete(rtl);
				}
				PTin=NULL;
				//-------------------
//--------------------------------
			}
		}
	}
	isonurbs_u.destrua();
	isonurbs_v.destrua();
	return(Tempo);
}

