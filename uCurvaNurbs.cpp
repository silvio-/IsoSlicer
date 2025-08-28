//---------------------------------------------------------------------------

#pragma hdrstop

#include "uCurvaNurbs.h"
#include "uImagem.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define TOL 0.000001
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
	 _a > _b ? _a : _b; })
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
	 _a < _b ? _a : _b; })

void cnurbs::inicialize(int grau, int k, int kr)
{
	n = grau;
	K = k;
	Kr = kr;
	c = Kr+1-n;
	d =  (Ponto *) calloc (c, sizeof(Ponto));
	_d = (Ponto *) calloc (c, sizeof(Ponto));
	u = (double *) calloc (K, sizeof(double));
	r = (int *) calloc (K, sizeof(int));
	uix=(int *) calloc (Kr, sizeof(int));
}

//necessita que as multiplicidades estejam carregadas
void cnurbs::defina_intervalos_uteis()
{
	int rix=0;
	for(int i=0; i<K; i++)
		for(int j=1; j<=r[i]; j++)
			uix[rix++]=i;

	L=uix[Kr-n-1]-uix[n-1]+1;
	uuteis=(int *) calloc (L, sizeof(int));
	uuteis[0]=n-1;
	uuteis[L-1]=Kr-n-1;
	int j=n;
	for(int i=1; i<L-1; i++) {
		j += r[uix[j]];
		uuteis[i]=j-1;
	}
}


void cnurbs::destrua()
{
	delete(d);
	delete(_d);
	delete(u);
	delete(r);
	delete(uix);
}

void cnurbs::Pcarregue(int i, Ponto P)
{
	d[i] = P;
}

void cnurbs::Ucarregue(int i, double U, int Mult)
{
	u[i] = U;
	r[i] = (Mult>n)?n:Mult;
}


void cnurbs::_dcopie()
{
	for(int i=0; i<c; i++)
		_d[i] = d[i];
}

Ponto lerp(double t, Ponto A, Ponto B)
{
     Ponto P((1-t)*A.x + t*B.x, (1-t)*A.y + t*B.y,
			(1-t)*A.z + t*B.z, (1-t)*A.w + t*B.w);
	return(P);
}

//Aplica DeBoor de acordo com Farin 4a edição
Ponto  cnurbs::deBoor(int I, int r_, double U)
{
	double t;

	_dcopie();

	for(int k=1; k<=n-r_; k++) {
		for(int i=I-n+k+1; i<=I-r_+1; i++){
			t=(U-u[uix[i-1]])/(u[uix[i+n-k]]-u[uix[i-1]]);
			_d[i-1] = lerp(t, _d[i-1], _d[i]);
		}
		for(int i=I-r_+1; i>=I-n+k+1; i--)
			_d[i] = _d[i-1];
	}

	return(_d[I-r_+1]);
}


//Aplica o Algoritmo de Oslo para fazer uma subdivisão. A subdivisão é
//feita tomando-se a média dos limites inferior e superior de todo o
//suporte útil.  A saída é o par de curvas resultante da subdivisão.
//Nota: O Alg. de Oslo é mais geral do que a subdivisão em duas curvas:
//para uma quantidade arbitrária de inserções, ele gera a poligonal
//correspondente às inserções, que gera a mesma curva original.
//Ainda com problemas.

void find(int KN, double *TAU, double *T, int j, int *MU)
{
	for(int i=0; i<=KN-1; i++)
		if ( (fabs(T[j]-TAU[i])<TOL)||(T[j]>TAU[i]))
			*MU=i;
}

//Aqui vamos usar a forma recursiva do algoritmo.

