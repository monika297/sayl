/*
 * Comparator.h
 *
 *  Created on: 25 mars 2012
 *      Author: ubuntu
 */

#ifndef COMPARATOR_H_
#define COMPARATOR_H_

namespace SAYL {
/*******************************************************************************************//**
 * @brief Comparator permettant d'avoir le foncteur de comparaison il a besoin d'un operateur de comparaison
 */
class Comparator {
public:
	typedef enum{Equal,LessOrEqual,MoreOrEqual,Less,More,NotEqual} Operator;/**<type enumérant les différent opérateur de comparaison*/
	/*************************************************************************************//**
	 * @brief Constructeur de la classe Comparator
	 * @param operateur operateur de comparaison permettant de définir la fonction de comparaison
	 *****************************************************************************************/
	Comparator(Operator);
	virtual ~Comparator();
/*********************************************************************************************//**
 * @brief redéfinition de l'operateur () permettant d'appliquer le foncteur directement à 2
 * elements de type variable
 */
	template<typename T,typename Q>bool operator () (T e1,Q e2)
		{
			switch(op){
				case Equal:
					return e1==e2;
					break;
				case MoreOrEqual:
					return e1>=e2;
					break;
				case LessOrEqual:
					return e1<=e2;
					break;
				case NotEqual:
					return e1!=e2;
					break;
				}
		}

private:
	Operator op;/**<operateur de comparaison*/
};//Comparator

}//SAYL

 /* namespace SAYL */
#endif /* COMPARATOR_H_ */
