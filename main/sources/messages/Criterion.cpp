
#include "model/File.h"
#include "Criterion.h"

namespace SAYL {

Criterion::Criterion(std::string attribute,
		Comparator::Operator op,std::string value)
		:attribute(attribute),value(value){}

Criterion::~Criterion() {
	// TODO Auto-generated destructor stub
}
bool Criterion::look(File& file){
	int size;
	Comparator com (op);
	if(attribute=="size"){

		std::istringstream ss(value);
		ss>>size;
		return  com( file.getFileSize(),size);
	}
	else if(attribute=="filename"){
		return com(file.getFilename(),value);
	}
}

std::string Criterion::getAttribute() const {
	return attribute;
}

void Criterion::setAttribute(std::string attribute) {
	this->attribute = attribute;
}

std::string Criterion::getValue() const {
	return value;
}

void Criterion::setValue(std::string value) {
	this->value = value;
}
}/* namespace SAYL */
