#include "Core/Public/deCVar.h"

#include <unordered_map>
#include <array>
#include <shared_mutex>

#include "Tools/Public/deConsole.h"

// Honestly i dont think anyone will be able to hit the max
#define MAX_INT_CVARS 1000
#define MAX_FLOAT_CVARS 1000
#define MAX_STRING_CVARS 200

de::CVar::CVar_Int CVAR_protection_enabled("protection_enabled", 1, de::CVar::ProtectionLevel::UnProtected);

namespace de
{
	enum class CVarType : char
	{
		INT,
		FLOAT,
		STRING,
	};

	class CVarParameter
	{
	public:
		friend class CVarSystemImpl;

		int32_t arrayIndex;

		CVarType type;
		CVar::ProtectionLevel protectionLvl;
		std::string name;
	};

	template<typename T>
	struct CVarStorage
	{
		T initial;
		T current;
		CVarParameter* parameter;
	};

	template<typename T>
	struct CVarArray
	{
		CVarStorage<T>* cvars{ nullptr };
		int32_t lastCVar{ 0 };

		CVarArray(size_t size)
		{
			cvars = new CVarStorage<T>[size]();
		}


		CVarStorage<T>* GetCurrentStorage(int32_t index)
		{
			return &cvars[index];
		}

		T* GetCurrentPtr(int32_t index)
		{
			return &cvars[index].current;
		};

		T GetCurrent(int32_t index)
		{
			return cvars[index].current;
		};

		void SetCurrent(const T& val, int32_t index)
		{
			cvars[index].current = val;
		}

		int Add(const T& value, CVarParameter* param)
		{
			int index = lastCVar;

			cvars[index].current = value;
			cvars[index].initial = value;
			cvars[index].parameter = param;

			param->arrayIndex = index;
			lastCVar++;
			return index;
		}

		int Add(const T& initialValue, const T& currentValue, CVarParameter* param)
		{
			int index = lastCVar;

			cvars[index].current = currentValue;
			cvars[index].initial = initialValue;
			cvars[index].parameter = param;

			param->arrayIndex = index;
			lastCVar++;

			return index;
		}
	};

	uint32_t Hash(const char* str)
	{
		return StringUtils::fnv1a_32(str, strlen(str));
	}

	class CVarSystemImpl : public CVarSystem
	{
	public:
		CVarParameter* GetCVar(StringUtils::StringHash hash) override final;	

		double* GetFloatCVar(StringUtils::StringHash hash) override final;
		int32_t* GetIntCVar(StringUtils::StringHash hash) override final;
		const char* GetStringCVar(StringUtils::StringHash hash) override final;

		void SetFloatCVar(StringUtils::StringHash hash, double value) override final;
		void SetIntCVar(StringUtils::StringHash hash, int32_t value) override final;
		void SetStringCVar(StringUtils::StringHash hash, const char* value) override final;

		void SetFloatCVarSafe(StringUtils::StringHash hash, double value) override final;
		void SetIntCVarSafe(StringUtils::StringHash hash, int32_t value) override final;
		void SetStringCVarSafe(StringUtils::StringHash hash, const char* value) override final;

		CVarParameter* CreateFloatCVar(const char* name, double defaultValue, double currentValue) override final;
		CVarParameter* CreateIntCVar(const char* name, int32_t defaultValue, int32_t currentValue) override final;
		CVarParameter* CreateStringCVar(const char* name, const char* defaultValue, const char* currentValue) override final;

	public:
		CVarArray<int32_t> intCVars{ MAX_INT_CVARS };

		CVarArray<double> floatCVars{ MAX_FLOAT_CVARS };

		CVarArray<std::string> stringCVars{ MAX_STRING_CVARS };

		template<typename T>
		CVarArray<T>* GetCVarArray();

		template<>
		CVarArray<int32_t>* GetCVarArray()
		{
			return &intCVars;
		}
		template<>
		CVarArray<double>* GetCVarArray()
		{
			return &floatCVars;
		}
		template<>
		CVarArray<std::string>* GetCVarArray()
		{
			return &stringCVars;
		}

		//templated get-set cvar versions for syntax sugar
		template<typename T>
		T* GetCVarCurrent(uint32_t namehash) {
			CVarParameter* par = GetCVar(namehash);
			if (!par) {
				return nullptr;
			}
			else {
				return GetCVarArray<T>()->GetCurrentPtr(par->arrayIndex);
			}
		}

		template<typename T>
		void SetCVarCurrent(uint32_t namehash, const T& value)
		{
			CVarParameter* cvar = GetCVar(namehash);
			if (cvar)
			{
				GetCVarArray<T>()->SetCurrent(value, cvar->arrayIndex);
			}
		}

