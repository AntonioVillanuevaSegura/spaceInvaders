//Define un marciano ,su(s) imagenes ,vida ,posicion 
/***************************  DECLARACIONES *************************************/
/********************************************************************************/
#ifndef MARCIANOS_H
#define MARCIANOS_H

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/gdicmn.h>//wxPoint
#include <vector> 
#include <wx/dcmemory.h>
using namespace std;
/********************************************************************************/
/********************************************************************************/
class Marciano{
	protected:
    wxImage marcianoA,marcianoB,explosion;//cargar  imagenes 
	wxPoint pt;//Posicion marciano
	bool vivo;//Esta vivo o muerto ?
	int persistencia;//Temporizacion persistencia de la explosion	
	
	public:
	Marciano();
	Marciano(wxImage posA,wxImage posB,wxImage exp, wxPoint pt,wxBitmapType format=wxBITMAP_TYPE_XPM);//Constructor B		
	bool getVivo();
	void setVivo(bool estado);//vivo =true o false	
	wxImage getImagen(bool img);//Utiliza imgA o imgB segun bool
	wxPoint getPosicion();	
	void setPosicion(wxPoint pto);
		
	bool Persistencia() ;//Mientras que este activo se muestra la explosion
	void setPersistencia(int p=10);//Valor persistencia por defecto
	
};
/***************************  DEFINICIONES  *************************************/
#include "marcianos.cpp"
/********************************************************************************/
#endif
