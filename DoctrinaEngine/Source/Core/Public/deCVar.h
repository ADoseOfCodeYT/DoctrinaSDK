#ifndef _DECVAR_H_
#define _DECVAR_H_

#include "../ThirdParty/string_utils.h"

// Ok so this shit is sooo confusing
// Lemme go into some lore here
// First, i looked at how quake does cvars, tried to implement but im too stupid to understand that god-level code
// Then, i looked at Source Engine's implementation, i understood that a bit better but ehhh i dont need it to be that big
// I was gonna look at Unreal but fuck me id rather fight a gorilla than look at that mess
// Anyways, the way this works is that CVarSystem is an interface (and a singleton) and CVarSystemImpl is the actual implementation
// Legit just copied https://vkguide.dev/docs/extra-chapter/cvar_system/ (fucking life saver)

namespace de
{
	class CVarParam;

	class CVarSystem
	{

	public:
		static CVarSystem* Get();

		virtual CVarParam* GetCVar(StringUtils::StringHash hash) = 0;

		virtual double GetFloatCVar(StringUtils::StringHash hash) = 0;
		virtual int32_t GetIntCVar(StringUtils::StringHash hash) = 0;
		virtual const char* GetStringCVar(StringUtils::StringHash hash) = 0;

		virtual void SetFloatCVar(StringUtils::StringHash hash, double value) = 0; 	// This is unsafe since this function doesnt respect the ProtectionLevel
		virtual void SetIntCVar(StringUtils::StringHash hash, int32_t value) = 0; // This is unsafe since this function doesnt respect the ProtectionLevel
		virtual void SetStringCVar(StringUtils::StringHash hash, const char* value) = 0; // This is unsafe since this function doesnt respect the ProtectionLevel

		virtual void SetFloatCVarSafe(StringUtils::StringHash hash, double value) = 0; // This is safe since this function does respect the ProtectionLevel
		virtual void SetIntCVarSafe(StringUtils::StringHash hash, int32_t value) = 0; // This is safe since this function does respect the ProtectionLevel
		virtual void SetStringCVarSafe(StringUtils::StringHash hash, const char* value) = 0; // This is safe since this function does respect the ProtectionLevel

		virtual CVarParam* CreateFloatCVar(const char* name, double defaultValue, double currentValue) = 0;
		virtual CVarParam* CreateIntCVar(const char* name, int32_t defaultValue, int32_t currentValue) = 0;
		virtual CVarParam* CreateStringCVar(const char* name, const char* defaultValue, const char* currentValue) = 0;

	};

	template<typename T>
	struct CVar_Base
	{
	protected:
		int index;
		using CVarType = T;
	};

	namespace cvar
	{
		enum class ProtectionLevel : uint32_t
		{
			None = 0,
			UnProtected = 1 << 1, // Just a chill as fuck, no shits given
			Protected = 1 << 2, // Ehh will cock block the console:: but its legs are wide open to Set()
		};

		struct CVar_Float : CVar_Base<double>
		{
			CVar_Float(const char* name, double defaultValue, ProtectionLevel protectionLvl = ProtectionLevel::UnProtected);

			double Get();
			double* GetPtr();
			float GetFloat();
			float* GetFloatPtr();
			void Set(double val);
			void SetSafe(double val);
		};

		struct CVar_Int : CVar_Base<int32_t>
		{
			CVar_Int(const char* name, int32_t defaultValue, ProtectionLevel protectionLvl = ProtectionLevel::UnProtected);
			int32_t Get();
			int32_t* GetPtr();
			void Set(int32_t val);
			void SetSafe(int32_t val);
			void Toggle(); // boolean functionality, value must be 1 or 0 for this to work
		};

		struct CVar_String : CVar_Base<std::string>
		{
			CVar_String(const char* name, const char* defaultValue, ProtectionLevel protectionLvl = ProtectionLevel::UnProtected);

			const char* Get();
			void Set(std::string&& val);
			void SetSafe(std::string&& val);
		};

	}
}

#endif // !_DECVAR_H_
