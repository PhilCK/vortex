
/*
	Storage Component
	--
	
	Project:
	Caffeine
	
 */

#ifndef CAFFEINE_COMPONENTS_STORAGE_INCLUDED
#define CAFFEINE_COMPONENTS_STORAGE_INCLUDED

#include <Caffeine/Entity/Component.hpp>
#include <functional>

namespace Caffeine {
namespace Components {


class StorageComponent : public Entity::Component
{
public:

	//typedef void (*SQLCallback)(void *caller, int sizeOfResults, char **argv, char **azColName);
	//typedef void (*SQLCallback)(void *caller, int sizeOfResults, char **argv, char **azColName);

    typedef std::map<std::string, std::string> DBRowResult;
    
    typedef std::function<void(const DBRowResult&)> DBCallback;
    
	COMPONENT_RTTI(StorageComponent)
	
	explicit				StorageComponent();
							~StorageComponent();
	
	#pragma mark - Database.
	
	void					openDatabase();
	void					closeDatabase();
	//void					excuteSQL(const std::string &sqlQuery, SQLCallback sqlCallback);
	void					excuteSQL(const std::string &sqlQuery, DBCallback& callback);
	
	virtual void			sqlResult(const std::vector<std::string> &result);
	
	void					callBack(void *caller, int size, char **argv, char **azColName);
	
private:

	void					sqlPrivateCallback(void* data, int i, char** fields, char** colNames);
	
private:
	
    DBCallback m_callback;
    
}; // class


} // namespace
} // namespace

#endif // include guard
