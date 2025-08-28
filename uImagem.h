//---------------------------------------------------------------------------
 #include <vcl.h>
#ifndef uImagemH
#define uImagemH
//---------------------------------------------------------------------------
#include "uCamera.h"

void desenhe_cubo_padrao(camera Cam, Graphics::TBitmap *Bitmap);
void paratela(ponto *P, Graphics::TBitmap *Bitmap);
void desenhe_planos_e_eixos(camera Cam, Graphics::TBitmap *Bitmap, bool limpar);
void corrija_Pt_contra_plano_de_vista(camera Cam, ponto P1, ponto *P2);



#endif
