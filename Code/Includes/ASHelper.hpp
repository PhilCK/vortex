
#ifndef AS_HELPER_INCLUDED
#define AS_HELPER_INCLUDED

#include <AngelScript/angelscript.h>
#include <AngelScript/add_on/scriptstdstring/scriptstdstring.h>
#include <AngelScript/add_on/scriptarray/scriptarray.h>
#include <AngelScript/add_on/scriptbuilder/scriptbuilder.h>
#include <AngelScript/add_on/scripthandle/scripthandle.h>
#include <AngelScript/add_on/weakref/weakref.h>
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <cstdint>








// AS Helper thing.


#include <string>

template<typename T>
class ASValueType;

class ASRefType;

template<typename T>
class ASType;

class ASEnumType;



class ASEngine
{
public:

	explicit ASEngine(asIScriptEngine *engine)
	: m_engine(engine)
	{
		
	}
	
	ASEngine& pushNamespace(const std::string &type)
	{
		if(!type.empty())
		{
			m_namespaces.push_back(type);
		}
		
		std::string namespaceStr;
		for(auto &ns : m_namespaces)
		{
			if(ns.empty())
			{
				continue;
			}
			
			if(!namespaceStr.empty())
			{
				namespaceStr += "::";
			}
		
			namespaceStr += ns;
		}
		
		const int r = m_engine->SetDefaultNamespace(namespaceStr.c_str());
		assert(r >= 0);
		
		return *this;
	}
	
	ASEngine& popNamespace()
	{
		m_namespaces.pop_back();
		return pushNamespace("");
	}
	
	ASEngine& clearNamespaces()
	{
		m_namespaces.clear();
		return pushNamespace("");
	}
	
	template<typename T>
	ASValueType<T> addValueType(const std::string &type, const uint32_t flags)
	{
		return ASValueType<T>(*this, type, flags);
	}
	
	inline ASRefType addRefType(const std::string &type, const uint32_t flags);

    // Get needs to check that it exists.
	template<typename T>
	ASValueType<T> getValueType(const std::string &type)
	{
		return ASValueType<T>(*this, type, 0);
	}
	
    // Get needs to check that it exists.
	inline ASRefType getRefType(const std::string &type);
    
    inline ASEnumType addEnumType(const std::string &enumName);
	
	ASEngine& addGlobalFunction(const std::string &funcDecl, const asSFuncPtr &func, asDWORD callConv, void* obj = nullptr)
	{
		const int r = m_engine->RegisterGlobalFunction(funcDecl.c_str(), func, callConv, obj);
		assert(r >= 0);
		
		return *this;
	}
    

	asIScriptEngine* operator->() const { return m_engine; }
    
    
    ASEngine& end() { return *this; }
	
	void close()
	{
		clearNamespaces();
	}

private:
	
	asIScriptEngine *m_engine;
	std::vector<std::string> m_namespaces;
};



template<typename T>
class ASType
{
public:


	explicit ASType(ASEngine &engine, const std::string &type, const uint32_t flags)
	: m_engine(engine)
	, m_typeDecl(type)
	{
	}

		
	T& addCtor(const std::string &ctorDecl, const asSFuncPtr &func, asECallConvTypes callConv)
	{
		const int r = m_engine->RegisterObjectBehaviour(m_typeDecl.c_str(), asBEHAVE_CONSTRUCT, ctorDecl.c_str(), func, callConv); assert(r >= 0);
		
		return *static_cast<T*>(this);
	}
	
	
	T& addListCtor(const std::string &listCtorDecl, const asSFuncPtr &func, asECallConvTypes callConv)
	{
		const int r = m_engine->RegisterObjectBehaviour(m_typeDecl.c_str(), asBEHAVE_LIST_CONSTRUCT, listCtorDecl.c_str(), func, callConv); assert(r >= 0);
		
		return *static_cast<T*>(this);
	}
	
	
	T& addDtor(const std::string &dtorDecl, const asSFuncPtr &func, asECallConvTypes callConv)
	{
		const int r = m_engine->RegisterObjectBehaviour(m_typeDecl.c_str(), asBEHAVE_DESTRUCT, dtorDecl.c_str(), func, callConv); assert(r >= 0);
		
		return *static_cast<T*>(this);
	}
    
    
    T& addImplicitCast(const std::string &castDecl, const asSFuncPtr &func, asECallConvTypes callConv)
    {
        const int r = m_engine->RegisterObjectBehaviour(m_typeDecl.c_str(), asBEHAVE_IMPLICIT_REF_CAST, castDecl.c_str(), func, callConv); assert(r >= 0);
        
        return *static_cast<T*>(this);
    }
	
	
	T& addProperty(const std::string &propertyName, int offset)
	{
		const int r = m_engine->RegisterObjectProperty(m_typeDecl.c_str(), propertyName.c_str(), offset); assert(r >= 0);
		
		return *static_cast<T*>(this);
	}
	
	
	T& addMethod(const std::string &methodDecl, asSFuncPtr func, asECallConvTypes callConv)
	{
		const int r = m_engine->RegisterObjectMethod(m_typeDecl.c_str(), methodDecl.c_str(), func, callConv); assert(r >= 0);
		
		return *static_cast<T*>(this);
	}
	
