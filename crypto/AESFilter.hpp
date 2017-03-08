
#include "SecureHelpers.hpp"
#include "../Filter.hpp"

namespace cg {

class AESEncryptFilter : public cg::Filter
{
public:
	AESEncryptFilter(const CryptoPP::SecByteBlock& key,
		const CryptoPP::SecByteBlock& iv)
		:m_key(key), m_iv(iv) {}
	/**Covnert data from one place and store to another.  Dest and source may
	be the same, but may not overlap if they are different pointers.
	\param src The place to read the data from.
	\param size The size of the data destination.
	\return An array view with the converted data.*/
	virtual ArrayView Transform(const char* src, std::size_t size) override
	{
		ArrayView av(size);
		cg::SecureHelpers::AESEncrypt(av.data(), src, size, size,m_key,m_iv);
		return av;
	}
private:
	/**The key used.*/
	const CryptoPP::SecByteBlock& m_key;
	/**The IV used.*/
	const CryptoPP::SecByteBlock& m_iv;
};

class AESDecryptFilter : public cg::Filter
{
public:
	AESDecryptFilter(const CryptoPP::SecByteBlock& key,
		const CryptoPP::SecByteBlock& iv)
		:m_key(key), m_iv(iv) {}
	/**Covnert data from one place and store to another.  Dest and source may
	be the same, but may not overlap if they are different pointers.
	\param src The place to read the data from.
	\param size The size of the data destination.
	\return An array view with the converted data.*/
	virtual ArrayView Transform(const char* src, std::size_t size) override
	{
		ArrayView av(size);
		cg::SecureHelpers::AESDecrypt(av.data(), src, size, size, m_key, m_iv);
		return av;
	}
private:
	/**The key used.*/
	const CryptoPP::SecByteBlock& m_key;
	/**The IV used.*/
	const CryptoPP::SecByteBlock& m_iv;

};


}