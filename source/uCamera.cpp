//---------------------------------------------------------------------------

#pragma hdrstop

#include "uCamera.h"
#define TOL 0.001

//---------------------------------------------------------------------------
#pragma package(smart_init)

camera::camera(ponto Co, ponto Mira, vetor Up, float dd, float hhx, float hhy, int larg, int alt)
{
	C = Co;
	N = Mira.menos(Co);
	N.normalize();
	V = Up.gramschmidt(N);
	V.normalize();
	U = N.X(V);
	d = dd; hx = hhx;  hy = hhy;
	altura = alt;
	largura= larg;
	zbuffer=(float **)calloc(largura+1, sizeof(float *));
	for(int i=0; i<=largura; i++){
		zbuffer[i]=(float *)calloc(altura+1, sizeof(float));
		for(int j=0; j<=altura; j++)
			zbuffer[i][j]=1000000;
	}

}

void camera::inicialize_zbuffer()
{
	if (zbuffer==NULL) {
		zbuffer=(float **)calloc(largura+1, sizeof(float *));
		for(int i=0; i<=largura; i++){
			zbuffer[i]=(float *)calloc(altura+1, sizeof(float));
			for(int j=0; j<=altura; j++)
				zbuffer[i][j]=1000000;
		}
	}
	else
		for(int i=0; i<=largura; i++)
			for(int j=0; j<=altura; j++)
				zbuffer[i][j]=1000000;
}

void camera::desaloque_zbuffer()
{
	for(int i=0; i<=largura; i++)
		free(zbuffer[i]);
	free(zbuffer);
	zbuffer=NULL;
}


camera::camera(int larg, int alt)
{
	C.carregue(10.0, 10.0, 10.0);
	N.carregue(-1.0, -1.0, -1.0);
	N.normalize();
	V.carregue(0, 0, 1.0);
	V = V.gramschmidt(N);
	V.normalize();
	U = N.X(V);
	d = 1; hx = 1; hy = 1;
	altura = alt;
	largura= larg;
	zbuffer=(float **)calloc(largura+1, sizeof(float *));
	for(int i=0; i<=largura; i++){
		zbuffer[i]=(float *)calloc(altura+1, sizeof(float));
		for(int j=0; j<=altura; j++)
			zbuffer[i][j]=1000000;
    }

}

void camera::atualize(ponto Co, ponto Mira, vetor Up, float dd, float hhx, float hhy)
{
	C = Co;
	N = Mira.menos(Co);
	N.normalize();
	V = Up.gramschmidt(N);
	V.normalize();
	U = N.X(V);
	d = dd; hx = hhx;  hy = hhy;
}

ponto camera::coordsvista(ponto P)
{
	static vetor v;
	vetor P_C = P.menos(this->C);
	static ponto Pv;

	v.x = P_C.vezes(this->U);
	v.y = P_C.vezes(this->V);
	v.z = P_C.vezes(this->N);

	Pv.origemmais(v);
	return(Pv);
}

vetor camera::coordsvista(vetor V)
{
	static vetor v;

	v.x = V.vezes(this->U);
	v.y = V.vezes(this->V);
	v.z = V.vezes(this->N);

	return(v);
}

ponto camera::perspectiva(ponto Pv)
{
	 static ponto P;

	 P.x = Pv.x * this->d / (this->hx*Pv.z);
	 P.y = Pv.y * this->d / (this->hy*Pv.z);
	 P.z = this->d;

	 return(P);
}



void Luz_objeto::carregue(vetor Iamb, vetor Iluz, vetor od, ponto Pluz, float ka, float kd, float ks)
{
	Ia=Iamb;
	Ip=Iluz;
	It={50,50,180};//plano de interseção sendo transparente
	Od={od.x/(float)255, od.y/(float)255, od.z/(float)255};
	Pl=Pluz;
	Ka=ks; Kd=kd; Ks=ks;
}


vetor Luz_objeto::equacao_iluminacao(vetor N, ponto P)
{
	vetor V,R,L,I;
	double NvL, VvR;

	V={-P.x, -P.y, -P.z};
	V.normalize();

	L=Pl.menos(P);
	L.normalize();

	NvL=N.vezes(L);

	I={(NvL>0)?Ip.x*NvL*Kd*Od.x:0,  (NvL>0)?Ip.y*NvL*Kd*Od.y:0, (NvL>0)?Ip.z*NvL*Kd*Od.z:0};

	if (Ks>0){
		R={2*NvL*N.x-L.x,2*NvL*N.y-L.y, 2*NvL*N.z-L.z};
		VvR=V.vezes(R);
		if (VvR>0)
			I={I.x+Ip.x*VvR*Ks, I.y+Ip.y*VvR*Ks, I.z+Ip.z*VvR*Ks};

    }
	I={I.x+Ia.x*Ka,  I.y+Ia.y*Ka,I.z+Ia.z*Ka};
	I={(I.x>255)?255:I.x, (I.y>255)?255:I.y, (I.z>255)?255:I.z};
	return(I);
}