	ASEngine& end() { return m_engine; }
	
	std::string getType() const { return m_typeDecl; }
	ASEngine &getEngine() const { return m_engine; }
	
private:

	ASEngine &m_engine;
	const std::string m_typeDecl;

}; // ASType



template<typename T>
class ASValueType : public ASType<ASValueType<T> >
{
public:

	explicit ASValueType(ASEngine &engine, const std::string &type, const uint32_t flags)
	: ASType<ASValueType<T> >(engine, type, flags)
	{
		// Check if value exists if not then add.
		const int check = ASType<ASValueType<T> >::getEngine()->GetTypeIdByDecl(ASType<ASValueType<T> >::getType().c_str());
		
		if(check < 0)
		{
			const int r = ASType<ASValueType<T> >::getEngine()->RegisterObjectType(ASType<ASValueType<T> >::getType().c_str(), sizeof(T), asOBJ_VALUE | asGetTypeTraits<T>() | flags); assert(r >= 0);
		}
	}
	
};



class ASRefType : public ASType<ASRefType>
{
public:

	explicit ASRefType(ASEngine &engine, const std::string &type, const uint32_t flags)
	: ASType<ASRefType>(engine, type, flags)
	{
		// Check if value exists if not then add.
		const int check = ASType<ASRefType>::getEngine()->GetTypeIdByDecl(ASType<ASRefType>::getType().c_str());
		
		if(check < 0)
		{
			const int r = ASType<ASRefType>::getEngine()->RegisterObjectType(ASType<ASRefType>::getType().c_str(), 0, asOBJ_REF | flags); assert(r >= 0);
		}
	}

};



class ASEnumType
{
public:
    
    explicit ASEnumType(ASEngine &engine, const std::string &type)
	: m_engine(engine)
	, m_typeDecl(type)
	{
		m_engine->RegisterEnum(type.c_str());
	}
    
    inline ASEnumType& addEnumValue(const std::string &name, int32_t value)
	{
		const int r = m_engine->RegisterEnumValue(m_typeDecl.c_str(), name.c_str(), value);
		assert(r >= 0);
		
		return *this;
	}
    
    inline ASEngine& end() { return m_engine; }
	
	inline std::string getType() const { return m_typeDecl; }
	inline ASEngine &getEngine() const { return m_engine; }
	
private:
    
	ASEngine &m_engine;
	const std::string m_typeDecl;
    
};



ASRefType ASEngine::addRefType(const std::string &type, const uint32_t flags)
{
    return ASRefType(*this, type, flags);
}


ASRefType ASEngine::getRefType(const std::string &type)
{
    return ASRefType(*this, type, 0);
}


ASEnumType ASEngine::addEnumType(const std::string &enumName)
{
	return ASEnumType(*this, enumName);
}
























// OLD STUFF DOWN THERE 


namespace Caffeine {
namespace Systems {


class ASMethodHelper final
{
public:
    
    enum class Interaction
    {
        NO_STATE_PUSH,
        PUSH_NEW_STATE,
        AUTO_PUSH_STATE,
    };
    
    
    ASMethodHelper(asIScriptContext *context, asIScriptFunction *function, asIScriptObject *object)//, const Interaction interaction = Interaction::AUTO_PUSH_STATE)
    : m_context(context)
    , m_function(function)
    , m_object(object)
    , m_result(0)
    //, m_interaction(interaction)
    {
        m_result = m_context->PushState();
        m_result = m_context->Prepare(m_function);
        m_result = m_context->SetObject(m_object);
    }
    
    
    ~ASMethodHelper()
    {
        m_context->PopState();
    }
    
    
    inline bool exec()
    {
        m_result = m_context->Execute();
        
        if(m_result == asEXECUTION_EXCEPTION)
        {
            std::string errStr = m_context->GetExceptionString();
            asIScriptFunction *func = m_context->GetExceptionFunction();
            int i = m_context->GetExceptionLineNumber();
            
            // Log error
        }
        
        return m_result == asEXECUTION_FINISHED;
    }
    
    inline asIScriptFunction* getFunc() { return m_function; }
    inline asIScriptContext* getContext() { return m_context; }
    
    
private:
    
    asIScriptContext *m_context;
    asIScriptFunction *m_function;
    asIScriptObject *m_object;
    int m_result;
    //Interaction m_interaction;
    
}; // class
    
    
} // namespace
} // namespace



