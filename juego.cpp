#include "juego.h"

/********************************************************************************/
/********************************************************************************/
Juego::Juego (wxFrame* parent):wxPanel(parent), m_timer(this, TIMER_ID),clienteDC(this)
{ //Constructor del Juego
	
	SetBackgroundStyle(wxBG_STYLE_PAINT);	
	m_timer.Start(4); // Intervalo timer 1000=1s
	pto=PuntoBase;//Primer marciano posicion 
	velocidadMarcianos=VELOCIDAD_MARCIANOS;//Cada vez descienden a mas velocidad
	vidas1=3;//Vidas , numero de naves de cada jugador
	vidas2=3;//Vidas , numero de naves de cada jugador	

	menu=new menus();//Inicializa menus ,carga imagenes del juego 
	
	/* Inicializa numeros aleatorios  */
	srand (time(NULL));

	//Crea un vector de marcianos con sus coordenadas .Crea 11 marcianos de 80x80 por linea 

	for (int n=1;n<=11;n++){//Crea 1era. linea 0-10 = 11
		marcianos.push_back( Marciano(menu->buscaImagen("Alien3.xpm"),menu->buscaImagen("Alien3b.xpm"),menu->buscaImagen("AlienExplode.xpm"),pto=creaPos(pto)));//Crea vector marcianos	
	}

	for (int n=12;n<=22;n++){//Crea 2a. linea 11-21 = 11
		marcianos.push_back(Marciano(menu->buscaImagen("Alien1.xpm"),menu->buscaImagen("Alien1b.xpm"),menu->buscaImagen("AlienExplode.xpm"),pto=creaPos(pto)));//Crea vector marcianos		
	}
	
	for (int n=23;n<=33;n++){//Crea 3a. linea 22-32 = 11
		marcianos.push_back(Marciano(menu->buscaImagen("Alien1.xpm"),menu->buscaImagen("Alien1b.xpm"),menu->buscaImagen("AlienExplode.xpm"),pto=creaPos(pto)));//Crea vector marcianos		
	}
	
	for (int n=34;n<=44;n++){//Crea 4a. linea 33-43 = 11
		marcianos.push_back(Marciano(menu->buscaImagen("Alien0.xpm"),menu->buscaImagen("Alien0b.xpm"),menu->buscaImagen("AlienExplode.xpm"),pto=creaPos(pto)));//Crea vector marcianos		
	}

	for (int n=45;n<=55;n++){//Crea 5a. linea 44-54 = 11
		marcianos.push_back(Marciano(menu->buscaImagen("Alien0.xpm"),menu->buscaImagen("Alien0b.xpm"),menu->buscaImagen("AlienExplode.xpm"),pto=creaPos(pto)));//Crea vector marcianos		
	}		

	//Construye la nave 
	nave=new Nave(menu->buscaImagen("PlayerSprite.xpm"),menu->buscaImagen("PlayerSprite0.xpm"),menu->buscaImagen("PlayerSprite1.xpm"),wxPoint(600,675)); //Inicializa la nave 

	pto=PuntoBase;//Reset punto de referencia 
	
}
/********************************************************************************/
void Juego::OnTimer(wxTimerEvent& event) //TIMER 1 SEGUNDO
{
	int x(0),y(0);	
	
	if (marcianoTimer>1000){marcianoTimer=0;}//reset inicial
	
	if (marcianoTimer<velocidadMarcianos){marcianoTimer++;} //El tiempo marciano va mas lento que una bala 	
	else{
	
		marcianoTimer=0;//Reset
		
		imgActual=!imgActual;//Imagen a utilizar la A o la B , brazo arriba o abajo 
	
	//Limites de los marcianos
	
		if (limites()){//Los marcianos estan en el limite <-der o izq -> ?		
			sentido = !(sentido); //cambia el sentido de la marcha	DER IZQ	
			//incrementa y  descienden
			for (auto& et:marcianos){//lee el vector de marcianos por referencia !!!!		
				x=et.getPosicion().x;//Obtiene X del marciano corriente 		
				y= et.getPosicion().y;//Obitiene Y del marciano corriente
		
				//Nueva posicion en Y marciano bajan
				et.setPosicion(wxPoint(x,y+10));//Actualiza solo Y 
			}
		}
	
		if (limiteInferior()){resetMarcianos();vidas1--;} //Han llegado abajo reseteamos los marcianos	
	
	//Posiciona marcianos
		for (auto& et:marcianos){//lee el vector de marcianos por referencia !!!!
			
			//Mueve marciano izq. o der.
			x=et.getPosicion().x;//Obtiene X del marciano corriente 		
			y= et.getPosicion().y;//Obitiene Y del marciano corriente
		
			//Nueva posicion del marciano en funcion del sentido
			et.setPosicion(sentido ? wxPoint(x-10,y) : wxPoint(x+10,y));
			
	//Dispara el marciano ?		
			if (marcianoDispara()){//disparo aleatorio de un marciano
				//Primero mira que no hay marcianos delante en Y
				//0-10
				//11-21
				//22-32
				//33-43
				//44-54
				//mira y actual hay otras despues con vida?
				//mira en un posible disparo si toca algo en y
				
			if (!(marcianoDelante(et.getPosicion() ,marcianos))) {//Detecta si NO hay otros delante
				
					disparoNave(marcianoDisp,et.getPosicion(),true);//Gestiona el disparo de un marciano	
				}				
			}
			
		}	
	}
	
	//Control disparos nave y marcianos 	
	vectorDisparo(naveDisp,true);//Mueve los misiles dentro de un vector de tiro
	vectorDisparo(marcianoDisp,false);//Mueve los misiles dentro de un vector de tiro		
			
	paintNow();//parpadeo ....
}
/********************************************************************************/
	//Dibujo