		static CVarSystemImpl* Get()
		{
			return static_cast<CVarSystemImpl*>(CVarSystem::Get());
		}

	private:

		CVarParameter* InitCVar(const char* name);

	private:

		std::shared_mutex mutex_;

		std::unordered_map<uint32_t, CVarParameter> savedCVars;

		std::vector<CVarParameter*> cachedEditParameters;
	};

	double* CVarSystemImpl::GetFloatCVar(StringUtils::StringHash hash)
	{
		return GetCVarCurrent<double>(hash);
	}

	int32_t* CVarSystemImpl::GetIntCVar(StringUtils::StringHash hash)
	{
		return GetCVarCurrent<int32_t>(hash);
	}

	const char* CVarSystemImpl::GetStringCVar(StringUtils::StringHash hash)
	{
		return GetCVarCurrent<std::string>(hash)->c_str();
	}

	CVarSystem* CVarSystem::Get()
	{
		static CVarSystemImpl cvarSys{};
		return &cvarSys;
	}

	CVarParameter* CVarSystemImpl::GetCVar(StringUtils::StringHash hash)
	{
		std::shared_lock lock(mutex_);
		auto it = savedCVars.find(hash);

		if (it != savedCVars.end())
		{
			return &(*it).second;
		}

		return nullptr;
	}

	void CVarSystemImpl::SetFloatCVar(StringUtils::StringHash hash, double value)
	{
		SetCVarCurrent<double>(hash, value);
	}

	void CVarSystemImpl::SetIntCVar(StringUtils::StringHash hash, int32_t value)
	{
		SetCVarCurrent<int32_t>(hash, value);
	}

	void CVarSystemImpl::SetStringCVar(StringUtils::StringHash hash, const char* value)
	{
		SetCVarCurrent<std::string>(hash, value);
	}

	void CVarSystemImpl::SetFloatCVarSafe(StringUtils::StringHash hash, double value)
	{
		CVarParameter* cvar = GetCVar(hash);

		if (cvar)
		{
			switch (cvar->protectionLvl)
			{
			case CVar::ProtectionLevel::UnProtected:
				SetCVarCurrent<double>(hash, value);
				break;
			case CVar::ProtectionLevel::Protected:
				if (CVAR_protection_enabled.Get())
				{
					Console::Post("CVar : " + cvar->name + " cant be changed because it is a protected CVar. Set protection_enabled to 0 to change the value of this CVar", Console::LogLevel::Warning);
				}
				else
				{
					SetCVarCurrent<double>(hash, value);
				}
				break;
			default:
				break;
			}
		}
	}

	void CVarSystemImpl::SetIntCVarSafe(StringUtils::StringHash hash, int32_t value)
	{
		CVarParameter* cvar = GetCVar(hash);

		if (cvar)
		{
			switch (cvar->protectionLvl)
			{
			case CVar::ProtectionLevel::UnProtected:
				SetCVarCurrent<int>(hash, value);
				break;
			case CVar::ProtectionLevel::Protected:
				if (CVAR_protection_enabled.Get())
				{
					Console::Post("CVar : " + cvar->name + " cant be changed because it is a protected CVar. Set protection_enabled to 0 to change the value of this CVar", Console::LogLevel::Warning);
				}
				else
				{
					SetCVarCurrent<int>(hash, value);
				}
				break;
			default:
				break;
			}
		}
	}

	void CVarSystemImpl::SetStringCVarSafe(StringUtils::StringHash hash, const char* value)
	{
		CVarParameter* cvar = GetCVar(hash);

		if (cvar)
		{
			switch (cvar->protectionLvl)
			{
			case CVar::ProtectionLevel::UnProtected:
				SetCVarCurrent<std::string>(hash, value);
				break;
			case CVar::ProtectionLevel::Protected:
				if (CVAR_protection_enabled.Get())
				{
					Console::Post("CVar : " + cvar->name + " cant be changed because it is a protected CVar. Set protection_enabled to 0 to change the value of this CVar", Console::LogLevel::Warning);
				}
				else
				{
					SetCVarCurrent<std::string>(hash, value);
				}
				break;
			default:
				break;
			}
		}
	}

	CVarParameter* CVarSystemImpl::CreateFloatCVar(const char* name, double defaultValue, double currentValue)
	{
		std::unique_lock lock(mutex_);
		CVarParameter* param = InitCVar(name);
		if (!param) return nullptr;

		param->type = CVarType::FLOAT;

		GetCVarArray<double>()->Add(defaultValue, currentValue, param);

		return param;
	}