struct ASDetails
{
	asIScriptEngine		*asEngine;
	asIScriptContext	*asContext;
};



namespace ASHelper {

ASDetails& GetASDetails();
    

template<typename T>
class RefCount
{
public:
	
	explicit RefCount()
	: m_refCount(0)
	, m_isScript(false)
	{}
	
	
	void Addref()
	{
		++m_refCount;
	}
	
	
	void Release()
	{
		--m_refCount;
		if(!m_refCount)
		{
			if(isScript())
			{
				// TODO: Stop from leaking.
				//delete this;
			}
		}
	}
		
	
	static T* RefFactory()
	{
		T* newObj = new T();
		newObj->setIsScript(true);
		
		return newObj;
	}
	
	void setIsScript(const bool setScript) { m_isScript = setScript; }
	
private:

	bool isScript() const { return m_isScript; }
	
private:

	uint32_t	m_refCount;
	bool		m_isScript;

};


class Context;

class Function
{
public:

	explicit Function(Context *context, asIScriptFunction *func, asIScriptObject *obj = nullptr);
	Function& addArg(const float arg);
	Function& addArg(const int arg);
	Function& addArg(void *arg);
	Function& call();
	bool getReturnAsBool();
	float getReturnAsFloat();
	std::string getReturnAsStdString();
	
	inline bool isExists()			{ return m_func != nullptr; }
	inline bool isObjectMethod()	{ return m_obj != nullptr; }
	
//private:
    
    void prepare();
	
	Context					*m_context;
	
	asIScriptFunction		*m_func;
	asIScriptObject			*m_obj;
	unsigned int			m_arg;
    
    bool                    m_isReady;
};


class ScriptObject
{
public:

	explicit				ScriptObject(Context *context, const std::string &typeDec);
							~ScriptObject();
				
							ScriptObject(const ScriptObject &other);
	
	Function				getMethod(const std::string &funcDec);
	
	bool					isValid() const { return m_scriptObj != nullptr; }
	bool					isOfType(const std::string &type) const;
	
//private:

	Context					*m_context;
	
	asIScriptObject			*m_scriptObj;
	asIObjectType			*m_type;
	const std::string		m_typeDecl;
	asIScriptFunction		*m_factory;
};



class Interface
{
public:

	explicit Interface(Context *context, const std::string &name);

	Interface addMethod(const std::string &dec);
	
private:

	Context *m_context;

	const std::string m_interface;

};




class TypeInfo
{
public:

	explicit TypeInfo(Context *context, const std::string &typeDec);

	bool isOfType(const std::string &type);
	
private:

	Context *m_context;
	asIObjectType *m_type;

};



class EnumType
{
public:

	explicit EnumType(Context *context, const std::string &type);
	EnumType& addValue(const std::string &valueName);
	EnumType& addValue(const std::string &valueName, const unsigned int value);
	
private:

	unsigned int m_lastValue;
	Context *m_context;
	const std::string m_type;
};



template<typename T>
class Object;


class Context final
{
public:

	explicit				Context();
							~Context();
	

	template<typename T>
	Object<T> addObject(const std::string &type, const bool noCount = false)
	{
		asDWORD flag = asOBJ_REF;
		
		if(noCount)
		{
			flag |= asOBJ_NOCOUNT;
		}
		
		const int r = getasEngine()->RegisterObjectType(type.c_str(), 0, flag);
		assert(r >= 0);
		
		return Object<T>(this, type);
	}
	
	
	
	template<typename T>
	Object<T> addObjectValue(const std::string &type, int objSize, const bool isPod = false)
	{
		asDWORD flag = asOBJ_VALUE;
		
		if(isPod)
		{
			flag |= asOBJ_POD;
		}
		
		const int r = getasEngine()->RegisterObjectType(type.c_str(), objSize, flag);
		assert(r >= 0);
		
		return Object<T>(this, type);
	}
	
	
	
	template<typename T>
	void addGlobalFunc(const std::string &dec, T funcPtr, void *objForThisCall = nullptr)
	{
		assert(getasEngine());
		
		int r = getasEngine()->RegisterGlobalFunction(dec.c_str(), asFUNCTION(funcPtr), asCALL_CDECL, objForThisCall);
		
		assert(r >= 0);
	}
	
	Interface				addInterface(const std::string &name);
	void					addGlobalVar(const std::string &signature, void *object);
	bool					addCode(const std::string &code, const std::string &codeName);
	bool					buildCode();
	bool					compileScript(const std::string &file);
	bool					runScriptFunction(const std::string &signature);
	ScriptObject			getScriptObject(const std::string &typeDecl);
	TypeInfo				getTypeInfo(const std::string &typeDecl);
	EnumType				addEnum(const std::string &enumName);
	