void SUBDIV(FILE *fp, int tipo, Ponto *P, int k, double *TAU, double *T, int RP1, int I, int J, Ponto *PP)
{
	double t;
	int r=RP1-1;
	double P1,P2;
	Ponto PP1, PP2;
fprintf(fp,"\n");
for(int i=0; i<=k-r; i++) fprintf(fp,"----");
fprintf(fp,"(%d)",tipo);

	if (r > 0) {
		PP2.carregue(0,0,0,1);
		PP1.carregue(0,0,0,1);
		P1=TAU[I+k-r] - T[J+k-r];
		P2=T[J+k-r] - TAU[I-1];

		if (fabs(P1)>TOL) SUBDIV(fp,1,P,k,TAU,T,r,I - 1,J,&PP1);

		if (fabs(P2)>TOL) SUBDIV(fp,2,P,k,TAU,T,r,I,J,&PP2);

		t = P2/(P1 + P2);
		*PP=lerp(t, PP1, PP2);
fprintf(fp,"\n");
for(int i=0; i<=k-r; i++) fprintf(fp,"----");
fprintf(fp,"(%d)->P1=T[%d]=%.4f - TAU[%d]=%.4f | P2=TAU[%d]=%.4f - T[%d]=%.4f t=%.4f PP1=(%.4f,%.4f)  PP2=(%.4f,%.4f) PP=(%.4f,%.4f)",
tipo, J+k-r, T[J+k-r], I-1, TAU[I-1], I+k-r, TAU[I+k-r] ,J+k-r, T[J+k-r], t,PP1.x,PP1.z,PP2.x,PP2.z,PP->x,PP->z);
	}
	else {
		*PP=P[I];

fprintf(fp,"\n");
for(int i=0; i<=k-r; i++) fprintf(fp,"----");
fprintf(fp,"(%d)",tipo);
fprintf(fp,"PP=(%.4f,%.4f)X",PP->x,PP->z);

	}
}



//Aqui vamos usar a forma iterativa do algoritmo.

void iSUBDIV(Ponto *P, int k, double *TAU, double *T, int MU, int J, Ponto **TEMP, Ponto *PP)
{
	double t;
	double T1,T2;


	for(int I=(MU-k+1); I<=MU; I++) TEMP[1][I]=P[I];
	for(int r=1; r<=k-1; r++)
	{
		for(int i=MU-k+1+r; i<=MU; i++)
		{
			T1=T[J+k-r] - TAU[i];     //TAU está com i negativo para MU==0
			T2=TAU[i+k-r] - T[J+k-r];
			t = T2/(T1 + T2);
			TEMP[r+1][i] = lerp(t, TEMP[r][i], TEMP[r][i-1]);
		}
	}
	*PP=TEMP[k][MU];
}




void  cnurbs::oslo(cnurbs *esq, cnurbs *dir)
{
	double U;

	U=(u[uix[uuteis[0]]] + u[uix[uuteis[L-1]+1]])/2.0;
	int I=0,Ir=0,ix;
	for(ix=1; ix<L; ix++)
		if (U<u[uix[uuteis[ix]]]) {
			I=uix[uuteis[ix-1]];
			Ir=uuteis[ix-1];
			break;
		}
	if (ix==L)	{
		I=uix[uuteis[L-1]];
		Ir=uuteis[L-1];
	}
	int num_ins=n;     //num_ins=número de inserções
	if (fabs(U-u[I])<TOL) {
		num_ins= n - r[I];
		num_ins=(num_ins<0)?0:num_ins;
	}

	if (num_ins>0) {

	// A subdivisão no meio requer n inserções menos #repetições = num_ins
	// n=K; então: T tem tamanho N+K.
	// Kr=K+N do artigo de Oslo
		double *TAU, *T;
		int MU;

		TAU=(double *) calloc (Kr, sizeof(double));
		for(int i=0; i<Kr; i++)
			TAU[i]=u[uix[i]];

		T= (double *) calloc (Kr+num_ins, sizeof(double));

		for(int i=0; i<=Ir; i++)
			T[i]=TAU[i];
		for(int i=Ir+1; i<=Ir+num_ins; i++)
			T[i]=U;
		for(int i=Ir+num_ins+1; i<Kr+num_ins; i++)
			T[i]=TAU[i-num_ins];

		Ponto  *D = (Ponto *) calloc (c+num_ins, sizeof(Ponto));
		Ponto *P = d;
		Ponto **TEMP;  //Matriz temporária de pontos para iSUBDIV
/*		TEMP=(Ponto **)calloc(n+1,sizeof(Ponto *));
		for(int i=0; i<n; i++)
			TEMP[i]= (Ponto *)calloc(n+1,sizeof(Ponto));

		for(int j=0; j<n-1; j++)
			D[j]=P[j];     */
FILE *fp;
fp=fopen("A:\\silvio\\CAGD\\2024\\saida.txt", "w");

D[0]=P[0];
fprintf(fp,"\nD[0]=(%.4f,%.4f)",D[0].x,D[0].z);

		for(int j=1; j<c+num_ins; j++)
		{
			find(Kr, TAU, T, j, &MU);
 //			iSUBDIV(P, n, TAU, T, MU, j, TEMP, &(D[j]));
			SUBDIV(fp,0, P, n, TAU, T, n, MU, j, &(D[j]));
fprintf(fp,"\nD[%d]=(%.4f,%.4f)",j,D[j].x,D[j].z);
		}
fclose(fp);

/*		for(int i=0; i<n; i++)
			delete(TEMP[i]);
		delete(TEMP);   */


		//K=num de nós sem repetições, membro do objeto cnurbs
		if (num_ins<n)
			esq->inicialize(n,K,Kr+num_ins);
		else
			esq->inicialize(n,K+1,Kr+n);

		for(int i=0; i<c+num_ins; i++)
			esq->Pcarregue(i,D[i]);
		for(int i=0; i<I; i++)
			esq->Ucarregue(i,u[i],r[i]);
		if (num_ins==n) {
			esq->Ucarregue(I,u[I],r[I]);
			esq->Ucarregue(I+1,U,n);
			for(int i=I+2; i<=K; i++)
				esq->Ucarregue(i,u[i-1],r[i-1]);
		}
		else   {
			esq->Ucarregue(I,u[I],n);
			for(int i=I+1; i<K; i++)
				esq->Ucarregue(i,u[i],r[i]);
		}

		esq->defina_intervalos_uteis();        //esq está criando uma posição a mais em u ainda

		delete(TAU);
		delete(T);
		delete(D);
	}
	else
		esq=this;
}


