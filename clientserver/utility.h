#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <cstdlib>
#include <string>

namespace util {
	typedef int id_type;

	struct error {
		error(const std::string &message)
		{
			std::cerr << message << std::endl;
			std::exit(EXIT_FAILURE);
		}
	};
}

#endif /* UTILITY_H */