void Luz_objeto::Phong(camera Camera1, int i1, int j1, int i2, int j2, int i3, int j3, int Npts, int Mpts, ponto P1, ponto P2, ponto P3, vetor N1,
					   vetor N2, vetor N3, vetor N,	float *Rxmin, float *Rxmax, float deltaMin, float deltaMax,
					   int ymin, int ymax, float t_numerador, ponto p1, ponto p2, ponto p3, float Kt, bool wireframe,bool trim,
					   TBitmap *Bitmap, TBitmap *Textura)
{
	int yscan, xmin, xmax, r,g,b;
	float px,py,xr,xb,dx, t, gama,z;
	baricentrico bari;
	ponto P;
	vetor Np,I={255,255,255},Odd=Od;
	bool dontshow = false;

	xmin = round(*Rxmin);
	xmax = round(*Rxmax);
	if (xmin<0) xmin=0;
	if (xmax>Camera1.largura-1) xmax = Camera1.largura-1;
	if (ymin<0) ymin = 0;
	for(int yscan=ymax; yscan>=ymin; yscan--){
		py =(1.0 - (2*yscan/(float)Camera1.altura))*Camera1.hy;
		xr = *Rxmin - xmin;
		xb = *Rxmax - xmax;
		if (xmin==xmax)  dx=0;
		else dx =(xb - xr)/(float)(xmax - xmin);
		for(int x =xmin; x<=xmax; x++){
			px = (2*(x+xr+(x-xmin)*dx)/(float)Camera1.largura - 1.0)*Camera1.hx;
			t = t_numerador/(px*N.x + py*N.y + Camera1.d*N.z);
			bari.coordenadas_baricentricas(p1,p2,p3, (float)(x + xr + (x-xmin)*dx), (float)yscan);
			gama = 1.0 - bari.alfa - bari.beta;
			if (((bari.alfa>0)||(fabs(bari.alfa)<TOL))&&((bari.beta>0)||(fabs(bari.beta)<TOL))
			  &&((gama>0)||(fabs(gama)<TOL)) ){
				z=t*Camera1.d;
				if (z < fabs(Camera1.zbuffer[x][yscan])) {
					P={t*px, t*py, z};
					Np={bari.alfa*N1.x + bari.beta*N2.x + gama*N3.x,
						bari.alfa*N1.y + bari.beta*N2.y + gama*N3.y,
						bari.alfa*N1.z + bari.beta*N2.z + gama*N3.z};
					Np.normalize();
					if ((!wireframe)||(Textura==NULL)||(!trim)) Camera1.zbuffer[x][yscan]=z;
					if (wireframe&&(Textura==NULL)&&((bari.alfa<0.05)||(bari.beta<0.05)||(gama<0.05))){
						Od={0.1,0.25,1.0};
						I=equacao_iluminacao(Np,P);
						Od=Odd;
						Camera1.zbuffer[x][yscan]=z;
					}
					else
						if ((wireframe)&&(Textura!=NULL)){
							int tix = trunc((bari.alfa*i1 + bari.beta*i2 + gama*i3)*(Textura->Width/(float)Npts)+0.5);
							int tjx = trunc((bari.alfa*j1 + bari.beta*j2 + gama*j3)*(Textura->Height/(float)Mpts)+0.5);
							r = GetRValue(Textura->Canvas->Pixels[tix][tjx]);
							g = GetGValue(Textura->Canvas->Pixels[tix][tjx]);
							b = GetBValue(Textura->Canvas->Pixels[tix][tjx]);
							if ((r==0)&&(g==0)&&(b==0)&&(trim)) {
								dontshow = true;
							}
							else{
								dontshow = false;
								Od={(float)r/255.0,(float)g/255.0,(float)b/255.0};
								I=equacao_iluminacao(Np,P);
								Od=Odd;
								Camera1.zbuffer[x][yscan]=z;
							}
						}
						else
							I=equacao_iluminacao(Np,P);
					if (!dontshow) {
						if (Kt>TOL){
							r=GetRValue(Bitmap->Canvas->Pixels[x][yscan]);
							g=GetGValue(Bitmap->Canvas->Pixels[x][yscan]);
							b=GetBValue(Bitmap->Canvas->Pixels[x][yscan]);
							r=trunc( Kt*(float)r + (1.0 - Kt)*It.x );
							g=trunc( Kt*(float)g + (1.0 - Kt)*It.y );
							b=trunc( Kt*(float)b + (1.0 - Kt)*It.z );
							Bitmap->Canvas->Pixels[x][yscan] = static_cast<TColor>(RGB(r,g,b)) ;
						}
						else
							Bitmap->Canvas->Pixels[x][yscan] = static_cast<TColor>(RGB(trunc(I.x),trunc(I.y),trunc(I.z))) ;
					}
				}
			}
			else
				Bitmap->Canvas->Pixels[x][yscan] = static_cast<TColor>(RGB(150,150,150)) ;
		}
		*Rxmin = *Rxmin + deltaMin;
		*Rxmax = *Rxmax + deltaMax;
		xmin = round(*Rxmin);
		xmax = round(*Rxmax);
		if (xmin<0)  xmin =0;
		if (xmax>Camera1.largura-1)  xmax=Camera1.largura-1;
	}

}


