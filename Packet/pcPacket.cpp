enum class Type : uint8_t
{
	LOGIN_REQUEST = 0,
	LOGIN_RESPONSE = 1,
};
enum class LoginResult : uint8_t
{
	FAILED = 0,
	SUCCESS = 1,
};
#pragma pack(push, 1)
struct Header
{
	Type type;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct LoginRequest : Header
{
	char id[16];
	char password[16];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct LoginResponse : Header
{
	LoginResult result;
};
#pragma pack(pop)