void Juego::paintEvent(wxPaintEvent & evt){
	wxPaintDC dc(this);// only to be used in paint events. use wxClientDC to paint outside the paint event
    render(dc);
}
/********************************************************************************/
void Juego::paintNow(){
	
	//wxClientDC dc(this);//  use wxClientDC to paint outside the paint event	
    //render(dc);
	render(clienteDC);
}
/********************************************************************************/
void Juego::OnSize(wxSizeEvent& event){
    Refresh();
    event.Skip();	
}
/********************************************************************************/
void Juego::render(wxDC& dc){

	dc.SetBackground( *wxBLACK );//FONDO PANTALLA NEGRO
	wxBrush brocha(*wxBLACK,wxBRUSHSTYLE_SOLID );
	dc.SetBrush (brocha);
	dc.Clear();
		
	/****************************************************************************/	
	menu->pantallaJuego(score1,score2,vidas1,dc);//Dibuja textos pantalla principal

	/****************************************************************************/		
	
	//Dibuja disparo
	if (!naveDisp.empty()){//Si hay disparos en el vector
		for (auto disp:naveDisp){//lee el vector de disparos								
			dc.DrawBitmap(menu->buscaImagen("PlayerShotSpr.xpm"),disp);//dibuja disparo
		}
	}


	//Dibuja disparo marciano
	if (!marcianoDisp.empty()){//Si hay disparos en el vector
		for (auto disp:marcianoDisp){//lee el vector de disparos								
			dc.DrawBitmap(menu->buscaImagen("PlungerShot1.xpm"),disp);//dibuja disparo
		}
	}



	/****************************************************************************/	
	
    //Dibuja nave en la pantalla wxDC
	dc.DrawBitmap(nave->getImagen(true),nave->getPosicion(),true);	
	
	/****************************************************************************/	
	
    //Dibuja marcianos .Desde el vector hasta la pantalla  wxDC
    for (auto& et:marcianos){
		
		//Disparo de nave toca marciano ?
		if (colisionObjeto(et,naveDisp)  && et.getVivo()){ //Ha tocado un marciano vivo ?		 
			et.setVivo(false);//Esta muerto ....se activa persistencia explosion
			//destruye el misil
			naveDisp.pop_back();
			//aumenta puntos 
			score1+=10 ;//Sube 10 puntos por matar un marciano
		}		
		
		/****************************************************************************/		

		if (et.getVivo()){//Si esta vivo se muestra la imagen
			dc.DrawBitmap(et.getImagen(imgActual),et.getPosicion(),true);
		} else  if (et.Persistencia())//Si esta explotando 
				{
					dc.DrawBitmap(menu->buscaImagen("AlienExplode.xpm"),et.getPosicion(),true);
				}			
	}   
      
	/****************************************************************************/	      
	
    Refresh();
}
/********************************************************************************/
wxPoint Juego::creaPos(wxPoint pt){//crea coordenadas marciano solo al inicio
	//cada marciano mide 80x80 ,hay 11, la primera posicion sera 

	if (pt.x<PuntoBase.x+80*11){ pt.x+=80; }//linea de marcianos hasta 11 
	else {
		pt.x=PuntoBase.x+80 ;//Coordenada x del primer marciano mas su incremento
		pt.y+=80;}//Salta una linea

	return pt;
}
/********************************************************************************/
bool Juego::limites(){//Han llegado a la derecha o a la izquierda los marcianos ?
	//Analiza la primera linea de marcianos ha llegado al limite izq. o derch.
	
	if (marcianos[0].getPosicion().x <=2 || marcianos[10].getPosicion().x >=(1200-80)){
		velocidadMarcianos-=7;//Cada vez descienden a mas velocidad
		return true;
	}
	return false;
}
/********************************************************************************/
bool Juego::limiteInferior(){//Han llegado abajo ? Han ganado los marcianos ? 
	//Analiza  marciano inferior para analizar y 45-55
	//Tiene que analizar el ultimo marciano de la pila vivo
	std::vector<Marciano>::iterator it= marcianos.end();
	
	//Mirar desde atras el ultimo marciano con vida

	while (it != marcianos.begin() && ! (it->getVivo()) ){--it;}//Mira primer mar. vivo

	if (it->getPosicion().y > LIMITE_INFERIOR ){return true;}	 //Analiza el ultimo	
	return false;	
}
/********************************************************************************/
void Juego::resetMarcianos(){//Posicion inicial marcianos,vidas ..
	
	pto=PuntoBase;//Primer marciano posicion 
	velocidadMarcianos=VELOCIDAD_MARCIANOS;//Cada vez descienden a mas velocidad
	
	for (auto& et:marcianos){//lee el vector de marcianos por referencia !!!!						
			et.setPosicion(pto=creaPos(pto));//Actualiza solo Y 
			et.setVivo(true);//todos vivos
			et.setPersistencia(10);//Valor de la persistencia cuando explota			
		}
				
	//cambia el sentido de la marcha
	sentido=true;	
	
	naveDisp.clear();
	marcianoDisp.clear();
		
}
/********************************************************************************/
void Juego::OnTecla(wxKeyEvent& event){//Evento teclas ...
	//int flags=0;
	if (event.GetKeyCode()==WXK_LEFT){ctrlNave(1);}//->
	if (event.GetKeyCode()==WXK_RIGHT){ctrlNave(2);}//<-
	if (event.GetKeyCode()==WXK_SPACE  ||
	event.GetKeyCode()==WXK_UP){ctrlNave(3);}//Espacio
}
/********************************************************************************/
void Juego::ctrlNave(int ctrl){//Controla movimiento nave 1 izq 2 der 3 dispara 
	wxPoint tmp(nave->getPosicion());
	switch (ctrl){
		case 1:tmp.x-=5;break;//izq.
		case 2:tmp.x+=5;break;//derch.
		case 3:disparoNave(naveDisp,nave->getPosicion(),true);break;//disparo de la nave 
	}
	//analisis limites de la nave 0-1200 
	if (tmp.x<=0){tmp.x=0;}
	if (tmp.x>=(1200-80)){tmp.x=(1200-80);}	
	
	nave->setPosicion (tmp);
	paintNow();
}
/********************************************************************************/
void Juego::disparoNave(vector <wxPoint>& v,wxPoint pto,bool disparo){//Gestiona el disparo de la nave
	//lee el vector de disparo
	//mira su fin
	//destruye pop
	
	if (disparo) { //introduce un disparo en el vector bool=true
//		naveDisp.push_back (nave->getPosicion());
		v.push_back (pto);
		v.back().x+=40;//Acceso ultimo elemento disparo ,central en la nave
		disparo=false;}		
		//vectorDisparo(v);//Gestiona el movimiento de un misil
 }
