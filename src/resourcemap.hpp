#ifndef RESOURCEMAP_HPP
#define RESOURCEMAP_HPP

#include "resource.hpp"

#include <map>
#include <memory>
#include <utility>

/**
 *	@brief Map for storing resources.
 */
class ResourceMap
{
public:

	/**
	 *	@brief Loads resource of given type from file.
	 *	@param path Path to file.
	 *	@note If resource can not be loaded, the process will call `exit`.
	 */
	template<typename T>
	void loadFromFile(const std::string & path)
	{
		std::unique_ptr<Resource> ptr = std::make_unique<T>(T());

		bool result = ptr->loadFromFile(path);

		if(!result)	//Resource did not load
		{
			std::terminate();
		}

		m_map[path] = std::move(ptr);
	}

	/**
	 *	@brief Accesses resouce of given type.
	 *	@param path Path to file.
	 *	@return Pointer of given type to resource.
	 *	@note Attempts to load resource if resource can not be accessed. 
	 *	Will call `exit` if the resource can not be loaded.
	 */
	template<typename T>
	T* access(const std::string & path)
	{
		auto ptr = m_map.find(path);

		if(ptr == m_map.end() ) //If resource does not exist, load it
		{
			loadFromFile<T>(path);
		}
			
		return static_cast<T*>(ptr->second.get());
	}

private:
	std::map<std::string, std::unique_ptr<Resource>> m_map;
};

#endif

