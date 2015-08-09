

#ifndef BASE_ASSET_MANAGER_BASE_INCLUDED
#define BASE_ASSET_MANAGER_BASE_INCLUDED


#include <Caffeine/Utilities/Noncopyable.hpp>


namespace Base {

/*
	AssetManagerBase
	--
	Gets/Checks files in directories for changes.

	Used to get Scripts/Textues/Shaders/States etc.

	TODO: Check timestamps.
*/
class AssetManagerBase : private CaffUtils::Noncopyable
{
protected:

	explicit AssetManagerBase(const std::vector<std::string> dirs, const std::vector<std::string> &ext, const bool searchSubDirs = false)
	: m_checkSubDirs(searchSubDirs)
	, m_files()
	, m_paths(paths)
	, m_extensions(ext)
	{
	}


	virtual ~AssetManagerBase() {}

	
	void checkDirs()
	{
		// Get list of files to check against.
		for(auto &path : m_paths)
		{
			for(auto &ext : m_extensions)
			{
				// get list of files in directory with extension

			}
		}

		// Check for new files.
		{

		}

		// Check for deletions.
		{

		}

		// Check for updates.
		{

		}

		// Copy new list onto old list.
		{

		}
	}

	const FileInfo& 	getFile(const std::size_t i) const	{ return m_files.at(i);  }
	std::size_t 		numberOfFiles() const				{ return m_files.size(); }

	// File info.
	struct FileInfo : CaffUtils::Noncopyable
	{
		uint64_t 		createdTS;
		uint64_t 		updatedTS;
		std::string 	filename;
		std::string 	filepath;
	};	

private:

	// *** Child Hooks *** //

	virtual void newFile(FileInfo &info) 		{}
	virtual void updatedFile(FileInfo &info)	{}
	virtual void deletedFile(FileInfo &info)	{}

private:	

	bool 						m_checkSubDirs = false;
	std::vector<FileInfo> 		m_files;
	std::vector<std::string>	m_paths;
	std::vector<std::string>	m_extensions;

}; // class


} // namespace

#endif // include guard