/********************************************************************************/
//Lo utilizan marcianos y la nave para mover misiles 
void Juego::vectorDisparo(vector <wxPoint>& v,bool tipo){//Mueve los misiles dentro de un vector de tiro
	
		if (!v.empty()){//hay disparos en el vector ?

		for (auto& disp:v){//lee el vector de disparos referencia !!!!
			tipo ? disp.y-=10 : disp.y+=10 ;//decrementa o incrementa Y segun sea el marciano o la nave que disp.
			//Vigila los limites del tiro segun sea marciano o nave
			if ( tipo ? disp.y<=70 : disp.y>=LIMITE_INFERIOR){v.pop_back();}//ver si ha llegado al final y==0
		}
	} 	
} 

/********************************************************************************/
bool Juego::colisionObjeto (Marciano& objeto,vector<wxPoint>& v){//Un objeto marciano o derivado toca un wxPoint

	if (v.empty() ){return false;}//No hay disparos 

	for (auto disparo:v){//recorre el vector de disparos 		
		if ( colision(objeto.getPosicion(),disparo)) {return true;}//alcanzado 
	}

	return false;//No ha sido alcanzado
}

/********************************************************************************/
bool Juego::colision(wxPoint a,wxPoint b){//Objetos o puntos en colision 
	//La base del marciano es a 80x40 ,hay un margen de 10 pixels +o- a cada lado
	//Si el disparo esta dentro del cuadrado 80x40 esta tocado

	if ( (b.x) >= (a.x+12) && b.x <=(a.x+68) && //x los bordes no son la imagen
		 (b.y) >= (a.y) && b.y <= (a.y+40) //y
		){return true;}	
	return false;
}
/********************************************************************************/  

//Dispara si delante no hay marcianos 

bool Juego::marcianoDispara(){//Disparo aleatorio de un alien 
	int num= rand() % 10 + 1; //numero entre 1 y 10 
	//if (num%2 ==0){return true;}
		if (num ==2){return true;} //Solo cuando es 2 
	return false; 
}
/********************************************************************************/
bool Juego::marcianoDelante(wxPoint a,vector <Marciano>& v) {//Detecta si hay otros delante

//Mirar la vertical del marciano Y para ver si hay otros marcianos
//Analisis por [x posicion][y variable]??o todo

	int x=a.x;//Recupera x del marciano actual 
	
	for (int y=a.y ;y>=LIMITE_INFERIOR;y+=40){//simula disparo  a lo largo de Y en intervalo 20
		
		for (auto punto:v){//Recorre vector de marcianos				
		//hay que mirar la misma x y si hay colision a lo largo de Y
				
			//if ( (x== punto.getPosicion().x) && colision (wxPoint(x,y), punto.getPosicion()) && punto.getVivo() ) {return true;}//hay otro delante
			if (x==punto.getPosicion().x && y==punto.getPosicion().y && punto.getVivo() ){return true;}
			
		}
	}
	
return false;//No hay otros en Y
}

/********************************************************************************/

