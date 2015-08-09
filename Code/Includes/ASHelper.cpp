
#include <ASHelper.hpp>


namespace {

	//asIScriptEngine  *asEngine(nullptr);
	
	ASDetails asDetails;

//	asIScriptEngine *asEngine(nullptr);
//	std::size_t contextCount(0);
//	asIScriptContext *m_context(nullptr);
	
	void MessageCallback(const asSMessageInfo *msg, void *param)
	{
		switch(msg->type)
		{
			case(asMSGTYPE_ERROR):
				std::cout << "AS Error: ";
				break;
			case(asMSGTYPE_WARNING):
				std::cout << "AS Warning: ";
				break;
			case(asMSGTYPE_INFORMATION):
				std::cout << "AS Info: ";
				break;
			default:
				std::cout << "AS Unkown Type: ";
				break;
		}
		
		std::cout << msg->section << " (" << msg->row << ", " << msg->col << ") " << msg->message << std::endl;
	}
    
}

namespace ASHelper {

ASDetails& GetASDetails() { return asDetails; }
    

Function::Function(Context *context, asIScriptFunction *func, asIScriptObject *obj)
: m_func(func)
, m_obj(obj)
, m_arg(0)
, m_context(context)
, m_isReady(false)
{

}


	
Function& Function::addArg(const float arg)
{
	assert(m_context->getCtx());
    
    if(!m_isReady)
    {
        prepare();
    }
	
	m_context->getCtx()->SetArgFloat(m_arg++, arg);
	return *this;
}
	
	
	
Function& Function::addArg(const int arg)
{
	assert(m_context->getCtx());
    
    if(!m_isReady)
    {
        prepare();
    }
	
	m_context->getCtx()->SetArgDWord(m_arg++, arg);
	return *this;
}



Function& Function::addArg(void *arg)
{
	assert(m_context->getCtx());
    
    if(!m_isReady)
    {
        prepare();
    }
	
	m_context->getCtx()->SetArgObject(m_arg++, arg);
	return *this;
}



Function& Function::call()
{
    if(!m_isReady)
    {
        prepare();
    }
    
	m_context->getCtx()->Execute();
	
	m_context->getCtx()->PopState();
	
	m_arg = 0;
    m_isReady = false;
	
	return *this;
}

bool Function::getReturnAsBool()
{
	assert(m_context->getCtx());
	
	return (bool)m_context->getCtx()->GetReturnByte();
}

float Function::getReturnAsFloat()
{
	assert(m_context->getCtx());
	
	return m_context->getCtx()->GetReturnFloat();
}

std::string Function::getReturnAsStdString()
{
	assert(m_context->getCtx());
	
	std::string returnObj;
	
	returnObj = *(std::string*)m_context->getCtx()->GetReturnObject();
	
	return returnObj;
}

    
void Function::prepare()
{
    assert(m_context->getCtx());
	
	m_context->getCtx()->PushState();
	
	m_context->getCtx()->Prepare(m_func);
	m_context->getCtx()->SetObject(m_obj);
    
    m_isReady = true;
}


ScriptObject::ScriptObject(Context *context,const std::string &typeDec)
: m_context(context)
, m_scriptObj(nullptr)
, m_type(nullptr)
, m_typeDecl(typeDec)
, m_factory(nullptr)
{
    asDetails.asContext->PushState();
    
	asIScriptModule *mod = m_context->getasEngine()->GetModule(0);
	const int typeID = mod->GetTypeIdByDecl(typeDec.c_str());
	m_type = m_context->getasEngine()->GetObjectTypeById(typeID);
	m_type->AddRef();

	const std::string factoryName = typeDec + " @" + typeDec + "()";
	m_factory					  = m_type->GetFactoryByDecl(factoryName.c_str());

	
	asDetails.asContext->Prepare(m_factory);
	asDetails.asContext->Execute();
	
	m_scriptObj = *(asIScriptObject**)m_context->getCtx()->GetAddressOfReturnValue();
	m_scriptObj->AddRef();
    
        asDetails.asContext->PushState();
    
}



ScriptObject::ScriptObject(const ScriptObject &other)
: m_context(other.m_context)
, m_type(other.m_type)
, m_scriptObj(other.m_scriptObj)
, m_typeDecl(other.m_typeDecl)
{
	if(m_type)
	{
		m_type->AddRef();
	}
	if(m_scriptObj)
	{
		m_scriptObj->AddRef();
	}
}



ScriptObject::~ScriptObject()
{
	if(m_scriptObj)
	{
		m_scriptObj->Release();
	}
	
	if(m_type)
	{
		m_type->Release();
	}
}


Function ScriptObject::getMethod(const std::string &funcDec)
{
	asIScriptFunction *func = m_type->GetMethodByDecl(funcDec.c_str());
	assert(func);
	return Function(m_context, func, m_scriptObj);
}



bool ScriptObject::isOfType(const std::string &type) const
{
	if(m_type)
	{
		if(m_type->GetBaseType()->GetName() == type)
		{
			return true;
		}
	}
	
	return false;
}



TypeInfo::TypeInfo(Context *context, const std::string &typeDec)
: m_context(context)
, m_type(nullptr)
{
	asIScriptModule *mod = m_context->getasEngine()->GetModule(0);
	const int typeID = mod->GetTypeIdByDecl(typeDec.c_str());
	m_type = m_context->getasEngine()->GetObjectTypeById(typeID);
}



bool TypeInfo::isOfType(const std::string &type)
{
	if(m_type)
	{
		if(m_type->GetBaseType()->GetName() == type)
		{
			return true;
		}
	}
	
	return false;
}


Interface::Interface(Context *context, const std::string &name)
: m_interface(name)
, m_context(context)
{}



Interface Interface::addMethod(const std::string &dec)
{
	const int r = m_context->getasEngine()->RegisterInterfaceMethod(m_interface.c_str(), dec.c_str());
	assert(r >= 0);
	
	return *this;
}



EnumType::EnumType(Context *context, const std::string &type)
: m_lastValue(0)
, m_context(context)
, m_type(type)
{
	assert(context);
	
	const int r = context->getasEngine()->RegisterEnum(type.c_str());
	assert(r >= 0);
}



EnumType& EnumType::addValue(const std::string &valueName, const unsigned int value)
{
	assert(m_context);
	
	const int r = m_context->getasEngine()->RegisterEnumValue(m_type.c_str(), valueName.c_str(), value);
	assert(r >= 0);
	
	return *this;
}



EnumType& EnumType::addValue(const std::string &valueName)
{
	return addValue(valueName, ++m_lastValue);
}



Context::Context()
{
	// Create the asEngine if the asEngine hasn't been created yet.
	static bool done = false;
	
	
	if(!done)
	{
		done = true;
		
		asDetails.asEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		assert(asDetails.asEngine);
		
		asDetails.asEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
		
		asDetails.asEngine->GetModule(0, asGM_CREATE_IF_NOT_EXISTS);
		
		RegisterStdString(asDetails.asEngine);
		RegisterScriptArray(asDetails.asEngine, true);
	}
	
	asDetails.asContext = asDetails.asEngine->CreateContext();
	assert(asDetails.asContext);
}


Context::~Context()
{

}


Interface Context::addInterface(const std::string &name)
{
	const int r = asDetails.asEngine->RegisterInterface(name.c_str());
	assert(r >= 0);
	
	return Interface(this, name);
}


void Context::addGlobalVar(const std::string &signature, void *object)
{
	const int r = asDetails.asEngine->RegisterGlobalProperty(signature.c_str(), object);
	assert(r >= 0);
}

bool Context::addCode(const std::string &scriptCode, const std::string &codeName)
{
	// Module
	{
		asIScriptModule *module = asDetails.asEngine->GetModule(0, asGM_CREATE_IF_NOT_EXISTS);

		// Addscript section
		{
			const int r = module->AddScriptSection(codeName.c_str(), &scriptCode[0], scriptCode.length());
			
			if(r < 0)
			{
				std::cout << "AddScriptSection() Failed!" << std::endl;
				return false;
			}
		}
	}

	return true;
}


bool Context::buildCode()
{
	// Build
	asIScriptModule *module = asDetails.asEngine->GetModule(0, asGM_CREATE_IF_NOT_EXISTS);

	const int r = module->Build();
	
	if(r < 0)
	{
		std::cout << "Build() failed" << std::endl;
		return false;
	}

	return true;
}


bool Context::compileScript(const std::string &file)
{
	std::string scriptCode;

	// Deal with file contents
	{
		FILE *scriptFile = fopen(file.c_str(), "rb");
		
		if(scriptFile == 0)
		{
			std::cout << "Failed to open script" << std::endl;
			return false;
		}
		
		fseek(scriptFile, 0, SEEK_END);
		const std::size_t fileLength = ftell(scriptFile);
		fseek(scriptFile, 0, SEEK_SET);
		
		scriptCode.resize(fileLength);
			
		const std::size_t c = fread(&scriptCode[0], fileLength, 1, scriptFile);
		
		if(c == 0)
		{
			std::cout << "Failed to load script" << std::endl;
			return false;
		}
	}
	
	return addCode(scriptCode, file);
}

bool Context::runScriptFunction(const std::string &signature)
{
	asIScriptModule *module = asDetails.asEngine->GetModule(0);
	assert(module);
	
	asIScriptFunction *function = module->GetFunctionByDecl(signature.c_str());
	
	if(function == 0)
	{
		std::cout << "Failed to find function" << std::endl;
		return false;
	}
	
	assert(m_context);
	getCtx()->Prepare(function);
	
	const int r = m_context->Execute();
	if(r != asEXECUTION_FINISHED)
	{
		if(r == asEXECUTION_EXCEPTION)
		{
			std::cout << getCtx()->GetExceptionString() << std::endl;
		}
	}
	
	return true;
}


ScriptObject Context::getScriptObject(const std::string &typeDecl)
{
	return ScriptObject(this, typeDecl);
}


TypeInfo Context::getTypeInfo(const std::string &typeDecl)
{
	return TypeInfo(this, typeDecl);
}

EnumType Context::addEnum(const std::string &enumName)
{
	return EnumType(this, enumName);
}



asIScriptEngine * Context::getasEngine()	{ return asDetails.asEngine; }
asIScriptContext * Context::getCtx()		{ return asDetails.asContext; }

	

} // namespace