bool sao_colineares(int x1, int y1, int x2, int y2, int x3, int y3)
{
	int deltax2 = x2-x1, deltay2 = y2-y1, deltax3 = x3-x1, deltay3 = y3-y1;

	if ((deltax3==0)&&(deltay3==0)) return(true);
	else if (deltax3==0) return(deltax2==0);
	else if (deltay3==0) return(deltay2==0);
	else return(fabs((float)deltax2/(float)deltax3-(float)deltay2/(float)deltay3)<TOL);
}


void Luz_objeto::varredura(camera Camera1, int i1, int j1, int i2, int j2, int i3, int j3, int Npts, int Mpts,
							ponto P1, ponto P2, ponto P3, vetor N1, vetor N2, vetor N3, vetor N, float Kt,
							bool wireframe,bool trim, TBitmap *Bitmap,TBitmap *Textura)
{
	int xmin,xmax,ymin,ymax,aux2,aux1,x1,y1,x2,y2,x3,y3;
	float delta12,delta23,delta13,deltaMin,deltaMax,t_numerador;
	float Rxmin,Rxmax,px,py,alfa,beta,gama,z,xr,xb,dx;
	vetor  L,V,R,Np;
	ponto P;
	bool  lado_oposto;
	float meia_altura, meia_largura;



	meia_altura = Camera1.altura / 2.0;
	meia_largura= Camera1.largura / 2.0;           //preambulo: inicio

	Bitmap->Canvas->Pen->Color= (TColor) RGB (255, 0, 0);

	ponto Q1 = P1, Q2 = P2, Q3 = P3;
	Q1=Camera1.perspectiva(Q1);
	Q2=Camera1.perspectiva(Q2);
	Q3=Camera1.perspectiva(Q3);
	int larg=Bitmap->Width, alt=Bitmap->Height;

	Q1.x = (Q1.x + 1)*meia_largura ;
	Q1.y = (1 - Q1.y)*meia_altura;
	Q2.x = (Q2.x + 1)*meia_largura ;
	Q2.y = (1 - Q2.y)*meia_altura;
	Q3.x = (Q3.x + 1)*meia_largura ;
	Q3.y = (1 - Q3.y)*meia_altura;

	x1=trunc(Q1.x); y1=trunc(Q1.y);
	x2=trunc(Q2.x); y2=trunc(Q2.y);
	x3=trunc(Q3.x); y3=trunc(Q3.y);


	if (sao_colineares(x1,y1,x2,y2,x3,y3)) return;


	if ( !((x1==x2)&&(x1==x3)) &&  !((y1==y2)&&(y1==y3)) && !((x1<0)&&(x2<0)&&(x3<0))
		&& !((x1>Camera1.largura)&&(x2>Camera1.largura)&&(x3>Camera1.largura))  && !((y1<0)&&(y2<0)&&(y3<0))
		&& !((y1>Camera1.altura)&&(y2>Camera1.altura)&&(y3>Camera1.altura)) ){
   // PONTO MAIS ALTO E PONTO INTERMEDIARIO
	  ponto Ptmp;
	  vetor Ntmp;
	  if (y2>y1){
		 aux1 = x1;
		 aux2 = y1;
		 y1 = y2;
		 x1 = x2;
		 y2 = aux2;
		 x2 = aux1;
		 aux1 = i1;
		 aux2 = j1;
		 j1 = j2;
		 i1 = i2;
		 j2 = aux2;
		 i2 = aux1;
		 Ptmp=P1; Ntmp=N1;
		 P1=P2;   N1=N2;
		 P2=Ptmp; N2=Ntmp;
	  }
	  if (y3>y1) {
		 aux1 = x1;
		 aux2 = y1;
		 y1 = y3;
		 x1 = x3;
		 y3 = aux2;
		 x3 = aux1;
		 aux1 = i1;
		 aux2 = j1;
		 j1 = j3;
		 i1 = i3;
		 j3 = aux2;
		 i3 = aux1;
		 Ptmp=P1;  Ntmp=N1;
		 P1=P3;    N1=N3;
		 P3=Ptmp;  N3=Ntmp;
	  }
	  if (y3>y2) {  //Ponto mais baixo
		 aux1 = x2;
		 aux2 = y2;
		 y2 = y3;
		 x2 = x3;
		 y3 = aux2;
		 x3 = aux1;
		 aux1 = i2;
		 aux2 = j2;
		 j2 = j3;
		 i2 = i3;
		 j3 = aux2;
		 i3 = aux1;
		 Ptmp=P2; Ntmp=N2;
		 P2=P3;   N2=N3;
		 P3=Ptmp; N3=Ntmp;
	  }
	  if (y1==y2)
		 delta12 = 0;
      else
		 delta12 = -(x2-x1)/(float)(y2-y1);
	  if (y2==y3)
		 delta23 = 0;
	  else
		 delta23 = -(x3-x2)/(float)(y3-y2);
	  delta13 = -(x3-x1)/(float)(y3-y1);
   }                                             //preambulo: fim

		 //determina lado visivel: inicio
   ponto p1={(double)x1,(double)y1,1};
   ponto p2={(double)x2,(double)y2,1};
   ponto p3={(double)x3,(double)y3,1};
   V={-P1.x,-P1.y,-P1.z};
   V.normalize();
   if (V.vezes(N)<0) N.vezes(-1);
   if (V.vezes(N1)<0) N1.vezes(-1);
   if (V.vezes(N2)<0) N2.vezes(-1);
   if (V.vezes(N3)<0) N3.vezes(-1);

   t_numerador = P3.x * N.x + P3.y * N.y + P3.z * N.z;


   if (y1==y2)  {
	  deltaMin = delta13;
	  deltaMax = delta23;
	  xmin = x1;
	  xmax = x2;
	  if (delta13<delta23)  {
		 deltaMin = delta23;
		 deltaMax = delta13;
		 xmin = x2;
		 xmax = x1;
	  }
	  ymin = y3;
	  ymax = y1;
	  Rxmin = xmin;
	  Rxmax = xmax;

	  if (ymax>Camera1.altura-1) {
		 Rxmin = -deltaMin*(Camera1.altura-1-ymax)+xmin;
		 Rxmax = -deltaMax*(Camera1.altura-1-ymax)+xmax;
		 ymax = Camera1.altura-1;
	  }
	  Phong(Camera1, i1, j1, i2, j2, i3, j3, Npts, Mpts, P1, P2, P3, N1, N2, N3, N,
			&Rxmin, &Rxmax, deltaMin, deltaMax,	ymin, ymax, t_numerador, p1, p2, p3,
			 Kt, wireframe, trim,Bitmap, Textura);
   }
   else {
	  deltaMin = delta12;
	  deltaMax = delta13;
	  if (delta13<delta12)  {
		 deltaMin = delta13;
		 deltaMax = delta12;
	  }
	  Rxmin = x1;
	  Rxmax = x1;
	  ymin = y2;
	  ymax = y1;
	  if (ymax>Camera1.altura-1) {
		 ymax = Camera1.altura-1;
		 Rxmin = -deltaMin*(Camera1.altura-1-y1)+x1;
		 Rxmax = -deltaMax*(Camera1.altura-1-y1)+x1;
	  }
	  Phong(Camera1, i1, j1, i2, j2, i3, j3, Npts, Mpts, P1, P2, P3, N1, N2, N3, N, &Rxmin, &Rxmax, deltaMin, deltaMax,
			ymin, ymax, t_numerador, p1, p2, p3, Kt, wireframe, trim, Bitmap,Textura);
	  Rxmin = Rxmin-deltaMin;
	  Rxmax = Rxmax-deltaMax;
	  if (y2!=y3) {
		 deltaMin = delta13;
		 deltaMax = delta23;
		 if (delta13<delta23) {
			deltaMin = delta23;
			deltaMax = delta13;
		 }
		 ymin = y3;
		 ymax = y2;
		 if (ymax>Camera1.altura-1) {
			ymax = Camera1.altura-1;
			if (delta13>delta23) {
			   Rxmin = -deltaMin*(Camera1.altura-1-y1)+x1;
			   Rxmax = -deltaMax*(Camera1.altura-1-y2)+x2;
			}
			else {
			   Rxmin = -deltaMin*(Camera1.altura-1-y2)+x2;
			   Rxmax = -deltaMax*(Camera1.altura-1-y1)+x1;
			}
		 }
		Phong(Camera1, i1, j1, i2, j2, i3, j3, Npts, Mpts, P1, P2, P3, N1, N2, N3, N, &Rxmin, &Rxmax, deltaMin, deltaMax,
			ymin, ymax, t_numerador, p1, p2, p3, Kt, wireframe, trim, Bitmap,Textura);
	  }
   }
}