//Aplica o Algoritmo de Oslo para fazer uma subdivisão. Essa é
//a versão otimizada do artigo: MAKING THE OSLO ALGORITHM MORE EFFICIENT
// de T. LYCHE AND K. MORKEN : SlAM J. NUMER. ANAL. Vo|. 23, No. 3, June 1986

void  cnurbs::oslo2(cnurbs *esq, cnurbs *dir)
{
	double U;

	U=(u[uix[uuteis[0]]] + u[uix[uuteis[L-1]+1]])/2.0;
	int I=0,Ir=0,ix;
	for(ix=1; ix<L; ix++)
		if (U<u[uix[uuteis[ix]]]) {
			I=uix[uuteis[ix-1]];
			Ir=uuteis[ix-1];
			break;
		}
	if (ix==L)	{
		I=uix[uuteis[L-1]];
		Ir=uuteis[L-1];
	}
	int num_ins = n;     //num_ins=número de inserções
	if (fabs(U-u[I])<TOL) {
		num_ins= n - r[I];
		num_ins=(num_ins<0)?0:num_ins;
	}

	if (num_ins>0) {

	// A subdivisão no meio requer n inserções menos #repetições = num_ins
	// n=K; então: T tem tamanho N+K.
	// Kr=K+N do artigo de Oslo
		double *TAU, *T, *Csi;
		int MU;

		TAU= (double *) calloc (Kr+1, sizeof(double));
		for(int i=0; i<Kr; i++)
			TAU[i+1]=u[uix[i]];

		T= (double *) calloc (Kr+num_ins+1, sizeof(double));

		for(int i=0; i<=Ir; i++)
			T[i]=TAU[i];
		for(int i=Ir+1; i<=Ir+num_ins; i++)
			T[i]=U;
		for(int i=Ir+num_ins+1; i<Kr+num_ins; i++)
			T[i]=TAU[i-num_ins];

		Ponto *C = (Ponto *) calloc (c+1, sizeof(Ponto));
		Ponto *Ch = (Ponto *) calloc (c+num_ins+1, sizeof(Ponto));
		Ponto *D = (Ponto *) calloc (c+num_ins+1, sizeof(Ponto));

		for(int j=0; j<c; j++)
			C[j+1]=d[j];

		for(int j=0; j<=Ir-n+1; j++)
			D[j]=d[j];
		for(int j=Ir-n+1+2*num_ins; j<c+num_ins; j++)
			D[j]=d[j-num_ins];


		int i,ih,il,iu ,mul,n1=uix[uuteis[0]], n2=uix[uuteis[L-1]], k=num_ins+1, v, kv;  //I+1 porque no Oslo ixs começam em 1
		double tau2, d1, d2;
		int j=Ir-n+3, pix=Ir-n+2*num_ins;
FILE *fp;
fp=fopen("A:\\silvio\\CAGD\\2024\\saida.txt", "w");
fprintf(fp,"Pontos Vindos de LERP:\n");

  /*1.*/i = j + 1; mul = Ir+2-(n-num_ins);
  /*2.*/do {if (!((fabs(T[i]-TAU[mul]) < TOL)&&(i<j+n))) break; i++; mul--;}  while ((1));
  /*3.*/if (mul <= n2+1) Ch[k+1] = C[mul];	else  Ch[k+1].carregue(0,0,0,1);
  /*4.*/ih = mul + 1; v = 0;
fprintf(fp,"\nPara j=%d mul=%d Ch[%d]<=C[%d]=(%.4f,%.4f,%.4f)",j,mul,k+1,mul, C[mul].x,  C[mul].y,  C[mul].z);
  /*5.*/for(int p=1; p<k; p++){
fprintf(fp,"\n\nPerguntamos se T[%d]=%.4f == TAU[%d]=%.4f. Se sim: incremente ih=%d",j+p,T[j+p],ih,TAU[ih],ih);
		/*1.*/if (fabs(T[j+p]-TAU[ih]) < TOL) ih++;//encontrou multiplicidade em TAU:
			  else {                               //avance multiplicidades de T necessárias via p
			  /*1.*/v++;
			  /*2.*/kv = k - v + 1;
			  /*3.*/if ((mul-v<n1)||(mul-v>n2)) Ch[kv].carregue(0,0,0,1);
					else {
fprintf(fp,"\n Ch[%d]<-C[%d]=(%.4f, %.4f, %.4f)", kv, mul - v, C[mul-v].x,C[mul-v].y,C[mul-v].z);
					Ch[kv] = C[mul-v];}
			  /*4.*/tau2=(mul-v<=n2)?TAU[mul+kv-2+(n-num_ins)]:TAU[n2+k];
			  /*5.*/il=max(mul-v,n1); iu=min(mul-1, n2+v);
fprintf(fp,"\n p=%d v=%d i estará entre max(%d,%d)= %d e min(%d,%d)=%d", p, v,mul-v,n1, max(mul-v,n1),mul-1, n2+v,  min(mul-1, n2+v));
fprintf(fp,"\n      tau2=%.4f=TAU[%d] <%do>=%.4f",tau2,(mul-v<=n2)?mul+kv-2+(n-num_ins):n2+k,(mul-v<=n2)?1:2,TAU[(mul-v<=n2)?mul+kv-2+(n-num_ins):n2+k]);
			  /*6.*/for(i=il; i<=iu; i++){   //quantos lerps serão feitas: de il a iu
					/*1.*/kv++;
					/*2.*/d1= U - TAU[i]; d2= tau2 - U;
fprintf(fp,"\nCh[%d]=lerp({tau2=%.4f e TAU[%d]=%.4f},Ch[%d]=(%.4f,%.4f,%.4f),Ch[%d]=(%.4f,%.4f,%.4f))",kv,tau2,i,TAU[i],kv,Ch[kv].x,Ch[kv].y,Ch[kv].z,kv-1,Ch[kv-1].x,Ch[kv-1].y,Ch[kv-1].z);
					/*3.*/Ch[kv] = lerp(d2/(d1+d2),Ch[kv], Ch[kv-1]);
fprintf(fp,"->Ch[%d]=(%.4f,%.4f,%.4f)",kv,Ch[kv].x,Ch[kv].y,Ch[kv].z);
					}
fprintf(fp,"=>D[%d]=Ch[%d]=(%.4f,%.4f,%.4f)",pix,kv,Ch[kv].x,Ch[kv].y,Ch[kv].z);
			  /*7.*/D[pix--]=Ch[kv];
			  }
		}
		for(int i=pix; i>Ir-n+1; i--)
			D[i]=Ch[--kv];
fclose(fp);


		//K=num de nós sem repetições, membro do objeto cnurbs
		int rep=0;
		for(int i=0; i<=I; i++)
			rep += r[i];

		if (num_ins<n)  {
			esq->inicialize(n,I+1,rep+num_ins);
			dir->inicialize(n,K-I,Kr-rep+num_ins+1);
		}
		else  {
			esq->inicialize(n,I+2,rep+n);
			dir->inicialize(n,K-I,Kr-rep+n);
		}

		for(int i=0; i<I+2-(n-num_ins); i++)
			esq->Pcarregue(i,D[i]);
		for(int i=I+1-(n-num_ins); i<c+num_ins; i++)
			dir->Pcarregue(i-I-1+(n-num_ins),D[i]);
		for(int i=0; i<I; i++)
			esq->Ucarregue(i,u[i],r[i]);
		if (num_ins==n) {
			esq->Ucarregue(I,u[I],r[I]);
			esq->Ucarregue(I+1,U,n);
			dir->Ucarregue(0,U,n);
			for(int i=I+2; i<=K; i++)
				dir->Ucarregue(i-I-1,u[i-1],r[i-1]);
		}
		else   {
			esq->Ucarregue(I,u[I],n);
			dir->Ucarregue(0,u[I],n);
			for(int i=I+1; i<K; i++)
				dir->Ucarregue(i-I,u[i],r[i]);
		}

		esq->defina_intervalos_uteis();
		dir->defina_intervalos_uteis();

		delete(TAU);
		delete(T);
		delete(D);
		delete(C);
		delete(Ch);
	}
	else
		esq=dir=this;
}
void cnurbs::desenhe(camera Cam, Graphics::TBitmap *Bitmap, double espacamento, byte R, byte G, byte B)
{
	Bitmap->Canvas->Pen->Width = 2;
	Bitmap->Canvas->Pen->Color= (TColor) RGB (R, G, B);


	Ponto P(0.0,0.0,0.0,1.0);
	ponto p;
	bool prime=true;
	int namostras;

	double U, deltau;
	int r_;
	for(int i=0; i<L; i++) {
		namostras = floor(( u[uix[uuteis[i]+1]] - u[uix[uuteis[i]]] )/espacamento);
		deltau = ( u[uix[uuteis[i]+1]] - u[uix[uuteis[i]]] )/ (double) namostras;
		for(int j=0; j<=namostras; j++){
			U =  u[uix[uuteis[i]]] + j*deltau;
			r_=0;
			if (j==0)  r_=r[uix[uuteis[i]]];
			P=deBoor(uuteis[i],r_,U);
			p=P.projete();
			p=Cam.coordsvista(p);
			p=Cam.perspectiva(p);
			paratela(&p, Bitmap);
			if (prime) {
				Bitmap->Canvas->MoveTo((int)p.x, (int)p.y);
				prime=false;
			}
			else  Bitmap->Canvas->LineTo((int)p.x, (int)p.y);
		}
	}
}



