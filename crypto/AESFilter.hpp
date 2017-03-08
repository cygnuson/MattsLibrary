
#include "SecureHelpers.hpp"
#include "../Filter.hpp"

namespace cg {

class AESEncryptFilter : public cg::Filter
{
public:
	AESEncryptFilter(const CryptoPP::SecByteBlock& key,
		const CryptoPP::SecByteBlock& iv)
		:m_key(key), m_iv(iv) {}
	/**Transform some data.

	\param dest The place to put the transformed data.
	\param src The place to read data.
	\param dSize The size of the destination.
	\param sSize The size of the source data.*/
	inline virtual void Transform(char * dest,
		const char * src,
		std::size_t dSize,
		std::size_t sSize) override
	{
		cg::SecureHelpers::AESEncrypt(dest, src, dSize, sSize,m_key,m_iv);
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
	/**Transform some data.

	\param dest The place to put the transformed data.
	\param src The place to read data.
	\param dSize The size of the destination.
	\param sSize The size of the source data.*/
	inline virtual void Transform(char * dest,
		const char * src,
		std::size_t dSize,
		std::size_t sSize) override
	{
		cg::SecureHelpers::AESDecrypt(dest, src, dSize, sSize, m_key, m_iv);
	}
private:
	/**The key used.*/
	const CryptoPP::SecByteBlock& m_key;
	/**The IV used.*/
	const CryptoPP::SecByteBlock& m_iv;

};


}