	CVarParameter* CVarSystemImpl::CreateIntCVar(const char* name, int32_t defaultValue, int32_t currentValue)
	{
		std::unique_lock lock(mutex_);
		CVarParameter* param = InitCVar(name);
		if (!param) return nullptr;

		param->type = CVarType::INT;

		GetCVarArray<int32_t>()->Add(defaultValue, currentValue, param);

		return param;
	}

	CVarParameter* CVarSystemImpl::CreateStringCVar(const char* name, const char* defaultValue, const char* currentValue)
	{
		std::unique_lock lock(mutex_);
		CVarParameter* param = InitCVar(name);
		if (!param) return nullptr;

		param->type = CVarType::STRING;

		GetCVarArray<std::string>()->Add(defaultValue, currentValue, param);

		return param;
	}

	CVarParameter* CVarSystemImpl::InitCVar(const char* name)
	{

		uint32_t namehash = StringUtils::StringHash{ name };
		savedCVars[namehash] = CVarParameter{};

		CVarParameter& newParam = savedCVars[namehash];

		newParam.name = name;

		return &newParam;
	}

	template<typename T>
	T GetCVarCurrentByIndex(int32_t index) {
		return CVarSystemImpl::Get()->GetCVarArray<T>()->GetCurrent(index);
	}
	template<typename T>
	T* PtrGetCVarCurrentByIndex(int32_t index) {
		return CVarSystemImpl::Get()->GetCVarArray<T>()->GetCurrentPtr(index);
	}

	template<typename T>
	void SetCVarCurrentByIndex(int32_t index, const T& data) {
		CVarSystemImpl::Get()->GetCVarArray<T>()->SetCurrent(data, index);
	}
	
	namespace CVar
	{
		CVar_Float::CVar_Float(const char* name, double defaultValue, CVar::ProtectionLevel protectionLvl)
		{
			CVarParameter* cvar = CVarSystem::Get()->CreateFloatCVar(name, defaultValue, defaultValue);
			cvar->protectionLvl = protectionLvl;
			index = cvar->arrayIndex;
		}

		double CVar_Float::Get()
		{
			return GetCVarCurrentByIndex<CVarType>(index);
		}

		double* CVar_Float::GetPtr()
		{
			return PtrGetCVarCurrentByIndex<CVarType>(index);
		}

		float CVar_Float::GetFloat()
		{
			return static_cast<float>(Get());
		}

		float* CVar_Float::GetFloatPtr()
		{
			float* result = reinterpret_cast<float*>(GetPtr());
			return result;
		}

		void CVar_Float::Set(double f)
		{
			SetCVarCurrentByIndex<CVarType>(index, f);
		}

		CVar_Int::CVar_Int(const char* name, int32_t defaultValue, CVar::ProtectionLevel protectionLvl)
		{
			CVarParameter* cvar = CVarSystem::Get()->CreateIntCVar(name, defaultValue, defaultValue);
			cvar->protectionLvl = protectionLvl;
			index = cvar->arrayIndex;
		}

		int32_t CVar_Int::Get()
		{
			return GetCVarCurrentByIndex<CVarType>(index);
		}

		int32_t* CVar_Int::GetPtr()
		{
			return PtrGetCVarCurrentByIndex<CVarType>(index);
		}

		void CVar_Int::Set(int32_t val)
		{
			SetCVarCurrentByIndex<CVarType>(index, val);
		}

		void CVar_Int::Toggle() // boolean functionality
		{
			bool enabled = Get() != 0;

			Set(enabled ? 0 : 1);
		}

		CVar_String::CVar_String(const char* name , const char* defaultValue, CVar::ProtectionLevel protectionLvl)
		{
			CVarParameter* cvar = CVarSystem::Get()->CreateStringCVar(name, defaultValue, defaultValue);
			cvar->protectionLvl = protectionLvl;
			index = cvar->arrayIndex;
		}

		const char* CVar_String::Get()
		{
			return GetCVarCurrentByIndex<CVarType>(index).c_str();
		};

		void CVar_String::Set(std::string&& val)
		{
			SetCVarCurrentByIndex<CVarType>(index, val);
		}
	}
	
}

//const bool readonlyFlag = ((uint32_t)p->flags & (uint32_t)CVarFlags::EditReadOnly);
//const bool checkboxFlag = ((uint32_t)p->flags & (uint32_t)CVarFlags::EditCheckbox);
//const bool dragFlag = ((uint32_t)p->flags & (uint32_t)CVarFlags::EditFloatDrag);