	Context&				addEnum(const std::string &name, unsigned int value);
	
	asIScriptEngine *		getasEngine();
	asIScriptContext *		getCtx();

private:

	asIScriptContext		*m_context;

};



// Example REF_CAST behaviour
template<class A, class B>
B* refCast(A* a)
{
    // If the handle already is a null handle, then just return the null handle
    if( !a ) return 0;
    // Now try to dynamically cast the pointer to the wanted type
    B* b = dynamic_cast<B*>(a);

    return b;
}



template<typename T>
class Object
{

public:

	Object(Context *context, const std::string &type)
	: m_typeName(type)
	{}

	template<typename S>
	Object<T> addMethod(S s, const std::string &dec)
	{
		const int r = m_context->getasEngine()->RegisterObjectMethod(m_typeName.c_str(), dec.c_str(), asSMethodPtr<sizeof(void (T::*)())>::Convert((void (T::*)())(s)), asCALL_THISCALL);
		assert(r >= 0);
	
		return *this;
	}
	
	template<typename S>
	Object<T> addBehaviourRefAdd(S s)
	{
		const int r = m_context->getasEngine()->RegisterObjectBehaviour(m_typeName.c_str(), asBEHAVE_ADDREF, "void f()", asSMethodPtr<sizeof(void (T::*)())>::Convert((void (T::*)())(s)), asCALL_THISCALL);
		assert(r >= 0);
		
		return *this;
	}
	
	template<typename S>
	Object<T> addBehaviourRefRelease(S s)
	{
		const int r = m_context->getasEngine()->RegisterObjectBehaviour(m_typeName.c_str(), asBEHAVE_RELEASE, "void f()", asSMethodPtr<sizeof(void (T::*)())>::Convert((void (T::*)())(s)), asCALL_THISCALL);
		assert(r >= 0);
		
		return *this;
	}
	
	template<typename S>
	Object<T> addBehaviourFactory(S s)
	{
		const std::string dec = m_typeName + "@ f()";
		const int r = m_context->getasEngine()->RegisterObjectBehaviour(m_typeName.c_str(), asBEHAVE_FACTORY, dec.c_str(), asFunctionPtr(s), asCALL_CDECL);
		//const int r = m_context->getasEngine()->RegisterGlobalFunction(dec.c_str(), asFunctionPtr(s), asCALL_THISCALL_ASGLOBAL)
		assert(r >= 0);
		
		return *this;
	}
	
	template<typename S, typename D>
	Object<T> addSuperType(const std::string &super, const std::string &derived)
	{
		const std::string derivedDec = derived + "@ f()";
	
		const int r = m_context->getasEngine()->RegisterObjectBehaviour(super.c_str(), asBEHAVE_IMPLICIT_REF_CAST, derivedDec.c_str(), asFUNCTION((refCast<S, D>)), asCALL_CDECL_OBJLAST);
		assert(r >= 0);
		
		return *this;
	}
	
	template<typename S, typename D>
	Object<T> addDerivedType(const std::string &super, const std::string &derived)
	{
		const std::string superDec = super + "@ f()";
	
		const int r = m_context->getasEngine()->RegisterObjectBehaviour(derived.c_str(), asBEHAVE_REF_CAST, superDec.c_str(), asFUNCTION((refCast<D, S>)), asCALL_CDECL_OBJLAST);
		assert(r >= 0);
		
		return *this;
	}
	
	Object<T> addMemberVar(const std::size_t offset, const std::string &dec)
	{
		const int r = m_context->getasEngine()->RegisterObjectProperty(m_typeName.c_str(), dec.c_str(), offset);
		assert(r >= 0);
		
		return *this;
	}
    
    template<typename C>
    Object<T> addCtor(C c)
    {
        const int r = m_context->getasEngine()->RegisterObjectBehaviour(m_typeName.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(c), asCALL_CDECL_OBJLAST);
        assert( r >= 0);
        
        return *this;
    }
    
    template<typename C>
    Object<T> addListCtor(C c, const std::string &dec)
    {
        const std::string listDec = "void f(const int &in) " + dec;
        const int r = m_context->getasEngine()->RegisterObjectBehaviour(m_typeName.c_str(), asBEHAVE_LIST_CONSTRUCT, listDec.c_str(), asFUNCTION(c), asCALL_CDECL_OBJLAST);
        assert( r >= 0);
        
        return *this;
    }
    
    template<typename D>
    Object<T> addDtor(D d)
    {
        const int r = m_context->getasEngine()->RegisterObjectBehaviour(m_typeName.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(d), asCALL_CDECL_OBJLAST);
        assert( r >= 0);
        
        return *this;
    }
    
    
private:

	Context				*m_context;

	const std::string	m_typeName;

};


} // namespace

#endif
