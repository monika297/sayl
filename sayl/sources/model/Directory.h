#include "File.h"
#ifndef DIRECTORY_H_
#define DIRECTORY_H_
#define GoThroughDir(i,x) for(Directory<File>::iterator (i)=(x).begin() ; (i)!=(x).end() ; ++(i))


namespace SAYL {
/******************************************************************************//**
 * classe permettant de stocker des fichiers
 */
template<class F>
class Directory {
public:
	typedef Directory<File> servDir;
	typedef Directory<FileEntry> clientDir;
	typedef std::map <std::string ,File& >::iterator iterator;
	Directory();
	virtual ~Directory();
	F insert(F& file) {
		files.insert(std::pair<std::string,F&>(file.getKey(),file));
		return files.at(file.getKey());
	}
	F getFile(std::string key) {
		return files.at(key);
	}
	iterator begin() {
		return files.begin();
	}
	iterator end(){
		return files.end();
	}
private:
	std::map <std::string ,F& > files;
};

}

template<class F>
inline SAYL::Directory<F>::Directory():files() {
}

template<class F>
inline SAYL::Directory<F>::~Directory() {
}
 /* namespace SAYL */
#endif /* DIRECTORY_H_ */