void cnurbs::desenhe_poli(camera Cam, Graphics::TBitmap *Bitmap, byte R, byte G, byte B)
{
	Bitmap->Canvas->Pen->Width = 3;
	Bitmap->Canvas->Pen->Color= (TColor) RGB (R, G, B);

	ponto p;
	for(int i=0; i<c; i++) {
		p=d[i].projete();
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

//Gera uma nova curva NURBS com um dado nó ficando com multiplicidade máxima
//Obs: noh é índice em Uuteis (nós de domínio), de começo de intervalo
cnurbs cnurbs::gere_nurbs_com_multiplicidade_do_noh_completa(int noh)
{
	cnurbs novanurbs;

	int I=uuteis[noh];
	int r_=r[uix[I]];
	novanurbs.inicialize(n,K,Kr+n-r_);

	for(int i=0; i<K; i++){
		novanurbs.u[i]=u[i];
		novanurbs.r[i]=r[i];
	}
	novanurbs.r[uix[I]]=n;
	novanurbs.defina_intervalos_uteis();

	for(int i=0; i<=I-n+1; i++)
		novanurbs.d[i]=d[i];
	for(int i=I-r_+1; i<c;i++)
		novanurbs.d[i+n-r_]=d[i];

	_dcopie();

	double t,U=u[uix[I]];

	for(int k=1; k<=n-r_; k++) {
		for(int i=I-n+k+1; i<=I-r_+1; i++){
			t=(U-u[uix[i-1]])/(u[uix[i+n-k]]-u[uix[i-1]]);
			_d[i-1] = lerp(t, _d[i-1], _d[i]);
		}
		novanurbs.d[I+k-n+1]=_d[I+k-n];
		if (k<n-r_)
			novanurbs.d[I-2*r_-k+n+1]=_d[I-r_];
		for(int i=I-r_+1; i>=I-n+k+1; i--)
			_d[i] = _d[i-1];
	}
	return(novanurbs);
}

cnurbs cnurbs::gere_nurbs_com_multiplicidade_do_ultimo_noh_completa()
{
	cnurbs novanurbs;

	int I=uuteis[L-1];
	int r_=r[uix[I+1]];
	novanurbs.L=L;
	novanurbs.inicialize(n,K-n+r_,Kr);

	for(int i=0; i<novanurbs.K; i++){
		novanurbs.u[i]=u[i];
		novanurbs.r[i]=r[i];
	}
	novanurbs.r[uix[I+1]]=n;

	int rix=0;
	for(int i=0; i<novanurbs.K; i++)
		for(int j=1; j<=novanurbs.r[i]; j++)
			novanurbs.uix[rix++]=i;
	novanurbs.uuteis=(int *) calloc (novanurbs.L, sizeof(int));
	for(int i=0; i<novanurbs.L; i++)
		novanurbs.uuteis[i]=uuteis[i];

	for(int i=0; i<=I-n+r_+1; i++)
		novanurbs.d[i]=d[i];

	_dcopie();

	double t,U=u[uix[I+1]];

	for(int k=1; k<=n-r_; k++) {
		for(int i=I-n+k+r_+1; i<=I+1; i++){
			t=(U-u[uix[i-1]])/(u[uix[i+n-k]]-u[uix[i-1]]);
			_d[i-1] = lerp(t, _d[i-1], _d[i]);
		}
		novanurbs.d[I+k-n+r_+1]=_d[I-n+k+r_];
		for(int i=I+1; i>=I-n+k+r_+1; i--)
			_d[i] = _d[i-1];
	}
	return(novanurbs);
}

cnurbs cnurbs::gere_nurbs_com_multiplicidade_do_primeiro_noh_completa()
{
	cnurbs novanurbs;

	int I=uuteis[0];
	int r_=r[uix[I]];
	novanurbs.L=L;
	novanurbs.inicialize(n,K-n+r_,Kr);

	for(int i=0; i<novanurbs.K; i++){
		novanurbs.u[i]=u[i+n-r_];
		novanurbs.r[i]=r[i+n-r_];
	}
	novanurbs.r[0]=n;
	int rix=0;
	for(int i=0; i<novanurbs.K; i++)
		for(int j=1; j<=novanurbs.r[i]; j++)
			novanurbs.uix[rix++]=i;

	novanurbs.uuteis=(int *) calloc (novanurbs.L, sizeof(int));
	for(int i=0; i<novanurbs.L; i++)
		novanurbs.uuteis[i]=uuteis[i];

	for(int i=0; i<c; i++)
		novanurbs.d[i]=d[i];

	_dcopie();

	double t,U=u[uix[I]];
	for(int k=1; k<=n-r_; k++) {
		for(int i=I-n+k+1; i<=I-r_+1; i++){
			t=(U-u[uix[i-1]])/(u[uix[i+n-k]]-u[uix[i-1]]);
			_d[i-1] = lerp(t, _d[i-1], _d[i]);
		}
		novanurbs.d[n-r_-k]=_d[I-r_];
		for(int i=I-r_+1; i>=I-n+k+1; i--)
			_d[i] = _d[i-1];
	}
	return(novanurbs);
}

cbezier *cnurbs::ToBezier()
{
	cbezier *bzr = (cbezier *) calloc (L, sizeof(cbezier));
	cnurbs *nvnurbs = (cnurbs *) calloc (L+1, sizeof(cnurbs));
	nvnurbs[0]=gere_nurbs_com_multiplicidade_do_primeiro_noh_completa();
	for(int i=1; i<L; i++){
		nvnurbs[i] = nvnurbs[i-1].gere_nurbs_com_multiplicidade_do_noh_completa(i);
		nvnurbs[i-1].destrua();
	}
	nvnurbs[L]=nvnurbs[L-1].gere_nurbs_com_multiplicidade_do_ultimo_noh_completa();
	nvnurbs[L-1].destrua();
	for(int i=0; i<L; i++){
		bzr[i].inicialize(n);
		for(int j=0; j<=n; j++)
			bzr[i].carregue(j, nvnurbs[L].d[i*n+j]);
	}
	nvnurbs[L].destrua();
	delete(nvnurbs);
	return(bzr);
}

