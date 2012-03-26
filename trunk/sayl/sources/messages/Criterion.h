#include <sstream>
#include "model/File.h"
#include "draft/Comparator.h"
#ifndef Criterion_H_
#define Criterion_H_

namespace SAYL {
/*************************************************************************//**
 * @class Critere reliés à la requete look
 */
class Criterion {
public:
/****************************************************************************//**
 * @param attribute attribut du critere de la requete
 * @param op Opérateur de comparaison
 * @param value valeur par rapport à la quelle sera comparé l'attribut attribute des fichiers
 */
	Criterion(std::string attribute,Comparator::Operator op,std::string value);
	virtual ~Criterion();

/******************************************************************************//**
 * getAttribute
 */

	std::string getAttribute() const ;
	/******************************************************************************//**
	 * getAttribute
	 */
	void setAttribute(std::string attribute);
	/******************************************************************************//**
	 * getValue
	 */
	std::string getValue() const;
	/******************************************************************************//**
	 * setValue
	 */
	void setValue(std::string value);
	/******************************************************************************//**
	 * \fn look
	 * @brief fonction verifaint si le fichier repond au critere de ce Criterion
	 * \param fichier à verifer
	 */
	bool look(File&);

	Comparator::Operator getOperator() const
	{
		return op;
	}

	void setOperator(Comparator::Operator op)
	{
		this->op = op;
	}


private:
	std::string attribute;
	Comparator::Operator op;
	std::string value;//TODO put a template
};

} /* namespace SAYL */
#endif /* Criterion_H_ */
