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

	class CVarParam
	{
	public:
		friend class CVarSystemImpl;

		int32_t ArrayIndex;

		CVarType Type;
		CVar::ProtectionLevel ProtectionLvl;
		std::string Name;
	};

	template<typename T>
	struct CVarStorage
	{
		T Initial;
		T Current;
		CVarParam* Param;
	};

	template<typename T>
	struct CVarArray
	{
		CVarStorage<T>* CVars{ nullptr };
		int32_t LastCVar{ 0 };

		CVarArray(size_t size)
		{
			CVars = new CVarStorage<T>[size]();
		}


		CVarStorage<T>* GetCurrentStorage(int32_t index)
		{
			return &CVars[index];
		}

		T* GetCurrentPtr(int32_t index)
		{
			return &CVars[index].Current;
		};

		T GetCurrent(int32_t index)
		{
			return CVars[index].Current;
		};

		void SetCurrent(const T& val, int32_t index)
		{
			CVars[index].Current = val;
		}

		int Add(const T& value, CVarParam* param)
		{
			int index = LastCVar;

			CVars[index].Current = value;
			CVars[index].Initial = value;
			CVars[index].Parameter = param;

			param->ArrayIndex = index;
			LastCVar++;

			return index;
		}

		int Add(const T& initialValue, const T& currentValue, CVarParam* param)
		{
			int index = LastCVar;

			CVars[index].Current = currentValue;
			CVars[index].Initial = initialValue;
			CVars[index].Param = param;

			param->ArrayIndex = index;
			LastCVar++;

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
		CVarParam* GetCVar(StringUtils::StringHash hash) override final;

		double GetFloatCVar(StringUtils::StringHash hash) override final;
		int32_t GetIntCVar(StringUtils::StringHash hash) override final;
		const char* GetStringCVar(StringUtils::StringHash hash) override final;

		void SetFloatCVar(StringUtils::StringHash hash, double value) override final;
		void SetIntCVar(StringUtils::StringHash hash, int32_t value) override final;
		void SetStringCVar(StringUtils::StringHash hash, const char* value) override final;

		void SetFloatCVarSafe(StringUtils::StringHash hash, double value) override final;
		void SetIntCVarSafe(StringUtils::StringHash hash, int32_t value) override final;
		void SetStringCVarSafe(StringUtils::StringHash hash, const char* value) override final;

		CVarParam* CreateFloatCVar(const char* name, double defaultValue, double currentValue) override final;
		CVarParam* CreateIntCVar(const char* name, int32_t defaultValue, int32_t currentValue) override final;
		CVarParam* CreateStringCVar(const char* name, const char* defaultValue, const char* currentValue) override final;

		template<typename T>
		CVarArray<T>* GetCVarArray();

		template<>
		CVarArray<int32_t>* GetCVarArray()
		{
			return &IntCVars;
		}
		template<>
		CVarArray<double>* GetCVarArray()
		{
			return &FloatCVars;
		}
		template<>
		CVarArray<std::string>* GetCVarArray()
		{
			return &StringCVars;
		}

		//templated get-set cvar versions for syntax sugar
		template<typename T>
		T* GetCVarCurrent(uint32_t namehash) {
			CVarParam* par = GetCVar(namehash);
			if (!par) {
				return nullptr;
			}
			else {
				return GetCVarArray<T>()->GetCurrentPtr(par->ArrayIndex);
			}
		}

		template<typename T>
		void SetCVarCurrent(uint32_t namehash, const T& value)
		{
			CVarParam* cvar = GetCVar(namehash);
			if (cvar)
			{
				GetCVarArray<T>()->SetCurrent(value, cvar->ArrayIndex);
			}
		}

		static CVarSystemImpl* Get()
		{
			return static_cast<CVarSystemImpl*>(CVarSystem::Get());
		}

	public:

		CVarArray<int32_t> IntCVars{ MAX_INT_CVARS };

		CVarArray<double> FloatCVars{ MAX_FLOAT_CVARS };

		CVarArray<std::string> StringCVars{ MAX_STRING_CVARS };

	private:

		CVarParam* InitCVar(const char* name);

	private:

		std::shared_mutex mutex_;

		std::unordered_map<uint32_t, CVarParam> m_SavedCVars;

		std::vector<CVarParam*> m_CachedEditParameters;
	};

	double CVarSystemImpl::GetFloatCVar(StringUtils::StringHash hash)
	{
		double* val = GetCVarCurrent<double>(hash);
		if (val)
		{
			return *val;
		}
		else
		{
			Console::Post("CVar couldn't be found", Console::LogLevel::Warning);
			return 0.0;
		}
	}

	int32_t CVarSystemImpl::GetIntCVar(StringUtils::StringHash hash)
	{
		int32_t* val = GetCVarCurrent<int32_t>(hash);
		if (val)
		{
			return *val;
		}
		else
		{
			Console::Post("CVar couldn't be found", Console::LogLevel::Warning);
			return 0;
		}
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

	CVarParam* CVarSystemImpl::GetCVar(StringUtils::StringHash hash)
	{
		std::shared_lock lock(mutex_);
		auto it = m_SavedCVars.find(hash);

		if (it != m_SavedCVars.end())
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
		CVarParam* cvar = GetCVar(hash);

		if (cvar)
		{
			switch (cvar->ProtectionLvl)
			{
			case CVar::ProtectionLevel::UnProtected:
				SetCVarCurrent<double>(hash, value);
				break;
			case CVar::ProtectionLevel::Protected:
				if (CVAR_protection_enabled.Get())
				{
					Console::Post("CVar : " + cvar->Name + " cant be changed because it is a protected CVar. Set protection_enabled to 0 to change the value of this CVar", Console::LogLevel::Warning);
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
		CVarParam* cvar = GetCVar(hash);

		if (cvar)
		{
			switch (cvar->ProtectionLvl)
			{
			case CVar::ProtectionLevel::UnProtected:
				SetCVarCurrent<int>(hash, value);
				break;
			case CVar::ProtectionLevel::Protected:
				if (CVAR_protection_enabled.Get())
				{
					Console::Post("CVar : " + cvar->Name + " cant be changed because it is a protected CVar. Set protection_enabled to 0 to change the value of this CVar", Console::LogLevel::Warning);
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
		CVarParam* cvar = GetCVar(hash);

		if (cvar)
		{
			switch (cvar->ProtectionLvl)
			{
			case CVar::ProtectionLevel::UnProtected:
				SetCVarCurrent<std::string>(hash, value);
				break;
			case CVar::ProtectionLevel::Protected:
				if (CVAR_protection_enabled.Get())
				{
					Console::Post("CVar : " + cvar->Name + " cant be changed because it is a protected CVar. Set protection_enabled to 0 to change the value of this CVar", Console::LogLevel::Warning);
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

	CVarParam* CVarSystemImpl::CreateFloatCVar(const char* name, double defaultValue, double currentValue)
	{
		std::unique_lock lock(mutex_);
		CVarParam* param = InitCVar(name);
		if (!param) return nullptr;

		param->Type = CVarType::FLOAT;

		GetCVarArray<double>()->Add(defaultValue, currentValue, param);

		return param;
	}

	CVarParam* CVarSystemImpl::CreateIntCVar(const char* name, int32_t defaultValue, int32_t currentValue)
	{
		std::unique_lock lock(mutex_);
		CVarParam* param = InitCVar(name);
		if (!param) return nullptr;

		param->Type = CVarType::INT;

		GetCVarArray<int32_t>()->Add(defaultValue, currentValue, param);

		return param;
	}

	CVarParam* CVarSystemImpl::CreateStringCVar(const char* name, const char* defaultValue, const char* currentValue)
	{
		std::unique_lock lock(mutex_);
		CVarParam* param = InitCVar(name);
		if (!param) return nullptr;

		param->Type = CVarType::STRING;

		GetCVarArray<std::string>()->Add(defaultValue, currentValue, param);

		return param;
	}

	CVarParam* CVarSystemImpl::InitCVar(const char* name)
	{

		uint32_t namehash = StringUtils::StringHash{ name };
		m_SavedCVars[namehash] = CVarParam{};

		CVarParam& newParam = m_SavedCVars[namehash];

		newParam.Name = name;

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
			CVarParam* cvar = CVarSystem::Get()->CreateFloatCVar(name, defaultValue, defaultValue);
			cvar->ProtectionLvl = protectionLvl;
			index = cvar->ArrayIndex;
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
			CVarParam* cvar = CVarSystem::Get()->CreateIntCVar(name, defaultValue, defaultValue);
			cvar->ProtectionLvl = protectionLvl;
			index = cvar->ArrayIndex;
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
			CVarParam* cvar = CVarSystem::Get()->CreateStringCVar(name, defaultValue, defaultValue);
			cvar->ProtectionLvl = protectionLvl;
			index = cvar->ArrayIndex;
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