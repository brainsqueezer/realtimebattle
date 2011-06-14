#ifndef N_ESCRITOR_H
#define N_ESCRITOR_H
//------------------------------------------------------------------------------
/**
    @class nEscritor

    @brief Clase que se encarga de leer los mensajes entrantes
    y almacenarlos en la memoria
*/

class nEscritor
{
public:
	/// constructor
	nEscritor();
	/// destructor
	virtual ~nEscritor();

	/// Escribe el nombre y el color del Bot
	void Initialize(const char *nombre,const char *color1,const char *color2);

	/// Acelera
	void Accelerate(double aceleracion);

	/// Frena
	void Brake(double frenada);

	/// Opcion de robot
	void RobotOption(int opcion, int valor);

	/// Rota
	void Rotate(int elemento, double velocidad);

	/// Rota Hacia un angulo
	void RotateTo(int elemento, double velocidad, double angulo_destino);

	/// Rota un ángulo determinado
	void RotateAmount(int elemento, double velocidad, double angulo);

	/// "Barre" un ángulo
	void Sweep(int elemento, double velocidad, double angulo_izquierdo, double angulo_derecho);

	/// Dispara
	void Shoot(double energia);

	/// Muestra un mensaje de texto de depuracion
	void Debug(const char *mensaje);

	/// Muestra un mensaje de texto de depuracion con un numero
	void Debug(const char *mensaje, double numero);

	/// Muestra una linea
	void DebugLine(double angulo1, double radio1, double angulo2, double radio2);

	/// Muestra un circulo
	void DebugCircle(double anguloCentro, double radioCentro, double radioCirculo);

	/// Muestra un mensaje de texto
	void Print(const char *mensaje);

	/// Muestra un mensaje de texto con un numero
	void Print(const char *mensaje, double numero);
};

#endif
